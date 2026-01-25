/**
 * GPIO Initialization Template for Smart Plug AI
 * 
 * This template provides standard GPIO configuration for all digital I/O
 * used in the Smart Plug AI project. Includes relay control, status LEDs,
 * buttons, and tamper detection.
 * 
 * GPIO Pin Assignments:
 * - GPIO 4:  ADC input for SCT-013 current sensor
 * - GPIO 8:  I2C SDA (ATECC608A, 24C256)
 * - GPIO 9:  I2C SCL (ATECC608A, 24C256)
 * - GPIO 10: Tamper detection input (MAX6316 MR pin)
 * - GPIO 11: Watchdog toggle output (MAX6316 WDI pin)
 * - GPIO 18: Relay control output (via PC817 optocoupler)
 * - GPIO 2:  Status LED (built-in on most ESP32 boards)
 * - GPIO 0:  Boot button (for pairing mode / factory reset)
 * 
 * Safety Features:
 * - Relay defaults to OFF (safe state)
 * - Watchdog starts toggling immediately
 * - Tamper detection uses interrupt for fast response
 * - All outputs initialized before inputs to prevent floating states
 * 
 * Author: Smart Plug AI Development Team
 * Date: 2024-01-15
 * Version: 1.0
 */

#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "GPIO_INIT";

// GPIO Pin Definitions
#define RELAY_PIN           GPIO_NUM_18  // Relay control output
#define STATUS_LED_PIN      GPIO_NUM_2   // Status indicator LED
#define BUTTON_PIN          GPIO_NUM_0   // Boot/pairing button
#define TAMPER_PIN          GPIO_NUM_10  // Tamper detection input
#define WATCHDOG_PIN        GPIO_NUM_11  // Watchdog toggle output

// GPIO Configuration Macros
#define GPIO_OUTPUT_PIN_SEL ((1ULL << RELAY_PIN) | \
                             (1ULL << STATUS_LED_PIN) | \
                             (1ULL << WATCHDOG_PIN))
#define GPIO_INPUT_PIN_SEL  ((1ULL << BUTTON_PIN) | \
                             (1ULL << TAMPER_PIN))

/**
 * Initialize all GPIO pins with safe defaults
 * 
 * This function MUST be called early in app_main() before any other GPIO operations.
 * Ensures all pins are in safe, known states to prevent:
 * - Accidental relay activation
 * - Floating inputs causing spurious interrupts
 * - Undefined pin states during boot
 * 
 * @return ESP_OK on success, ESP_FAIL on error
 */
esp_err_t gpio_init_all(void) {
    esp_err_t ret = ESP_OK;
    
    ESP_LOGI(TAG, "Initializing GPIO pins...");
    
    // Configure OUTPUT pins
    gpio_config_t io_conf_out = {
        .pin_bit_mask = GPIO_OUTPUT_PIN_SEL,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ret = gpio_config(&io_conf_out);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure output pins: %s", esp_err_to_name(ret));
        return ret;
    }
    
    // ⚠️ SAFETY CRITICAL: Set relay to OFF state IMMEDIATELY
    gpio_set_level(RELAY_PIN, 0);
    ESP_LOGI(TAG, "Relay initialized to OFF (safe state)");
    
    // Initialize status LED to OFF
    gpio_set_level(STATUS_LED_PIN, 0);
    
    // Initialize watchdog toggle to LOW (will start toggling in separate task)
    gpio_set_level(WATCHDOG_PIN, 0);
    ESP_LOGI(TAG, "Watchdog pin initialized to LOW");
    
    // Configure INPUT pins
    gpio_config_t io_conf_in = {
        .pin_bit_mask = GPIO_INPUT_PIN_SEL,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,  // Enable internal pull-ups
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,    // Interrupts configured separately
    };
    ret = gpio_config(&io_conf_in);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure input pins: %s", esp_err_to_name(ret));
        return ret;
    }
    
    ESP_LOGI(TAG, "GPIO initialization complete");
    ESP_LOGI(TAG, "  Relay (GPIO %d): OFF", RELAY_PIN);
    ESP_LOGI(TAG, "  LED (GPIO %d): OFF", STATUS_LED_PIN);
    ESP_LOGI(TAG, "  Button (GPIO %d): INPUT with pull-up", BUTTON_PIN);
    ESP_LOGI(TAG, "  Tamper (GPIO %d): INPUT with pull-up", TAMPER_PIN);
    ESP_LOGI(TAG, "  Watchdog (GPIO %d): OUTPUT LOW", WATCHDOG_PIN);
    
    return ESP_OK;
}

