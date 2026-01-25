/**
 * @file atecc608a_driver.c
 * @brief ATECC608A Secure Element Driver for ESP32-S3
 * 
 * This driver provides a secure interface to the ATECC608A cryptographic
 * co-processor. It handles device initialization, key generation, signing,
 * and verification operations.
 * 
 * WEEK 1 Implementation Notes:
 * - I2C communication at address 0x60
 * - Connected via GPIO8 (SDA) and GPIO9 (SCL)
 * - Requires 2.2kΩ pull-up resistors on both lines
 * - Configuration locking is IRREVERSIBLE - test thoroughly first!
 * 
 * Security Features:
 * - Hardware key storage (keys never leave chip)
 * - ECDSA P-256 signature generation
 * - Device attestation support
 * - Tamper-resistant design
 * 
 * @date January 2026
 * @version 1.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "cryptoauthlib.h"

static const char *TAG = "ATECC608A";

// I2C configuration (Week 1 hardware setup)
#define I2C_MASTER_SCL_IO    9          /*!< GPIO number for I2C master clock */
#define I2C_MASTER_SDA_IO    8          /*!< GPIO number for I2C master data  */
#define I2C_MASTER_NUM       I2C_NUM_0  /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ   100000     /*!< I2C master clock frequency */

// ATECC608A I2C configuration
#define ATECC608A_I2C_ADDR   0xC0       /*!< ATECC608A I2C address (7-bit: 0x60) */

/**
 * @brief Initialize I2C bus for ATECC608A communication
 * 
 * Sets up I2C master mode with:
 * - 100kHz clock (safe for long wire runs)
 * - Internal pull-ups disabled (using external 2.2kΩ)
 * - SDA timeout enabled
 * 
 * @return ESP_OK on success, error code otherwise
 */
static esp_err_t i2c_master_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_DISABLE,  // Using external 2.2kΩ pull-ups
        .scl_pullup_en = GPIO_PULLUP_DISABLE,  // Using external 2.2kΩ pull-ups
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    
    esp_err_t err = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2C config failed: %s", esp_err_to_name(err));
        return err;
    }
    
    err = i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2C driver install failed: %s", esp_err_to_name(err));
        return err;
    }
    
    ESP_LOGI(TAG, "I2C master initialized on SDA=%d, SCL=%d", 
             I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO);
    return ESP_OK;
}

/**
 * @brief Initialize ATECC608A secure element
 * 
 * Initialization sequence:
 * 1. Configure I2C interface
 * 2. Wake up ATECC608A
 * 3. Read configuration zone
 * 4. Verify device is provisioned
 * 
 * ⚠️ WEEK 1 WARNING:
 * Do NOT lock configuration until you've:
 * - Tested all operations on breadboard
 * - Verified key generation works
 * - Backed up configuration data
 * - Understand locking is IRREVERSIBLE
 * 
 * @return ESP_OK if device is ready
 *         ESP_ERR_NOT_FINISHED if needs provisioning
 *         ESP_FAIL on error
 */
esp_err_t atecc608a_init(void)
{
    ESP_LOGI(TAG, "Initializing ATECC608A secure element...");
    
    // Initialize I2C bus
    esp_err_t err = i2c_master_init();
    if (err != ESP_OK) {
        return err;
    }
    
    // Configure ATECC608A interface
    ATCAIfaceCfg cfg = {
        .iface_type = ATCA_I2C_IFACE,
        .devtype = ATECC608A,
        .atcai2c.slave_address = ATECC608A_I2C_ADDR,
        .atcai2c.bus = I2C_MASTER_NUM,
        .atcai2c.baud = I2C_MASTER_FREQ_HZ,
        .wake_delay = 1500,     // Wake-up delay in microseconds
        .rx_retries = 20        // Number of retries on communication errors
    };
    
    // Initialize CryptoAuthLib
    ATCA_STATUS status = atcab_init(&cfg);
    if (status != ATCA_SUCCESS) {
        ESP_LOGE(TAG, "ATECC608A init failed: 0x%02X", status);
        return ESP_FAIL;
    }
    
    ESP_LOGI(TAG, "ATECC608A communication established");
    
    // Read device info
    uint8_t revision[4];
    status = atcab_info(revision);
    if (status == ATCA_SUCCESS) {
        ESP_LOGI(TAG, "Device revision: %02X %02X %02X %02X", 
                 revision[0], revision[1], revision[2], revision[3]);
    }
    
    // Check if device is configured
    bool is_locked = false;
    status = atcab_is_locked(LOCK_ZONE_CONFIG, &is_locked);
    if (status == ATCA_SUCCESS) {
        if (is_locked) {
            ESP_LOGI(TAG, "Configuration zone is LOCKED (provisioned)");
            return ESP_OK;
        } else {
            ESP_LOGW(TAG, "Configuration zone is UNLOCKED");
            ESP_LOGW(TAG, "Device needs provisioning - see provisioning guide");
            return ESP_ERR_NOT_FINISHED;
        }
    }
    
    ESP_LOGE(TAG, "Failed to check lock status: 0x%02X", status);
    return ESP_FAIL;
}

/**
 * Week 1 Safety Checklist:
 * 
 * Before using this driver:
 * [ ] I2C pull-up resistors installed (2.2kΩ to 3.3V)
 * [ ] SDA connected to GPIO8
 * [ ] SCL connected to GPIO9
 * [ ] ATECC608A powered from 3.3V (NOT 5V!)
 * [ ] No solder bridges on I2C pins
 * [ ] Tested I2C scanner shows device at 0x60
 * [ ] Understood that locking is IRREVERSIBLE
 * [ ] Have backup plan if something goes wrong
 * 
 * Common Issues:
 * - Device not detected: Check pull-ups and wiring
 * - Communication errors: Reduce I2C speed
 * - Random failures: Check power supply stability
 */
