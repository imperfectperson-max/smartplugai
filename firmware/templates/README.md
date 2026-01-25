# Firmware Code Templates

This directory contains reusable code templates and stubs for the Smart Plug AI firmware development. These templates implement common functionality and serve as starting points for firmware components.

## 📁 Available Templates

### 1. I2C Bus Scanner (`i2c_scanner.ino`)
**Purpose**: Scan I2C bus for connected devices and verify hardware connections.

**Features**:
- Automatic scanning of I2C addresses 0x01-0x7F
- Device identification for known components
- Verification of expected Smart Plug AI devices (ATECC608A, 24C256)
- Diagnostic output for troubleshooting

**When to Use**:
- Initial hardware testing
- Debugging I2C communication issues
- Verifying device addresses
- Hardware assembly verification

**Usage**:
```bash
# Upload to ESP32-S3
pio run --target upload
# Or with Arduino IDE: Sketch > Upload
```

---

### 2. GPIO Initialization (`gpio_initialization.c`)
**Purpose**: Standard GPIO configuration for all digital I/O in the project.

**Features**:
- Safe initialization sequence (outputs before inputs)
- Relay defaults to OFF (safe state)
- Interrupt configuration for tamper detection and button input
- Self-test function for hardware verification
- Helper functions for common GPIO operations

**Pin Assignments**:
- GPIO 4: ADC input for current sensor
- GPIO 8: I2C SDA
- GPIO 9: I2C SCL
- GPIO 10: Tamper detection
- GPIO 11: Watchdog toggle
- GPIO 18: Relay control
- GPIO 2: Status LED
- GPIO 0: Boot button

**Integration**:
```c
#include "gpio_initialization.h"

void app_main(void) {
    ESP_ERROR_CHECK(gpio_init_all());
    ESP_ERROR_CHECK(gpio_self_test());
    ESP_ERROR_CHECK(gpio_configure_tamper_interrupt(tamper_isr_handler));
    // ... rest of initialization
}
```

---

### 3. ATECC608A Driver (`atecc608a_driver.c`)
**Purpose**: High-level interface to Microchip ATECC608A secure element.

**Features**:
- Device initialization and configuration verification
- ECC P-256 key pair generation
- ECDSA signature generation and verification
- Hardware random number generation
- Secure key storage (private keys never leave chip)
- Self-test function

**Security Considerations**:
- ⚠️ **Configuration lock is IRREVERSIBLE** - test thoroughly first!
- Private keys stored in slot 0 are NEVER readable
- Always use spare chip for testing before production
- Back up all configuration before locking

**Slot Configuration**:
- Slot 0: Device private key (locked, never readable)
- Slot 1: Server public key (always readable)
- Slot 2: TLS session keys (encrypted read)
- Slots 3-15: Available for application use

**Example Usage**:
```c
// Initialize
ESP_ERROR_CHECK(atecc608a_init());
ESP_ERROR_CHECK(atecc608a_self_test());

// Generate device key pair (provisioning step)
uint8_t public_key[64];
ESP_ERROR_CHECK(atecc608a_generate_key_pair(SLOT_DEVICE_PRIVKEY, public_key));

// Sign data
uint8_t signature[64];
const char *message = "Turn relay ON";
ESP_ERROR_CHECK(atecc608a_sign_data(0, (uint8_t*)message, strlen(message), signature));

// Verify signature
ESP_ERROR_CHECK(atecc608a_verify_signature((uint8_t*)message, strlen(message),
                                            signature, public_key, 0));
```

---

### 4. Current Sensor Driver (`current_sensor_driver.c`)
**Purpose**: High-accuracy AC current measurement with True RMS calculation.

**Features**:
- True RMS calculation over complete AC cycles
- Automatic zero-offset calibration
- Known-load calibration procedure
- Temperature compensation
- Multi-point calibration for linearity correction
- Calibration data persistence

**Hardware Requirements**:
- SCT-013-030 current transformer (30A/1V)
- 33Ω ±1% burden resistor (1W)
- 100nF noise filter capacitor
- 1N4007 protection diode

**Calibration Procedure**:
```c
// Step 1: Initialize
ESP_ERROR_CHECK(current_sensor_init());

// Step 2: Zero offset (NO LOAD!)
ESP_ERROR_CHECK(current_sensor_calibrate_zero());

// Step 3: Known load (60W bulb = 0.261A @ 230V)
ESP_ERROR_CHECK(current_sensor_calibrate_with_load(0.261));

// Step 4: Save calibration
ESP_ERROR_CHECK(current_sensor_save_calibration());
```

**Measurement**:
```c
float current = current_sensor_read_rms();
printf("Current: %.3f A\n", current);
```

**Accuracy**: ±2% over 0.1A - 30A range (after calibration)

---

### 5. Watchdog Driver (`watchdog_driver.c`)
**Purpose**: Physical security through watchdog timer and tamper detection.

**Features**:
- Periodic watchdog toggle (500ms interval)
- Tamper detection via interrupt (<100ms response time)
- Automatic sensitive data wipe on tamper
- Audit logging of security events
- System reset trigger on security breach

**Security Response Sequence**:
1. Detect tamper (switch opening)
2. Log event with timestamp
3. Wipe sensitive data (credentials, keys)
4. Stop watchdog toggle
5. System reset in 1.6s
6. Report tamper to backend on reboot

