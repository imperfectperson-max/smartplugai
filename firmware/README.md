# Smart Plug AI Firmware

ESP32-based firmware for real-time power monitoring, MQTT communication, and remote relay control.

## 🎯 Overview

The firmware is responsible for:

- **Sensor Reading**: Continuously monitor current and power using SCT-013 non-invasive current transformer (recommended for Phase 1 prototypes)
- **MQTT Communication**: Publish encrypted telemetry data and subscribe to signed control commands over TLS 1.3
- **Relay Control**: Switch connected devices on/off via relay module with signed command verification
- **OTA Updates**: Support secure over-the-air firmware updates with signature verification
- **Wi-Fi Management**: Connect to Wi-Fi networks with fallback AP mode for secure provisioning
- **Security**: Implement secure boot, flash encryption, ATECC608A cryptographic operations, and tamper detection
- **Safety**: Implement watchdog timers and fail-safe mechanisms

## 🔌 Supported Hardware

### Required Components (Recommended Secure Stack)

- **ESP32-S3 Development Board** (ESP32-S3-WROOM-1-N8 or compatible)
  - Secure boot and flash encryption support
  - Hardware cryptographic acceleration (RSA, AES, SHA, ECC)
  - 30+ GPIO pins
  - Wi-Fi 802.11 b/g/n and Bluetooth 5 (LE)
  - Dual-core Xtensa LX7 processor (up to 240 MHz)
  - 512 KB SRAM, 8 MB Flash (N8 variant)
  - Note: ESP32-S3 provides enhanced security features over ESP32

- **ATECC608A Secure Element**
  - Hardware-based cryptographic key storage
  - ECDSA signature generation and verification
  - Secure key generation and storage (private keys never leave chip)
  - I2C interface (address: 0x60 typical)
  - Used for device attestation and signed command verification

- **24C256 Secure EEPROM** (32KB)
  - Secure configuration and credential storage
  - I2C interface
  - Tamper-resistant data storage

- **MAX6316 Tamper Detection Watchdog**
  - Monitors for physical tampering attempts
  - Triggers secure erase on tamper detection
  - Battery-backed tamper detection

- **SCT-013 Current Transformer** (Recommended for Phase 1)
  - **SCT-013-030**: 0-30A AC current sensor (non-invasive)
  - Requires burden resistor (typically 100-200Ω) and voltage divider
  - Connect to ESP32-S3 ADC pin (GPIO 1-10)
  - Suitable for current and power monitoring (voltage estimated from mains standard)
  - **Advantages**: Non-invasive, safer installation, no direct AC contact
  - **Note**: For prototypes, this is the recommended approach over invasive sensing

- **INA219 Power Monitor Module** (Legacy/Optional)
  - Current sensing: 0-3.2A (±3.2A with bidirectional mode)
  - Voltage sensing: 0-26V
  - I2C interface (default address: 0x40)
  - 12-bit ADC resolution
  - **Note**: Requires invasive installation on AC line - use only if necessary and with proper safety measures
  - **Status**: Legacy option, SCT-013 preferred for security and safety

- **Relay Module** (5V single channel with optocoupler isolation)
  - Rating: 10A @ 250VAC minimum
  - Control voltage: 5V DC
  - Normally Open (NO) and Common terminals

- **Power Supply** (Certified 5V, 2A for Phase 1 Prototypes)
  - **Recommended**: UL/CE certified 5V/2A USB phone charger
  - Input: 100-240VAC
  - Output: 5V DC, 2A minimum
  - **Note**: For Phase 1 prototypes, use certified phone charger instead of HLK-PM01
  - **Future**: HLK-PM01 or similar isolated module for production with proper certifications

### Pinout Configuration

```cpp
// Default GPIO assignments for ESP32-S3
#define I2C_SDA_PIN     8     // I2C SDA for ATECC608A and 24C256
#define I2C_SCL_PIN     9     // I2C SCL for ATECC608A and 24C256
#define RELAY_PIN       4     // Relay control
#define STATUS_LED_PIN  2     // Built-in LED (optional)
#define BUTTON_PIN      0     // Boot button for pairing/reset
#define TAMPER_PIN      5     // MAX6316 tamper detection input

// For SCT-013 current sensor (recommended)
#define SCT_PIN         1     // ADC input for current sensor (ADC1_CH0)

// For INA219 (legacy/optional)
#define I2C_SDA_PIN_ALT 21    // INA219 SDA (if using INA219 instead)
#define I2C_SCL_PIN_ALT 22    // INA219 SCL (if using INA219 instead)
```

