/**
 * ATECC608A Secure Element Driver
 * 
 * This driver provides a high-level interface to the Microchip ATECC608A
 * hardware security module for the Smart Plug AI project.
 * 
 * Features:
 * - Device initialization and configuration verification
 * - ECC key pair generation (NIST P-256)
 * - ECDSA signature generation and verification
 * - Hardware random number generation
 * - Secure key storage (private keys never leave the chip)
 * - TLS certificate storage
 * 
 * Security Features:
 * - Private keys stored in locked slots (never readable)
 * - Device attestation for firmware integrity verification
 * - Encrypted I2C communication (optional)
 * - Anti-tampering measures
 * 
 * Hardware Configuration:
 * - I2C Address: 0x60 (default, configurable to 0x6A-0x6E)
 * - I2C Speed: 100kHz (standard mode) or 400kHz (fast mode)
 * - Pull-ups: 2.2kΩ required on SDA/SCL
 * 
 * Slot Configuration:
 * - Slot 0: Device private key (ECC P-256, never readable)
 * - Slot 1: Server public key (ECC P-256, always readable)
 * - Slot 2: TLS session keys (AES-128, encrypted read)
 * - Slots 3-15: Available for application use
 * 
 * ⚠️ WARNING: Configuration lock is IRREVERSIBLE. Test thoroughly before locking!
 * 
 * Author: Smart Plug AI Development Team
 * Date: 2024-01-15
 * Version: 1.0
 */

#include "cryptoauthlib.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "ATECC608A";

// I2C configuration for ATECC608A
#define ATECC_I2C_ADDRESS   0xC0  // 8-bit address (0x60 << 1)
#define ATECC_I2C_BUS       0     // I2C bus number
#define ATECC_I2C_SPEED     100000 // 100kHz (can use 400000 for fast mode)

// Slot assignments
#define SLOT_DEVICE_PRIVKEY  0  // Device ECC private key (locked, never readable)
#define SLOT_SERVER_PUBKEY   1  // Server ECC public key (always readable)
#define SLOT_TLS_SESSION     2  // TLS session keys (AES, encrypted)

// Global state
static ATCAIfaceCfg g_atecc_cfg;
static bool g_atecc_initialized = false;

/**
 * Initialize ATECC608A device
 * 
 * Configures I2C interface and verifies device presence.
 * Must be called before any other ATECC608A operations.
 * 
 * @return ESP_OK on success, ESP_FAIL if device not found or initialization fails
 */
esp_err_t atecc608a_init(void) {
    ATCA_STATUS status;
    
    ESP_LOGI(TAG, "Initializing ATECC608A...");
    
    // Configure I2C interface
    g_atecc_cfg.iface_type = ATCA_I2C_IFACE;
    g_atecc_cfg.devtype = ATECC608A;
    g_atecc_cfg.atcai2c.slave_address = ATECC_I2C_ADDRESS;
    g_atecc_cfg.atcai2c.bus = ATECC_I2C_BUS;
    g_atecc_cfg.atcai2c.baud = ATECC_I2C_SPEED;
    g_atecc_cfg.wake_delay = 1500;  // Wake delay in microseconds
    g_atecc_cfg.rx_retries = 20;    // Number of retries for communication errors
    
    // Initialize CryptoAuthLib with this configuration
    status = atcab_init(&g_atecc_cfg);
    if (status != ATCA_SUCCESS) {
        ESP_LOGE(TAG, "ATECC608A init failed with status: 0x%02X", status);
        ESP_LOGE(TAG, "Check I2C connections and pull-up resistors!");
        return ESP_FAIL;
    }
    
    // Verify device is responding
    uint8_t serial_number[9];
    status = atcab_read_serial_number(serial_number);
    if (status != ATCA_SUCCESS) {
        ESP_LOGE(TAG, "Failed to read serial number: 0x%02X", status);
        return ESP_FAIL;
    }
    
    // Log device information
    ESP_LOGI(TAG, "ATECC608A detected successfully");
    ESP_LOGI(TAG, "Serial Number: %02X%02X%02X%02X%02X%02X%02X%02X%02X",
             serial_number[0], serial_number[1], serial_number[2],
             serial_number[3], serial_number[4], serial_number[5],
             serial_number[6], serial_number[7], serial_number[8]);
    
    // Check if device is configured and locked
    bool is_locked = false;
    status = atcab_is_locked(LOCK_ZONE_CONFIG, &is_locked);
    if (status == ATCA_SUCCESS) {
        ESP_LOGI(TAG, "Configuration zone: %s", is_locked ? "LOCKED" : "UNLOCKED");
    }
    
    status = atcab_is_locked(LOCK_ZONE_DATA, &is_locked);
    if (status == ATCA_SUCCESS) {
        ESP_LOGI(TAG, "Data zone: %s", is_locked ? "LOCKED" : "UNLOCKED");
    }
    
    g_atecc_initialized = true;
    return ESP_OK;
}

