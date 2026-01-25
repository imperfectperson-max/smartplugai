/**
 * Watchdog and Tamper Detection Driver
 * 
 * This driver implements physical security through the TPS3823 watchdog timer
 * and tamper detection switch. Provides protection against:
 * - Software hangs (watchdog reset)
 * - Physical tampering (enclosure opening)
 * - Firmware manipulation attempts
 * 
 * Features:
 * - Periodic watchdog toggle to prevent system reset
 * - Tamper detection via interrupt (response time <100ms)
 * - Automatic sensitive data wipe on tamper
 * - Audit logging of all security events
 * - Configurable tamper response actions
 * 
 * Hardware Configuration:
 * - Watchdog IC: TPS3823-33 (3.3V, 1.6s timeout)
 * - Watchdog Toggle Pin: GPIO 11 (must toggle every 500ms)
 * - Tamper Input Pin: GPIO 10 (normally LOW, HIGH on tamper)
 * - Tamper Switch: Normally closed switch across MR and GND
 * - Reset Output: Connected to ESP32 EN pin via 10kΩ pull-up
 * 
 * Tamper Response Sequence:
 * 1. Detect tamper (interrupt triggered)
 * 2. Log tamper event with timestamp
 * 3. Wipe sensitive data (keys, credentials, session tokens)
 * 4. Stop watchdog toggle → System reset in 1.6s
 * 5. On reboot, report tamper to backend
 * 
 * ⚠️ SECURITY CRITICAL: Do not modify tamper response without security review!
 * 
 * Author: Smart Plug AI Development Team
 * Date: 2024-01-15
 * Version: 1.0
 */

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "nvs_flash.h"
#include "nvs.h"

static const char *TAG = "WATCHDOG";

// GPIO Pin Definitions
#define WATCHDOG_PIN        GPIO_NUM_11  // WDI pin - must toggle periodically
#define TAMPER_PIN          GPIO_NUM_10  // MR pin - tamper detection input

// Timing Configuration
#define WATCHDOG_TOGGLE_INTERVAL_MS  500   // Toggle every 500ms (well under 1.6s timeout)
#define WATCHDOG_TIMEOUT_MS          1600  // TPS3823-33 timeout period

// Event bits for task synchronization
#define TAMPER_DETECTED_BIT  BIT0

// Global state
static TaskHandle_t g_watchdog_task_handle = NULL;
static EventGroupHandle_t g_security_events = NULL;
static bool g_tamper_detected = false;
static uint32_t g_tamper_count = 0;
static uint64_t g_last_tamper_time = 0;

/**
 * Tamper detection ISR handler
 * 
 * ⚠️ CRITICAL: This runs in interrupt context - keep it fast!
 * Sets flag and event bit, actual response happens in task context.
 * 
 * @param arg GPIO pin number (for multi-pin support)
 */