**Connection Diagram (Text-based)**:

```
ESP32-S3 DevKit        ATECC608A          SCT-013 Sensor       Relay Module
---------------        ----------         ---------------      --------------
3.3V ──────────────> VCC                                      VCC <──── 5V
GND ───────────────> GND                  GND (burden res)    GND <──── GND
GPIO 8 ────────────> SDA                                      
GPIO 9 ────────────> SCL                                      
GPIO 1 ──────────────────────────────> Signal (via burden)
GPIO 4 ───────────────────────────────────────────────────> IN (Control)
GPIO 5 ──────────> Tamper Detect (MAX6316)

Power Supply (Certified 5V/2A Charger)
---------------------------------------
USB-C/Micro USB Input ──> 5V DC Output
                            │
                            ├──> ESP32-S3 5V
                            └──> Relay VCC

SCT-013 Installation (Non-Invasive)
-----------------------------------
AC Line ──> [ Clamp around wire ] ──> Signal Output ──> Burden Resistor + Divider ──> ESP32-S3 GPIO 1
Relay Common (C) ──> AC Output Socket
Relay NO ──> AC Line (switched)
AC Neutral ──> AC Output Socket (Direct)
AC Ground ──> AC Output Socket (Direct)

Security Components
-------------------
24C256 EEPROM ──> I2C (same bus as ATECC608A)
MAX6316 Watchdog ──> Tamper detect to GPIO 5
Fuse (2A) + MOV ──> AC line protection
Tamper-evident seal ──> Physical enclosure
```

## 🔒 Secure Provisioning & Setup

### Overview

This project implements a comprehensive security architecture with multiple layers of protection:

1. **Secure Boot**: Ensures only signed firmware can run on the device
2. **Flash Encryption**: Encrypts firmware and data stored in flash memory
3. **ATECC608A Provisioning**: Securely stores cryptographic keys in hardware
4. **MQTT over TLS 1.3**: Encrypted communication with client certificate authentication
5. **Tamper Detection**: Physical security monitoring

### Step 1: Enable Secure Boot (ESP32-S3)

Secure boot prevents unauthorized firmware from running on the device.

```bash
# Using ESP-IDF
cd $IDF_PATH
. ./export.sh

# Configure secure boot
idf.py menuconfig
# Navigate to: Security features -> Enable hardware Secure Boot
# Select: Secure Boot V2 (RSA-PSS with SHA-256)

# Generate secure boot signing key
espsecure.py generate_signing_key secure_boot_signing_key.pem

# Build with secure boot enabled
idf.py build
```

### Step 2: Enable Flash Encryption

Flash encryption protects firmware and data stored in flash memory.

```bash
# Enable in menuconfig
idf.py menuconfig
# Navigate to: Security features -> Enable flash encryption on boot

# Flash encryption will be automatically enabled on first boot
# The encryption key is generated and stored in eFuse (one-time programmable)
```

### Step 3: Provision ATECC608A Secure Element

The ATECC608A stores cryptographic keys that never leave the chip.

```bash
# Install ATECC provisioning tools
pip install cryptoauthlib

# Generate device certificate and keys
python provision_atecc608a.py --device-id smartplug_001

# This script will:
# - Generate ECC private key in slot 0 (locked, never readable)
# - Store device certificate
# - Configure slot access policies
# - Lock configuration (irreversible)
```

**Provisioning Script Example** (see `tools/provision_atecc608a.py`):
```python
from cryptoauthlib import *

def provision_device(device_id):
    # Initialize ATECC608A
    atca_init()
    
    # Generate ECC key pair in slot 0
    atca_genkey(GENKEY_MODE_PRIVATE, 0)
    
    # Get public key for certificate generation
    public_key = atca_getpubkey(0)
    
    # Generate and store device certificate
    # (Certificate includes device_id, public key, signed by CA)
    cert = generate_device_cert(device_id, public_key)
    atca_write_cert(cert)
    
    # Lock configuration (irreversible!)
    atca_lock_config()
    atca_lock_data()
```

### Step 4: Configure MQTT over TLS 1.3