/**
 * Generate ECC key pair in specified slot
 * 
 * Generates a new ECC P-256 key pair. Private key is stored in the specified slot
 * and NEVER leaves the chip. Public key is returned.
 * 
 * ⚠️ WARNING: Can only be called BEFORE data zone is locked!
 * 
 * @param slot Slot number (0-15)
 * @param public_key Buffer to receive 64-byte public key (X and Y coordinates)
 * @return ESP_OK on success, ESP_FAIL on error
 */
esp_err_t atecc608a_generate_key_pair(uint8_t slot, uint8_t *public_key) {
    ATCA_STATUS status;
    
    if (!g_atecc_initialized) {
        ESP_LOGE(TAG, "ATECC608A not initialized!");
        return ESP_FAIL;
    }
    
    if (public_key == NULL) {
        ESP_LOGE(TAG, "Public key buffer is NULL!");
        return ESP_FAIL;
    }
    
    ESP_LOGI(TAG, "Generating ECC key pair in slot %d...", slot);
    
    // Generate key pair (private key stays in ATECC, public key returned)
    status = atcab_genkey(slot, public_key);
    if (status != ATCA_SUCCESS) {
        ESP_LOGE(TAG, "Key generation failed: 0x%02X", status);
        return ESP_FAIL;
    }
    
    ESP_LOGI(TAG, "Key pair generated successfully");
    ESP_LOG_BUFFER_HEX_LEVEL(TAG, public_key, 64, ESP_LOG_DEBUG);
    
    return ESP_OK;
}

/**
 * Sign data using private key in specified slot
 * 
 * Generates ECDSA signature using ECC P-256 private key stored in chip.
 * Private key never leaves the chip - signature is generated internally.
 * 
 * @param slot Slot number containing private key (0-15)
 * @param data Data to sign (will be SHA-256 hashed internally)
 * @param data_len Length of data in bytes
 * @param signature Buffer to receive 64-byte ECDSA signature (R and S values)
 * @return ESP_OK on success, ESP_FAIL on error
 */
esp_err_t atecc608a_sign_data(uint8_t slot, const uint8_t *data, size_t data_len, uint8_t *signature) {
    ATCA_STATUS status;
    uint8_t digest[32];  // SHA-256 digest (32 bytes)
    
    if (!g_atecc_initialized) {
        ESP_LOGE(TAG, "ATECC608A not initialized!");
        return ESP_FAIL;
    }
    
    if (data == NULL || signature == NULL) {
        ESP_LOGE(TAG, "NULL pointer passed to sign function!");
        return ESP_FAIL;
    }
    
    ESP_LOGI(TAG, "Signing %d bytes of data using slot %d...", data_len, slot);
    
    // Calculate SHA-256 hash of data
    status = atcab_sha(data_len, data, digest);
    if (status != ATCA_SUCCESS) {
        ESP_LOGE(TAG, "SHA-256 hashing failed: 0x%02X", status);
        return ESP_FAIL;
    }
    
    // Generate signature using private key in specified slot
    status = atcab_sign(slot, digest, signature);
    if (status != ATCA_SUCCESS) {
        ESP_LOGE(TAG, "Signature generation failed: 0x%02X", status);
        return ESP_FAIL;
    }
    
    ESP_LOGI(TAG, "Signature generated successfully");
    ESP_LOG_BUFFER_HEX_LEVEL(TAG, signature, 64, ESP_LOG_DEBUG);
    
    return ESP_OK;
}