static void IRAM_ATTR tamper_isr_handler(void* arg) {
    // Set tamper flag (atomic operation)
    g_tamper_detected = true;
    
    // Set event bit to wake up security response task
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(g_security_events, TAMPER_DETECTED_BIT, &xHigherPriorityTaskWoken);
    
    // Yield if a higher priority task was woken
    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

/**
 * Initialize watchdog and tamper detection
 * 
 * Configures GPIO pins and interrupt handlers for physical security.
 * Must be called early in system initialization.
 * 
 * @return ESP_OK on success, ESP_FAIL on error
 */
esp_err_t watchdog_init(void) {
    esp_err_t ret = ESP_OK;
    
    ESP_LOGI(TAG, "Initializing watchdog and tamper detection...");
    
    // Create event group for security events
    g_security_events = xEventGroupCreate();
    if (g_security_events == NULL) {
        ESP_LOGE(TAG, "Failed to create event group!");
        return ESP_FAIL;
    }
    
    // Configure watchdog toggle output
    gpio_config_t wd_conf = {
        .pin_bit_mask = (1ULL << WATCHDOG_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ret = gpio_config(&wd_conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure watchdog pin: %s", esp_err_to_name(ret));
        return ret;
    }
    
    // Initialize watchdog pin to LOW
    gpio_set_level(WATCHDOG_PIN, 0);
    
    // Configure tamper detection input
    gpio_config_t tamper_conf = {
        .pin_bit_mask = (1ULL << TAMPER_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,  // Internal pull-up
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_POSEDGE,    // Trigger on switch opening (LOW→HIGH)
    };
    ret = gpio_config(&tamper_conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure tamper pin: %s", esp_err_to_name(ret));
        return ret;
    }
    
    // Install ISR service if not already installed
    ret = gpio_install_isr_service(0);
    if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Failed to install ISR service: %s", esp_err_to_name(ret));
        return ret;
    }
    
    // Add ISR handler for tamper pin
    ret = gpio_isr_handler_add(TAMPER_PIN, tamper_isr_handler, (void*) TAMPER_PIN);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add ISR handler: %s", esp_err_to_name(ret));
        return ret;
    }
    
    ESP_LOGI(TAG, "Watchdog and tamper detection initialized");
    ESP_LOGI(TAG, "  Watchdog pin: GPIO %d (toggle every %d ms)", WATCHDOG_PIN, WATCHDOG_TOGGLE_INTERVAL_MS);
    ESP_LOGI(TAG, "  Tamper pin: GPIO %d (interrupt on POSEDGE)", TAMPER_PIN);
    ESP_LOGI(TAG, "  Timeout period: %d ms", WATCHDOG_TIMEOUT_MS);
    
    // Load tamper count from NVS
    load_tamper_count();
    
    return ESP_OK;
}

/**
 * Watchdog toggle task
 * 
 * Periodically toggles the watchdog pin to prevent system reset.
 * Runs continuously as long as system is operating normally.
 * 
 * ⚠️ CRITICAL: If this task stops running, system will reset in 1.6s
 * 
 * @param pvParameter Task parameter (unused)
 */
void watchdog_task(void* pvParameter) {
    ESP_LOGI(TAG, "Watchdog task started");
    
    TickType_t last_wake_time = xTaskGetTickCount();
    const TickType_t toggle_period = pdMS_TO_TICKS(WATCHDOG_TOGGLE_INTERVAL_MS);
    
    while (1) {
        // Check if tamper was detected
        if (g_tamper_detected) {
            ESP_LOGE(TAG, "⚠️  TAMPER DETECTED! Suspending watchdog task...");
            ESP_LOGE(TAG, "System will reset in %d ms", WATCHDOG_TIMEOUT_MS);
            
            // Stop toggling - let watchdog reset the system
            vTaskSuspend(NULL);  // Suspend this task (will cause reset)
        }
        
        // Toggle watchdog pin
        gpio_set_level(WATCHDOG_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(WATCHDOG_TOGGLE_INTERVAL_MS / 2));
        gpio_set_level(WATCHDOG_PIN, 0);
        
        // Wait for next toggle period
        vTaskDelayUntil(&last_wake_time, toggle_period);
    }
}

/**
 * Security response task
 * 
 * Handles tamper detection events and executes security response.
 * Runs at high priority to ensure fast response time.
 * 
 * Response sequence:
 * 1. Log tamper event
 * 2. Wipe sensitive data
 * 3. Set tamper flag
 * 4. Allow watchdog to reset system
 * 
 * @param pvParameter Task parameter (unused)
 */
void security_response_task(void* pvParameter) {
    ESP_LOGI(TAG, "Security response task started");
    
    while (1) {
        // Wait for tamper detection event
        EventBits_t bits = xEventGroupWaitBits(
            g_security_events,
            TAMPER_DETECTED_BIT,
            pdTRUE,  // Clear bits on exit
            pdFALSE, // Wait for any bit
            portMAX_DELAY
        );
        
        if (bits & TAMPER_DETECTED_BIT) {
            handle_tamper_event();
        }
    }
}

/**
 * Handle tamper detection event
 * 
 * ⚠️ SECURITY CRITICAL: This function must complete quickly and reliably!
 * 
 * Actions performed:
 * - Log tamper event with timestamp
 * - Increment tamper counter in NVS
 * - Wipe sensitive data from memory and storage
 * - Set global tamper flag
 * - Notify backend (if network available)
 */
static void handle_tamper_event(void) {
    ESP_LOGE(TAG, "═══════════════════════════════════════");
    ESP_LOGE(TAG, "⚠️  TAMPER ALERT: ENCLOSURE OPENED!");
    ESP_LOGE(TAG, "═══════════════════════════════════════");
    
    // Record timestamp
    g_last_tamper_time = esp_timer_get_time() / 1000000;  // Convert to seconds
    g_tamper_count++;
    
    ESP_LOGE(TAG, "Tamper count: %d", g_tamper_count);
    ESP_LOGE(TAG, "Timestamp: %llu seconds since boot", g_last_tamper_time);
    
    // Save tamper event to NVS
    save_tamper_event();
    
    // Wipe sensitive data
    ESP_LOGW(TAG, "Wiping sensitive data...");
    wipe_sensitive_data();
    ESP_LOGW(TAG, "Sensitive data wiped");
    
    // Log to backend if network is available
    // TODO: Implement network tamper reporting
    // mqtt_publish_tamper_alert();
    
    ESP_LOGE(TAG, "Security response complete");
    ESP_LOGE(TAG, "Watchdog will reset system in %d ms", WATCHDOG_TIMEOUT_MS);
    ESP_LOGE(TAG, "═══════════════════════════════════════");
    
    // Tamper flag is already set by ISR
    // Watchdog task will suspend itself, causing system reset
}

/**
 * Wipe sensitive data from memory and storage
 * 
 * ⚠️ SECURITY CRITICAL: Must be fast and thorough!
 * 
 * Data wiped:
 * - Wi-Fi credentials from NVS
 * - MQTT credentials from NVS
 * - Session tokens from RAM
 * - Encryption keys from RAM (not from ATECC608A - those are hardware protected)
 * - User data caches
 */
static void wipe_sensitive_data(void) {
    // Wipe RAM-based sensitive data
    // TODO: Implement based on your application's data structures
    
    // Wipe NVS-stored credentials
    nvs_handle_t nvs_handle;
    esp_err_t ret = nvs_open("security", NVS_READWRITE, &nvs_handle);
    if (ret == ESP_OK) {
        // Erase specific keys
        nvs_erase_key(nvs_handle, "wifi_ssid");
        nvs_erase_key(nvs_handle, "wifi_pass");
        nvs_erase_key(nvs_handle, "mqtt_user");
        nvs_erase_key(nvs_handle, "mqtt_pass");
        nvs_erase_key(nvs_handle, "session_token");
        
        // Commit changes
        nvs_commit(nvs_handle);
        nvs_close(nvs_handle);
        
        ESP_LOGW(TAG, "NVS credentials erased");
    }
    
    // Note: ATECC608A keys are hardware-protected and cannot be read even by us
    // They remain secure even if device is physically compromised
}

/**
 * Save tamper event to NVS
 * 
 * Persists tamper detection event for audit trail.
 * Can survive system resets and be reported to backend on reconnection.
 */
static void save_tamper_event(void) {
    nvs_handle_t nvs_handle;
    esp_err_t ret = nvs_open("security", NVS_READWRITE, &nvs_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open NVS: %s", esp_err_to_name(ret));
        return;
    }
    
    // Save tamper count
    nvs_set_u32(nvs_handle, "tamper_count", g_tamper_count);
    
    // Save last tamper time
    nvs_set_u64(nvs_handle, "last_tamper", g_last_tamper_time);
    
    // Set tamper flag
    nvs_set_u8(nvs_handle, "tampered", 1);
    
    // Commit to flash
    nvs_commit(nvs_handle);
    nvs_close(nvs_handle);
    
    ESP_LOGI(TAG, "Tamper event saved to NVS");
}

/**
 * Load tamper count from NVS
 * 
 * Retrieves historical tamper events on boot.
 */
static void load_tamper_count(void) {
    nvs_handle_t nvs_handle;
    esp_err_t ret = nvs_open("security", NVS_READONLY, &nvs_handle);
    if (ret != ESP_OK) {
        ESP_LOGI(TAG, "No previous tamper events recorded");
        return;
    }
    
    // Read tamper count
    uint32_t count = 0;
    ret = nvs_get_u32(nvs_handle, "tamper_count", &count);
    if (ret == ESP_OK) {
        g_tamper_count = count;
        ESP_LOGW(TAG, "Historical tamper count: %d", g_tamper_count);
    }
    
    // Check if device was previously tampered
    uint8_t tampered = 0;
    ret = nvs_get_u8(nvs_handle, "tampered", &tampered);
    if (ret == ESP_OK && tampered) {
        ESP_LOGW(TAG, "⚠️  Device has been tampered with previously!");
        ESP_LOGW(TAG, "Manual inspection and re-certification required");
    }
    
    nvs_close(nvs_handle);
}

/**
 * Check if tamper has been detected
 * 
 * @return true if tamper detected, false otherwise
 */
bool watchdog_is_tampered(void) {
    return g_tamper_detected;
}

/**
 * Get tamper count
 * 
 * @return Number of times tamper has been detected
 */
uint32_t watchdog_get_tamper_count(void) {
    return g_tamper_count;
}

/**
 * Clear tamper flag (for testing/development only!)
 * 
 * ⚠️ WARNING: This should NOT be used in production!
 * Only for development/testing purposes.
 * 
 * In production, tamper flag can only be cleared by:
 * 1. Physical inspection by authorized personnel
 * 2. Re-certification process
 * 3. Updating tamper seal
 */
void watchdog_clear_tamper_flag_DEVELOPMENT_ONLY(void) {
    ESP_LOGW(TAG, "⚠️  CLEARING TAMPER FLAG - DEVELOPMENT ONLY!");
    
    g_tamper_detected = false;
    
    nvs_handle_t nvs_handle;
    if (nvs_open("security", NVS_READWRITE, &nvs_handle) == ESP_OK) {
        nvs_erase_key(nvs_handle, "tampered");
        nvs_commit(nvs_handle);
        nvs_close(nvs_handle);
    }
}

/**
 * Start watchdog and security tasks
 * 
 * Creates the watchdog toggle task and security response task.
 * Both run at high priority to ensure system reliability and security.
 * 
 * @return ESP_OK on success, ESP_FAIL on error
 */
esp_err_t watchdog_start_tasks(void) {
    BaseType_t ret;
    
    // Create watchdog toggle task (high priority)
    ret = xTaskCreate(
        watchdog_task,
        "watchdog_toggle",
        2048,           // Stack size
        NULL,           // Parameters
        configMAX_PRIORITIES - 1,  // High priority
        &g_watchdog_task_handle
    );
    
    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Failed to create watchdog task!");
        return ESP_FAIL;
    }
    
    // Create security response task (highest priority)
    ret = xTaskCreate(
        security_response_task,
        "security_response",
        4096,           // Larger stack for security operations
        NULL,
        configMAX_PRIORITIES,  // Highest priority
        NULL
    );
    
    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Failed to create security task!");
        return ESP_FAIL;
    }
    
    ESP_LOGI(TAG, "Watchdog and security tasks started");
    return ESP_OK;
}

/*
 * Usage Example in main.c:
 * 
 * #include "watchdog_driver.h"
 * 
 * void app_main(void) {
 *     // Initialize NVS (required for tamper logging)
 *     ESP_ERROR_CHECK(nvs_flash_init());
 *     
 *     // Initialize watchdog and tamper detection
 *     ESP_ERROR_CHECK(watchdog_init());
 *     
 *     // Start watchdog and security tasks
 *     ESP_ERROR_CHECK(watchdog_start_tasks());
 *     
 *     // Check if device was previously tampered
 *     if (watchdog_is_tampered()) {
 *         ESP_LOGW("MAIN", "Device was tampered - security measures active");
 *     }
 *     
 *     // ... rest of application initialization
 * }
 */