All device communication uses encrypted MQTT over TLS 1.3 with client certificates.

**MQTT Configuration**:
```cpp
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

WiFiClientSecure secureClient;
PubSubClient mqttClient(secureClient);

void setupMQTTwithTLS() {
  // Load CA certificate (server verification)
  secureClient.setCACert(ca_cert_pem);
  
  // Load client certificate from ATECC608A
  secureClient.setCertificate(device_cert_pem);
  
  // Use ATECC608A for TLS handshake (private key never leaves chip)
  secureClient.setPrivateKey(NULL);  // Key is in ATECC608A
  
  // Connect to MQTT broker with TLS
  mqttClient.setServer(MQTT_BROKER, 8883);  // Port 8883 for TLS
  mqttClient.connect(DEVICE_ID, MQTT_USERNAME, MQTT_PASSWORD);
}
```

### Step 5: Implement Signed Commands

Control commands are signed using ECDSA to prevent unauthorized device control.

**Command Verification**:
```cpp
#include <ATECC608A.h>

bool verifyCommandSignature(const char* command, const byte* signature) {
  // Hash the command
  byte hash[32];
  SHA256(command, strlen(command), hash);
  
  // Verify signature using ATECC608A
  // Public key of command server is stored in ATECC slot 1
  bool valid = ATECC608A.verify(hash, signature, 1);
  
  if (!valid) {
    Serial.println("Invalid command signature - rejected!");
  }
  
  return valid;
}

void handleControlCommand(const char* payload) {
  // Parse JSON: {"command": "relay_on", "signature": "base64..."}
  String command = extractCommand(payload);
  byte signature[64];
  extractSignature(payload, signature);
  
  // Verify before executing
  if (verifyCommandSignature(command.c_str(), signature)) {
    executeCommand(command);
  }
}
```

### Security Best Practices

- **Never hardcode credentials**: Use secure storage (ATECC608A, NVS encryption)
- **Rotate credentials regularly**: Implement certificate renewal mechanism
- **Monitor tamper detection**: React to MAX6316 tamper alerts
- **Implement secure OTA**: Verify firmware signatures before flashing
- **Use TLS 1.3 exclusively**: Disable older TLS versions
- **Rate limit control commands**: Prevent command flooding attacks
- **Audit logging**: Log all security-relevant events

For detailed security architecture, see [docs/SECURITY.md](../docs/SECURITY.md).

## 🚀 Quick Start

### Prerequisites

- **PlatformIO** (recommended) or **Arduino CLI**
- USB cable for programming ESP32
- Visual Studio Code with PlatformIO extension (optional but recommended)

### Option 1: Using PlatformIO

1. **Install PlatformIO**:
   ```bash
   # Via VS Code extension or
   pip install platformio
   ```

2. **Initialize Project**:
   ```bash
   cd firmware
   pio init --board esp32-s3-devkitc-1
   ```

3. **Install Dependencies** (add to `platformio.ini`):
   ```ini
   [env:esp32-s3-devkitc-1]
   platform = espressif32
   board = esp32-s3-devkitc-1
   framework = arduino
   lib_deps =
       sparkfun/SparkFun ATECCX08a Arduino Library@^2.0.0
       knolleary/PubSubClient@^2.8
       bblanchon/ArduinoJson@^6.21.0
   monitor_speed = 115200
   ```

4. **Build and Upload**:
   ```bash
   pio run --target upload
   pio device monitor  # View serial output
   ```

### Option 2: Using Arduino CLI

1. **Install Arduino CLI**:
   ```bash
   curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
   ```

2. **Install ESP32-S3 Board Support**:
   ```bash
   arduino-cli core update-index
   arduino-cli core install esp32:esp32
   ```

3. **Install Libraries**:
   ```bash
   arduino-cli lib install "SparkFun ATECCX08a Arduino Library"
   arduino-cli lib install "PubSubClient"
   arduino-cli lib install "ArduinoJson"
   ```

4. **Compile and Upload**:
   ```bash
   arduino-cli compile --fqbn esp32:esp32:esp32s3 firmware.ino
   arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32s3 firmware.ino
   ```

## 📝 Example Sketch

Here's a complete minimal example that demonstrates INA219 reading, MQTT publish, and relay control:

```cpp
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <ArduinoJson.h>

// ⚠️ WARNING: Never hardcode credentials in production firmware!
// Use Wi-Fi manager (e.g., WiFiManager library) and secure credential storage
// This example uses placeholders for demonstration only
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// MQTT broker configuration
const char* MQTT_BROKER = "mqtt.example.com";
const int MQTT_PORT = 1883;
const char* MQTT_USERNAME = "YOUR_MQTT_USERNAME";
const char* MQTT_PASSWORD = "YOUR_MQTT_PASSWORD";
const char* DEVICE_ID = "smartplug_001";  // Unique device identifier

// GPIO pins
#define RELAY_PIN 4
#define STATUS_LED_PIN 2

// MQTT topics
String telemetryTopic = "smartplug/" + String(DEVICE_ID) + "/telemetry";
String controlTopic = "smartplug/" + String(DEVICE_ID) + "/control";
String statusTopic = "smartplug/" + String(DEVICE_ID) + "/status";

// Objects
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
Adafruit_INA219 ina219;

// Global variables
bool relayState = false;
unsigned long lastTelemetryTime = 0;
const unsigned long TELEMETRY_INTERVAL = 1000;  // 1 second

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(STATUS_LED_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  
  // Initialize I2C and INA219
  Wire.begin(21, 22);  // SDA, SCL
  if (!ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  
  // Optional: Configure INA219 for higher resolution
  ina219.setCalibration_16V_400mA();  // Adjust based on expected load
  
  // Connect to Wi-Fi
  connectWiFi();
  
  // Configure MQTT
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);
  
  Serial.println("Smart Plug initialized successfully");
}

void loop() {
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();
  
  // Publish telemetry data at regular intervals
  if (millis() - lastTelemetryTime >= TELEMETRY_INTERVAL) {
    publishTelemetry();
    lastTelemetryTime = millis();
  }
}

void connectWiFi() {
  Serial.print("Connecting to Wi-Fi: ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWi-Fi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Connecting to MQTT broker...");
    
    if (mqttClient.connect(DEVICE_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("connected");
      mqttClient.subscribe(controlTopic.c_str());
      publishStatus("online");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.println(topic);
  
  // Parse JSON payload
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, payload, length);
  
  if (error) {
    Serial.print("JSON parsing failed: ");
    Serial.println(error.c_str());
    return;
  }
  
  // Handle relay control command
  if (String(topic) == controlTopic) {
    const char* command = doc["command"];
    
    if (strcmp(command, "relay_on") == 0) {
      setRelay(true);
    } else if (strcmp(command, "relay_off") == 0) {
      setRelay(false);
    } else if (strcmp(command, "relay_toggle") == 0) {
      setRelay(!relayState);
    }
  }
}

void setRelay(bool state) {
  relayState = state;
  digitalWrite(RELAY_PIN, state ? HIGH : LOW);
  digitalWrite(STATUS_LED_PIN, state ? HIGH : LOW);
  
  Serial.print("Relay ");
  Serial.println(state ? "ON" : "OFF");
  
  publishStatus(state ? "on" : "off");
}

void publishTelemetry() {
  float shuntVoltage = ina219.getShuntVoltage_mV();
  float busVoltage = ina219.getBusVoltage_V();
  float current = ina219.getCurrent_mA();
  float power = ina219.getPower_mW();
  float loadVoltage = busVoltage + (shuntVoltage / 1000);
  
  // Create JSON payload
  StaticJsonDocument<256> doc;
  doc["device_id"] = DEVICE_ID;
  doc["timestamp"] = millis();
  doc["voltage"] = loadVoltage;
  doc["current"] = current / 1000.0;  // Convert to Amps
  doc["power"] = power / 1000.0;      // Convert to Watts
  doc["relay_state"] = relayState;
  
  char buffer[256];
  serializeJson(doc, buffer);
  
  mqttClient.publish(telemetryTopic.c_str(), buffer);
  
  Serial.print("Telemetry: V=");
  Serial.print(loadVoltage);
  Serial.print("V, I=");
  Serial.print(current / 1000.0);
  Serial.print("A, P=");
  Serial.print(power / 1000.0);
  Serial.println("W");
}

void publishStatus(const char* status) {
  StaticJsonDocument<128> doc;
  doc["device_id"] = DEVICE_ID;
  doc["status"] = status;
  doc["timestamp"] = millis();
  
  char buffer[128];
  serializeJson(doc, buffer);
  
  mqttClient.publish(statusTopic.c_str(), buffer, true);  // Retained message
}
```