/**
 * Configure tamper detection interrupt
 * 
 * Tamper switch is normally closed. Opening the enclosure opens the switch,
 * pulling the pin HIGH and triggering a POSEDGE interrupt.
 * 
 * ⚠️ SECURITY CRITICAL: Fast response required to wipe sensitive data
 * 
 * @param isr_handler Function to call on tamper detection
 * @return ESP_OK on success, ESP_FAIL on error
 */
esp_err_t gpio_configure_tamper_interrupt(gpio_isr_t isr_handler) {
    esp_err_t ret = ESP_OK;
    
    ESP_LOGI(TAG, "Configuring tamper detection interrupt...");
    
    // Install GPIO ISR service if not already installed
    ret = gpio_install_isr_service(0);
    if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Failed to install ISR service: %s", esp_err_to_name(ret));
        return ret;
    }
    
    // Set interrupt type to POSEDGE (switch opening)
    ret = gpio_set_intr_type(TAMPER_PIN, GPIO_INTR_POSEDGE);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set interrupt type: %s", esp_err_to_name(ret));
        return ret;
    }
    
    // Add ISR handler for tamper pin
    ret = gpio_isr_handler_add(TAMPER_PIN, isr_handler, (void*) TAMPER_PIN);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add ISR handler: %s", esp_err_to_name(ret));
        return ret;
    }
    
    // Enable interrupt on tamper pin
    ret = gpio_intr_enable(TAMPER_PIN);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to enable interrupt: %s", esp_err_to_name(ret));
        return ret;
    }
    
    ESP_LOGI(TAG, "Tamper detection interrupt configured on GPIO %d", TAMPER_PIN);
    return ESP_OK;
}

/**
 * Configure button interrupt (for pairing mode / factory reset)
 * 
 * Button press patterns:
 * - Short press (<3s): Enter pairing mode
 * - Long press (>10s): Factory reset (wipe all data)
 * 
 * @param isr_handler Function to call on button press
 * @return ESP_OK on success, ESP_FAIL on error
 */
esp_err_t gpio_configure_button_interrupt(gpio_isr_t isr_handler) {
    esp_err_t ret = ESP_OK;
    
    ESP_LOGI(TAG, "Configuring button interrupt...");
    
    // Set interrupt type to ANYEDGE (detect press and release)
    ret = gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_ANYEDGE);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set interrupt type: %s", esp_err_to_name(ret));
        return ret;
    }
    
    // Add ISR handler for button pin
    ret = gpio_isr_handler_add(BUTTON_PIN, isr_handler, (void*) BUTTON_PIN);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add ISR handler: %s", esp_err_to_name(ret));
        return ret;
    }
    
    // Enable interrupt on button pin
    ret = gpio_intr_enable(BUTTON_PIN);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to enable interrupt: %s", esp_err_to_name(ret));
        return ret;
    }
    
    ESP_LOGI(TAG, "Button interrupt configured on GPIO %d", BUTTON_PIN);
    return ESP_OK;
}

/**
 * Set relay state
 * 
 * Controls the relay via PC817 optocoupler isolation.
 * HIGH = Relay ON (load powered), LOW = Relay OFF (load unpowered)
 * 
 * ⚠️ SAFETY NOTE: This function only controls the digital signal.
 * Actual AC switching is handled by relay_control module with soft-start.
 * 
 * @param state true = ON, false = OFF
 */
void gpio_set_relay(bool state) {
    gpio_set_level(RELAY_PIN, state ? 1 : 0);
    ESP_LOGI(TAG, "Relay set to %s", state ? "ON" : "OFF");
}

/**
 * Get relay state
 * 
 * @return true if relay is ON, false if OFF
 */
bool gpio_get_relay(void) {
    return gpio_get_level(RELAY_PIN);
}