/**
 * Verify ECDSA signature
 * 
 * Verifies an ECDSA signature using a public key.
 * Can use public key from a slot or externally provided public key.
 * 
 * @param data Original data that was signed
 * @param data_len Length of data in bytes
 * @param signature 64-byte ECDSA signature to verify
 * @param public_key 64-byte public key (NULL to use public key from slot)
 * @param slot Slot containing public key (ignored if public_key != NULL)
 * @return ESP_OK if signature is valid, ESP_FAIL if invalid or error
 */
esp_err_t atecc608a_verify_signature(const uint8_t *data, size_t data_len,
                                       const uint8_t *signature,
                                       const uint8_t *public_key, uint8_t slot) {
    ATCA_STATUS status;
    uint8_t digest[32];
    bool is_verified = false;
    
    if (!g_atecc_initialized) {
        ESP_LOGE(TAG, "ATECC608A not initialized!");
        return ESP_FAIL;
    }
    
    ESP_LOGI(TAG, "Verifying signature...");
    
    // Calculate SHA-256 hash of data
    status = atcab_sha(data_len, data, digest);
    if (status != ATCA_SUCCESS) {
        ESP_LOGE(TAG, "SHA-256 hashing failed: 0x%02X", status);
        return ESP_FAIL;
    }
    
    // Verify signature
    if (public_key != NULL) {
        // Verify using external public key
        status = atcab_verify_extern(digest, signature, public_key, &is_verified);
    } else {
        // Verify using public key stored in slot
        status = atcab_verify_stored(digest, signature, slot, &is_verified);
    }
    
    if (status != ATCA_SUCCESS) {
        ESP_LOGE(TAG, "Signature verification failed: 0x%02X", status);
        return ESP_FAIL;
    }
    
    if (is_verified) {
        ESP_LOGI(TAG, "✓ Signature is VALID");
        return ESP_OK;
    } else {
        ESP_LOGW(TAG, "✗ Signature is INVALID!");
        return ESP_FAIL;
    }
}

/**
 * Generate random number using hardware RNG
 * 
 * Uses ATECC608A's true random number generator.
 * More secure than ESP32's RNG for cryptographic purposes.
 * 
 * @param random_number Buffer to receive 32-byte random number
 * @return ESP_OK on success, ESP_FAIL on error
 */
esp_err_t atecc608a_get_random(uint8_t *random_number) {
    ATCA_STATUS status;
    
    if (!g_atecc_initialized) {
        ESP_LOGE(TAG, "ATECC608A not initialized!");
        return ESP_FAIL;
    }
    
    if (random_number == NULL) {
        ESP_LOGE(TAG, "Random number buffer is NULL!");
        return ESP_FAIL;
    }
    
    // Generate 32 bytes of true random data
    status = atcab_random(random_number);
    if (status != ATCA_SUCCESS) {
        ESP_LOGE(TAG, "Random number generation failed: 0x%02X", status);
        return ESP_FAIL;
    }
    
    ESP_LOGD(TAG, "Generated 32 bytes of random data");
    return ESP_OK;
}

/**
 * Read device configuration
 * 
 * Reads the 128-byte configuration zone.
 * Useful for verifying device configuration before locking.
 * 
 * @param config_data Buffer to receive 128-byte configuration
 * @return ESP_OK on success, ESP_FAIL on error
 */