## 📡 MQTT Topics and Message Formats

### Telemetry Topic (Device → Cloud)

**Topic**: `smartplug/{device_id}/telemetry`

**Payload** (JSON):

```json
{
  "device_id": "smartplug_001",
  "timestamp": 1234567890,
  "voltage": 120.5,
  "current": 0.85,
  "power": 102.4,
  "relay_state": true
}
```

**Frequency**: Published every 1 second (configurable)

### Control Topic (Cloud → Device)

**Topic**: `smartplug/{device_id}/control`

**Payload** (JSON):

```json
{
  "command": "relay_on"
}
```

**Supported Commands**:

- `relay_on`: Turn relay ON
- `relay_off`: Turn relay OFF
- `relay_toggle`: Toggle relay state
- `update_interval`: Change telemetry interval (include `interval_ms` field)

**Example with interval update**:

```json
{
  "command": "update_interval",
  "interval_ms": 5000
}
```

### Status Topic (Device → Cloud)

**Topic**: `smartplug/{device_id}/status`

**Payload** (JSON):

```json
{
  "device_id": "smartplug_001",
  "status": "online",
  "timestamp": 1234567890
}
```

**Status Values**: `online`, `offline`, `on`, `off`, `error`

**Note**: This topic uses MQTT retained messages for Last Will and Testament (LWT).

### Configuration Topic (Cloud → Device)

**Topic**: `smartplug/{device_id}/config`

**Payload** (JSON):

```json
{
  "telemetry_interval": 1000,
  "calibration": {
    "voltage_offset": 0.0,
    "current_offset": 0.0
  }
}
```

## 🔄 OTA (Over-The-Air) Updates

### Method 1: ArduinoOTA (Recommended)

Add OTA support to your firmware:

```cpp
#include <ArduinoOTA.h>

void setupOTA() {
  ArduinoOTA.setHostname(DEVICE_ID);
  ArduinoOTA.setPassword("secure_ota_password");
  
  ArduinoOTA.onStart([]() {
    String type = (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem";
    Serial.println("Start updating " + type);
  });
  
  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA Update Complete");
  });
  
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
  });
  
  ArduinoOTA.begin();
}

// Add to setup()
setupOTA();

// Add to loop()
ArduinoOTA.handle();
```

**Upload via OTA**:

```bash
# Using PlatformIO
pio run --target upload --upload-port smartplug_001.local

# Using Arduino IDE: Tools > Port > Network Ports > smartplug_001.local
```

### Method 2: HTTP OTA Update

Implement HTTP-based OTA for remote updates:

```cpp
#include <HTTPUpdate.h>

void checkForOTAUpdate() {
  HTTPClient http;
  http.begin("https://api.example.com/firmware/latest");
  int httpCode = http.GET();
  
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    // Parse version, download URL from payload
    // Use HTTPUpdate.update() to download and flash
  }
  
  http.end();
}
```

### OTA Safety Considerations

- Always verify firmware signature before flashing
- Implement rollback mechanism for failed updates
- Keep bootloader and partition table intact
- Test OTA updates thoroughly before deployment
- Provide fallback to serial upload if OTA fails

## 🛡️ Safety Notes

### Electrical Safety (CRITICAL!)

- **⚠️ DANGER**: Working with mains voltage (110-240VAC) can be LETHAL
- **Never** work on AC circuits while powered
- Use proper isolation between AC and DC circuits
- Ensure power supply is certified and isolated (UL/CE)
- Add fuse on AC line (2A slow-blow recommended)
- Use appropriate wire gauge (16-18 AWG for AC)
- Verify proper grounding of AC ground connection
- Test for shorts between AC and DC sides (>1MΩ resistance)
- Use proper enclosure rated for mains voltage
- Follow local electrical codes and regulations

### Firmware Safety

- Implement watchdog timer for automatic recovery
- Add brownout detection and handling
- Limit relay switching frequency to prevent damage
- Add thermal monitoring if using high-power loads
- Implement fail-safe: relay OFF on boot/reset
- Store critical settings in non-volatile memory (EEPROM/SPIFFS)

### Data Safety