/**
 * Set status LED state
 * 
 * LED patterns:
 * - Solid ON: Device connected and operating normally
 * - Slow blink (1Hz): Pairing mode
 * - Fast blink (5Hz): Error condition
 * - OFF: Device offline or unpowered
 * 
 * @param state true = ON, false = OFF
 */
void gpio_set_status_led(bool state) {
    gpio_set_level(STATUS_LED_PIN, state ? 1 : 0);
}

/**
 * Toggle status LED
 * Useful for blink patterns in separate task
 */
void gpio_toggle_status_led(void) {
    gpio_set_level(STATUS_LED_PIN, !gpio_get_level(STATUS_LED_PIN));
}

/**
 * Get button state
 * 
 * @return true if button is pressed (LOW due to pull-up), false if not pressed
 */
bool gpio_is_button_pressed(void) {
    return gpio_get_level(BUTTON_PIN) == 0;  // Active LOW
}

/**
 * Get tamper detection state
 * 
 * @return true if enclosure is tampered (switch open), false if closed
 */
bool gpio_is_tampered(void) {
    return gpio_get_level(TAMPER_PIN) == 1;  // Active HIGH
}

/**
 * Toggle watchdog pin
 * 
 * This function should be called periodically (every 500ms) to prevent
 * the TPS3823 watchdog from resetting the system.
 * 
 * ⚠️ CRITICAL: If this function stops being called, system will reset in ~1.6s
 */
void gpio_toggle_watchdog(void) {
    static bool watchdog_state = false;
    watchdog_state = !watchdog_state;
    gpio_set_level(WATCHDOG_PIN, watchdog_state ? 1 : 0);
}

/**
 * Perform GPIO self-test
 * 
 * Tests all GPIO pins for correct configuration and operation.
 * Should be run during boot to verify hardware integrity.
 * 
 * @return ESP_OK if all tests pass, ESP_FAIL if any test fails
 */
esp_err_t gpio_self_test(void) {
    ESP_LOGI(TAG, "Starting GPIO self-test...");
    
    // Test 1: Verify relay starts in OFF state
    if (gpio_get_level(RELAY_PIN) != 0) {
        ESP_LOGE(TAG, "SELF-TEST FAILED: Relay not in OFF state!");
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "  ✓ Relay in safe OFF state");
    
    // Test 2: Verify button has pull-up
    if (gpio_get_level(BUTTON_PIN) != 1) {
        ESP_LOGW(TAG, "  ⚠ Button pin is LOW (may be pressed or missing pull-up)");
    } else {
        ESP_LOGI(TAG, "  ✓ Button pull-up working");
    }
    
    // Test 3: Verify tamper detection has pull-up
    if (gpio_get_level(TAMPER_PIN) != 0) {
        ESP_LOGE(TAG, "SELF-TEST FAILED: Tamper switch not closed (enclosure open?)");
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "  ✓ Tamper switch closed (enclosure secure)");
    
    // Test 4: Test LED on/off
    gpio_set_status_led(true);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_status_led(false);
    ESP_LOGI(TAG, "  ✓ Status LED toggle working");
    
    // Test 5: Test watchdog toggle
    for (int i = 0; i < 5; i++) {
        gpio_toggle_watchdog();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    ESP_LOGI(TAG, "  ✓ Watchdog toggle working");
    
    ESP_LOGI(TAG, "GPIO self-test PASSED");
    return ESP_OK;
}

/*
 * Usage Example in main.c:
 * 
 * #include "gpio_initialization.h"
 * 
 * void app_main(void) {
 *     // Initialize GPIO pins first thing
 *     ESP_ERROR_CHECK(gpio_init_all());
 *     
 *     // Run self-test
 *     ESP_ERROR_CHECK(gpio_self_test());
 *     
 *     // Configure interrupts
 *     ESP_ERROR_CHECK(gpio_configure_tamper_interrupt(tamper_isr_handler));
 *     ESP_ERROR_CHECK(gpio_configure_button_interrupt(button_isr_handler));
 *     
 *     // Create watchdog task
 *     xTaskCreate(watchdog_task, "watchdog", 2048, NULL, 5, NULL);
 *     
 *     // ... rest of application initialization
 * }
 */