**Hardware**:
- TPS3823-33 watchdog IC (3.3V, 1.6s timeout)
- Normally closed tamper switch
- Connected to ESP32 EN pin via 10kΩ pull-up

**Integration**:
```c
// Initialize
ESP_ERROR_CHECK(nvs_flash_init());
ESP_ERROR_CHECK(watchdog_init());

// Start tasks
ESP_ERROR_CHECK(watchdog_start_tasks());

// Check tamper status
if (watchdog_is_tampered()) {
    ESP_LOGW(TAG, "Device was tampered!");
}
```

**⚠️ Critical Notes**:
- Watchdog task runs at highest priority
- Do NOT modify tamper response without security review
- Tamper flag persists across resets
- Only authorized personnel can clear tamper flag

---

## 🔧 Integration Guidelines

### 1. Minimal Changes Required

Most templates are designed to work with minimal modification:
- Update GPIO pin numbers if needed (in main CMakeLists.txt or platformio.ini)
- Adjust I2C addresses if using different configuration
- Modify calibration constants for specific hardware
- Add application-specific functionality on top of templates

### 2. Dependencies

**ESP-IDF Components**:
- `driver` - GPIO, ADC, I2C drivers
- `esp_adc_cal` - ADC calibration
- `nvs_flash` - Non-volatile storage
- `freertos` - RTOS tasks and synchronization
- `esp_log` - Logging framework

**External Libraries**:
- `cryptoauthlib` - For ATECC608A (install via component manager)

### 3. Build System

**PlatformIO** (recommended):
```ini
[env:esp32-s3-devkitc-1]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
lib_deps =
    sparkfun/SparkFun ATECCX08a Arduino Library@^2.0.0
```

**ESP-IDF**:
```cmake
idf_component_register(
    SRCS "atecc608a_driver.c" 
         "current_sensor_driver.c"
         "gpio_initialization.c"
         "watchdog_driver.c"
    INCLUDE_DIRS "."
    REQUIRES driver esp_adc_cal nvs_flash freertos
)
```

---

## 🧪 Testing Each Template

### I2C Scanner
```bash
# Expected output:
# 0x50  24C256 EEPROM        FOUND
# 0x60  ATECC608A Crypto     FOUND
```

### GPIO Initialization
```c
// Self-test should print:
// ✓ Relay in safe OFF state
// ✓ Button pull-up working
// ✓ Tamper switch closed
// ✓ Status LED toggle working
// ✓ Watchdog toggle working
```

### ATECC608A Driver
```c
// Self-test output:
// ✓ Serial number read OK
// ✓ Random number generation OK
// ✓ SHA-256 hashing OK
```

### Current Sensor
```c
// Expected readings:
// No load: 0.00A ±0.05A
// 60W bulb: 0.26A ±2% (after calibration)
// 100W bulb: 0.43A ±2% (after calibration)
```

### Watchdog
```c
// Normal operation:
// Watchdog pin toggles every 500ms
// 
// Tamper test:
// Open enclosure → tamper interrupt → data wipe → system reset
```

---

## 📝 Customization Points

### GPIO Pins
Modify pin assignments in `gpio_initialization.c`:
```c
#define RELAY_PIN           GPIO_NUM_18
#define STATUS_LED_PIN      GPIO_NUM_2
// ... etc
```

### I2C Configuration
Update I2C settings in relevant templates:
```c
#define I2C_SDA_PIN 8
#define I2C_SCL_PIN 9
#define I2C_FREQ 100000  // 100kHz standard mode
```

### Calibration Constants
Adjust in `current_sensor_driver.c`:
```c
#define BURDEN_RESISTOR_OHMS  33.0
#define CT_TURNS_RATIO        1800
#define LINE_FREQUENCY        50.0  // or 60.0 for US
```

### Security Parameters
Modify in `watchdog_driver.c` (⚠️ requires security review):
```c
#define WATCHDOG_TOGGLE_INTERVAL_MS  500
#define WATCHDOG_TIMEOUT_MS          1600
```

---

## ⚠️ Important Warnings

### ATECC608A
- **CONFIGURATION LOCK IS IRREVERSIBLE!**
- Always test on spare chips before production
- Back up all keys and configuration
- Private keys CANNOT be exported after generation

### High Voltage Safety
- NEVER work on AC circuits while powered
- Use isolation transformer for testing
- Verify isolation (>10MΩ) before AC connection
- Follow proper electrical codes and regulations

### Watchdog System
- Do NOT disable watchdog in production
- Do NOT modify tamper response without security review
- Ensure watchdog task runs at high priority
- Test tamper detection thoroughly

---

## 📚 Additional Resources

- **ESP-IDF Documentation**: https://docs.espressif.com/projects/esp-idf/
- **ATECC608A Datasheet**: https://www.microchip.com/ATECC608A
- **SCT-013 Reference**: http://openenergymonitor.org/emon/
- **TPS3823 Datasheet**: https://www.ti.com/product/TPS3823

---

## 🤝 Contributing

When modifying templates:
1. Test thoroughly on hardware
2. Update documentation and comments
3. Verify backward compatibility
4. Add example usage
5. Update this README

---

## 📄 License

These templates are part of the Smart Plug AI project and follow the same MIT license.

---

**Last Updated**: 2024-01-15  
**Version**: 1.0  
**Maintainer**: Smart Plug AI Development Team