- Use TLS/SSL for MQTT in production (port 8883)
- Implement authentication for OTA updates
- Never hardcode credentials in firmware
- Use secure storage for Wi-Fi credentials (ESP32 preferences library)
- Implement device provisioning with unique keys per device

## 🐛 Troubleshooting

### INA219 Not Detected

**Symptoms**: I2C scan doesn't find device at 0x40

**Solutions**:

- Verify I2C wiring (SDA/SCL not swapped)
- Check power supply to INA219 (3.3V or 5V)
- Add 4.7kΩ pull-up resistors on SDA/SCL if needed
- Try I2C scanner sketch to find actual address
- Check for solder bridges or cold joints

```cpp
// I2C Scanner sketch
void scanI2C() {
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found device at 0x");
      Serial.println(addr, HEX);
    }
  }
}
```

### Inaccurate Power Readings

**Symptoms**: Readings don't match known loads

**Solutions**:

- Calibrate INA219 with known loads
- Adjust shunt resistor value in code
- Use appropriate calibration function for expected range
- Verify proper current path through INA219 shunt
- Check for voltage drop in connections

```cpp
// Different calibration options
ina219.setCalibration_32V_2A();   // For loads up to 2A
ina219.setCalibration_32V_1A();   // For higher resolution at lower currents
ina219.setCalibration_16V_400mA(); // For low-power devices
```

### MQTT Connection Issues

**Symptoms**: Cannot connect to MQTT broker

**Solutions**:

- Verify broker address and port (1883 for non-TLS, 8883 for TLS)
- Check username/password
- Ensure broker allows remote connections
- Check firewall rules
- Verify network connectivity (ping broker)
- Enable MQTT debug logging

```cpp
// Enable debug output
mqttClient.setCallback(mqttCallback);
Serial.print("MQTT state: ");
Serial.println(mqttClient.state());
// -4 = timeout, -3 = connection lost, -2 = connect failed
// -1 = disconnected, 0 = connected
```

### ESP32 Brownouts/Resets

**Symptoms**: Random resets, "Brownout detector" messages

**Solutions**:

- Use adequate power supply (2A minimum)
- Add bulk capacitor near ESP32 (100-470μF)
- Reduce Wi-Fi transmit power if needed
- Check for voltage drops during relay switching
- Use separate power for relay coil if needed

```cpp
// Reduce Wi-Fi power
WiFi.setTxPower(WIFI_POWER_11dBm);  // Reduce from default 20dBm
```

### Relay Not Switching

**Symptoms**: Relay doesn't click or switch

**Solutions**:

- Verify GPIO output voltage (should be 3.3V high)
- Check relay control voltage requirement (may need 5V)
- Use transistor driver if relay requires more current
- Test relay independently with direct power
- Check for damaged relay coil

```cpp
// Test relay directly
pinMode(RELAY_PIN, OUTPUT);
digitalWrite(RELAY_PIN, HIGH);
delay(1000);
digitalWrite(RELAY_PIN, LOW);
```

## 📚 Additional Resources

- **Soldering Guide**: See [docs/soldering-checklist.md](../docs/soldering-checklist.md) for hardware assembly
- **INA219 Datasheet**: [Texas Instruments INA219](https://www.ti.com/lit/ds/symlink/ina219.pdf)
- **ESP32 Documentation**: [Espressif ESP32 Docs](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
- **PlatformIO Guide**: [PlatformIO ESP32](https://docs.platformio.org/en/latest/boards/espressif32/esp32dev.html)
- **MQTT Protocol**: [MQTT.org](https://mqtt.org/)
- **ArduinoOTA**: [Arduino OTA Updates](https://arduino-esp8266.readthedocs.io/en/latest/ota_updates/readme.html)

## 🤝 Contributing

Firmware contributions are welcome! Please:

- Test thoroughly with real hardware before submitting
- Follow Arduino/ESP32 coding conventions
- Add comments for complex logic
- Document any new MQTT topics or commands
- Update this README with new features

See [CONTRIBUTING.md](../CONTRIBUTING.md) for general guidelines.

---

**Security First**: Always prioritize electrical safety and security best practices. If unsure about AC wiring, consult a licensed electrician. For detailed security architecture and provisioning workflows, see [docs/SECURITY.md](../docs/SECURITY.md) and [docs/ROADMAP.md](../docs/ROADMAP.md).