esp_err_t atecc608a_read_config(uint8_t *config_data) {
    ATCA_STATUS status;
    
    if (!g_atecc_initialized) {
        ESP_LOGE(TAG, "ATECC608A not initialized!");
        return ESP_FAIL;
    }
    
    if (config_data == NULL) {
        ESP_LOGE(TAG, "Config data buffer is NULL!");
        return ESP_FAIL;
    }
    
    ESP_LOGI(TAG, "Reading configuration zone...");
    
    status = atcab_read_config_zone(config_data);
    if (status != ATCA_SUCCESS) {
        ESP_LOGE(TAG, "Configuration read failed: 0x%02X", status);
        return ESP_FAIL;
    }
    
    ESP_LOGI(TAG, "Configuration read successfully");
    ESP_LOG_BUFFER_HEX_LEVEL(TAG, config_data, 128, ESP_LOG_DEBUG);
    
    return ESP_OK;
}

/**
 * Perform self-test
 * 
 * Verifies ATECC608A functionality by:
 * 1. Reading serial number
 * 2. Generating random number
 * 3. Performing SHA-256 hash
 * 
 * @return ESP_OK if all tests pass, ESP_FAIL otherwise
 */
esp_err_t atecc608a_self_test(void) {
    esp_err_t ret = ESP_OK;
    ATCA_STATUS status;
    uint8_t serial_number[9];
    uint8_t random_data[32];
    uint8_t test_data[] = "Hello, ATECC608A!";
    uint8_t digest[32];
    
    ESP_LOGI(TAG, "Running ATECC608A self-test...");
    
    // Test 1: Read serial number
    status = atcab_read_serial_number(serial_number);
    if (status != ATCA_SUCCESS) {
        ESP_LOGE(TAG, "  ✗ Serial number read FAILED");
        ret = ESP_FAIL;
    } else {
        ESP_LOGI(TAG, "  ✓ Serial number read OK");
    }
    
    // Test 2: Generate random number
    status = atcab_random(random_data);
    if (status != ATCA_SUCCESS) {
        ESP_LOGE(TAG, "  ✗ Random number generation FAILED");
        ret = ESP_FAIL;
    } else {
        ESP_LOGI(TAG, "  ✓ Random number generation OK");
    }
    
    // Test 3: SHA-256 hash
    status = atcab_sha(sizeof(test_data), test_data, digest);
    if (status != ATCA_SUCCESS) {
        ESP_LOGE(TAG, "  ✗ SHA-256 hashing FAILED");
        ret = ESP_FAIL;
    } else {
        ESP_LOGI(TAG, "  ✓ SHA-256 hashing OK");
    }
    
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "ATECC608A self-test PASSED");
    } else {
        ESP_LOGE(TAG, "ATECC608A self-test FAILED");
    }
    
    return ret;
}

/**
 * Release ATECC608A resources
 * 
 * Cleans up and releases the ATECC608A device.
 * Call before application exit or when switching to different I2C configuration.
 */
void atecc608a_release(void) {
    if (g_atecc_initialized) {
        atcab_release();
        g_atecc_initialized = false;
        ESP_LOGI(TAG, "ATECC608A released");
    }
}

/*
 * Usage Example:
 * 
 * // Initialize
 * ESP_ERROR_CHECK(atecc608a_init());
 * ESP_ERROR_CHECK(atecc608a_self_test());
 * 
 * // Generate device key pair (do this once during provisioning)
 * uint8_t public_key[64];
 * ESP_ERROR_CHECK(atecc608a_generate_key_pair(SLOT_DEVICE_PRIVKEY, public_key));
 * 
 * // Sign some data
 * const char *message = "Important command: turn relay ON";
 * uint8_t signature[64];
 * ESP_ERROR_CHECK(atecc608a_sign_data(SLOT_DEVICE_PRIVKEY, 
 *                                      (uint8_t*)message, strlen(message),
 *                                      signature));
 * 
 * // Verify signature
 * ESP_ERROR_CHECK(atecc608a_verify_signature((uint8_t*)message, strlen(message),
 *                                             signature, public_key, 0));
 * 
 * // Clean up
 * atecc608a_release();
 */
