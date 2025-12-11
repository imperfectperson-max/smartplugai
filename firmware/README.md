# Smart Plug AI Firmware

ESP32-based firmware for real-time power monitoring, MQTT communication, and remote relay control.

## 🎯 Overview

The firmware is responsible for:

- **Sensor Reading**: Continuously monitor voltage, current, and power using INA219
- **MQTT Communication**: Publish telemetry data and subscribe to control commands
- **Relay Control**: Switch connected devices on/off via relay module
- **OTA Updates**: Support over-the-air firmware updates for remote maintenance
- **Wi-Fi Management**: Connect to Wi-Fi networks with fallback AP mode for configuration
- **Safety**: Implement watchdog timers and fail-safe mechanisms

## 🔌 Supported Hardware

### Required Components

- **ESP32 Development Board** (ESP32-DevKitC or compatible)
  - 30+ GPIO pins
  - Wi-Fi and Bluetooth
  - Dual-core processor (240 MHz)
  - 520 KB SRAM, 4+ MB Flash

- **INA219 Power Monitor Module**
  - Current sensing: 0-3.2A (±3.2A with bidirectional mode)
  - Voltage sensing: 0-26V
  - I2C interface (default address: 0x40)
  - 12-bit ADC resolution

- **Relay Module** (5V single channel with optocoupler isolation)
  - Rating: 10A @ 250VAC minimum
  - Control voltage: 5V DC
  - Normally Open (NO) and Common terminals

- **AC-DC Power Supply** (5V, 2A isolated supply)
  - HLK-PM01 or similar isolated module
  - Input: 100-240VAC
  - Output: 5V DC, 2A

### Alternative: SCT-013 Current Transformer

For non-invasive current sensing, you can use SCT-013 current transformers instead of INA219:

- **SCT-013-030**: 0-30A AC current sensor
- Requires burden resistor and voltage divider
- Connect to ESP32 ADC pin (GPIO 34-39)
- Suitable for monitoring only (no voltage sensing)

### Pinout Configuration

```cpp
// Default GPIO assignments for ESP32
#define I2C_SDA_PIN     21    // INA219 SDA
#define I2C_SCL_PIN     22    // INA219 SCL
#define RELAY_PIN       4     // Relay control
#define STATUS_LED_PIN  2     // Built-in LED (optional)
#define BUTTON_PIN      0     // Boot button for pairing/reset

// For SCT-013 alternative
#define SCT_PIN         34    // ADC input for current sensor
```

**Connection Diagram (Text-based)**:

```
ESP32 DevKit          INA219 Module          Relay Module
-----------          ---------------        --------------
3.3V/5V ───────────> VCC                    VCC <──── 5V
GND ────────────────> GND                    GND <──── GND
GPIO 21 ────────────> SDA
GPIO 22 ────────────> SCL
GPIO 4  ─────────────────────────────────> IN (Control)

Power Supply (HLK-PM01)
------------------------
AC Input (L, N, GND) ──> [Isolated Supply] ──> 5V DC Output
                                                  │
                                                  ├──> ESP32 VIN
                                                  └──> Relay VCC

INA219 Sensing (AC Side)
------------------------
AC Line ──> INA219 VIN+ ──> INA219 VIN- ──> Relay Common (C)
Relay NO ──> AC Output Socket
AC Neutral ──> AC Output Socket (Direct)
AC Ground ──> AC Output Socket (Direct)
```

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
   pio init --board esp32dev
   ```

3. **Install Dependencies** (add to `platformio.ini`):
   ```ini
   [env:esp32dev]
   platform = espressif32
   board = esp32dev
   framework = arduino
   lib_deps =
       adafruit/Adafruit INA219@^1.2.0
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

2. **Install ESP32 Board Support**:
   ```bash
   arduino-cli core update-index
   arduino-cli core install esp32:esp32
   ```

3. **Install Libraries**:
   ```bash
   arduino-cli lib install "Adafruit INA219"
   arduino-cli lib install "PubSubClient"
   arduino-cli lib install "ArduinoJson"
   ```

4. **Compile and Upload**:
   ```bash
   arduino-cli compile --fqbn esp32:esp32:esp32 firmware.ino
   arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32 firmware.ino
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

**Safety First**: Always prioritize electrical safety. If unsure about AC wiring, consult a licensed electrician.
