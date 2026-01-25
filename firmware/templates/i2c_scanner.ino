/*
 * I2C Bus Scanner Template
 * 
 * This template scans the I2C bus for connected devices and reports their addresses.
 * Useful for debugging I2C connections and verifying device addresses.
 * 
 * Expected devices for Smart Plug AI:
 * - ATECC608A: 0x60 (default address, can be 0x6A-0x6E with configuration)
 * - 24C256 EEPROM: 0x50 (can be 0x50-0x57 depending on A0-A2 pins)
 * 
 * Hardware Connections:
 * - SDA: GPIO 8 (configurable)
 * - SCL: GPIO 9 (configurable)
 * - Pull-ups: 2.2kΩ to 3.3V on both SDA and SCL (MANDATORY)
 * 
 * Usage:
 * 1. Upload this sketch to ESP32-S3
 * 2. Open Serial Monitor at 115200 baud
 * 3. Observe detected I2C addresses
 * 4. Verify expected devices are present
 */

#include <Wire.h>

// I2C pin configuration for ESP32-S3
#define I2C_SDA_PIN 8   // GPIO 8 - I2C Data line
#define I2C_SCL_PIN 9   // GPIO 9 - I2C Clock line
#define I2C_FREQ 100000 // 100kHz (standard mode) - use 400000 for fast mode

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) {
    delay(10); // Wait for serial port to connect
  }
  
  Serial.println("\n\n=== Smart Plug AI I2C Bus Scanner ===");
  Serial.println("Scanning I2C bus...\n");
  
  // Initialize I2C with custom pins
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN, I2C_FREQ);
  
  // Allow I2C bus to stabilize
  delay(100);
  
  scanI2CBus();
}

void loop() {
  // Rescan every 5 seconds
  delay(5000);
  Serial.println("\n--- Rescanning I2C bus ---");
  scanI2CBus();
}

/**
 * Scan I2C bus for all possible addresses (0x01 to 0x7F)
 * 
 * I2C Address Notes:
 * - 0x00-0x07: Reserved addresses
 * - 0x08-0x77: Valid device addresses
 * - 0x78-0x7F: Reserved addresses
 */
void scanI2CBus() {
  byte error, address;
  int devicesFound = 0;
  
  Serial.println("Addr  Device Name          Status");
  Serial.println("----  -------------------  --------");
  
  // Scan addresses 0x01 through 0x7F
  for (address = 1; address < 127; address++) {
    // Begin transmission to this address
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    // Check if device responded
    if (error == 0) {
      // Device found - print address and identify if known
      Serial.print("0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.print("  ");
      Serial.print(identifyDevice(address));
      Serial.println("  FOUND");
      devicesFound++;
    }
    else if (error == 4) {
      // Unknown error at this address
      Serial.print("0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  Unknown error!");
    }
    
    // Small delay between scans to avoid bus congestion
    delay(5);
  }
  
  // Print summary
  Serial.println("----  -------------------  --------");
  if (devicesFound == 0) {
    Serial.println("\n⚠️  WARNING: No I2C devices found!");
    Serial.println("Check your wiring:");
    Serial.println("  - SDA connected to GPIO 8");
    Serial.println("  - SCL connected to GPIO 9");
    Serial.println("  - 2.2kΩ pull-ups to 3.3V (MANDATORY)");
    Serial.println("  - Power connections (VCC, GND)");
  }
  else {
    Serial.print("\n✓ Found ");
    Serial.print(devicesFound);
    Serial.println(" I2C device(s)");
    
    // Verify expected devices
    verifyExpectedDevices();
  }
}

/**
 * Identify device by I2C address
 * Returns a friendly name if known, otherwise "Unknown"
 */
String identifyDevice(byte address) {
  switch (address) {
    case 0x50:
    case 0x51:
    case 0x52:
    case 0x53:
    case 0x54:
    case 0x55:
    case 0x56:
    case 0x57:
      return "24C256 EEPROM      ";
      
    case 0x60:
    case 0x6A:
    case 0x6B:
    case 0x6C:
    case 0x6D:
    case 0x6E:
      return "ATECC608A Crypto   ";
      
    case 0x68:
    case 0x69:
      return "DS3231 RTC / MPU   ";
      
    case 0x40:
    case 0x41:
    case 0x44:
    case 0x45:
      return "INA219 Power Mon   ";
      
    case 0x48:
    case 0x49:
    case 0x4A:
    case 0x4B:
      return "ADS1115 ADC        ";
      
    case 0x3C:
    case 0x3D:
      return "SSD1306 OLED       ";
      
    case 0x76:
    case 0x77:
      return "BME280 Temp/Humid  ";
      
    default:
      return "Unknown Device     ";
  }
}

/**
 * Verify that expected devices for Smart Plug AI are present
 */
void verifyExpectedDevices() {
  bool atecc608aFound = false;
  bool eepromFound = false;
  
  // Check for ATECC608A (address 0x60 typical)
  Wire.beginTransmission(0x60);
  if (Wire.endTransmission() == 0) {
    atecc608aFound = true;
  }
  
  // Check for 24C256 EEPROM (address 0x50 typical)
  Wire.beginTransmission(0x50);
  if (Wire.endTransmission() == 0) {
    eepromFound = true;
  }
  
  // Report verification results
  Serial.println("\nExpected Device Verification:");
  Serial.print("  ATECC608A (0x60): ");
  Serial.println(atecc608aFound ? "✓ FOUND" : "✗ NOT FOUND");
  
  Serial.print("  24C256 (0x50):    ");
  Serial.println(eepromFound ? "✓ FOUND" : "✗ NOT FOUND");
  
  if (atecc608aFound && eepromFound) {
    Serial.println("\n✓ All expected devices found - hardware OK!");
  }
  else {
    Serial.println("\n⚠️  WARNING: Some expected devices missing!");
    if (!atecc608aFound) {
      Serial.println("  - Check ATECC608A connections and power");
    }
    if (!eepromFound) {
      Serial.println("  - Check 24C256 EEPROM connections and power");
    }
  }
}

/*
 * Expected Output Example:
 * 
 * === Smart Plug AI I2C Bus Scanner ===
 * Scanning I2C bus...
 * 
 * Addr  Device Name          Status
 * ----  -------------------  --------
 * 0x50  24C256 EEPROM        FOUND
 * 0x60  ATECC608A Crypto     FOUND
 * ----  -------------------  --------
 * 
 * ✓ Found 2 I2C device(s)
 * 
 * Expected Device Verification:
 *   ATECC608A (0x60): ✓ FOUND
 *   24C256 (0x50):    ✓ FOUND
 * 
 * ✓ All expected devices found - hardware OK!
 */
