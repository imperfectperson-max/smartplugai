# Week 1 Quick Reference Guide

⚡ **Fast access to critical Week 1 information** ⚡

## 📋 At-a-Glance Assembly Sequence

```text
Day 1-2: Soldering & Assembly
├─ Morning: Component Prep
│  ├─ Workspace setup
│  ├─ Component verification
│  └─ Pre-tinning
└─ Afternoon: Assembly
   ├─ Stage 1: ESP32-S3 base
   ├─ Stage 2: Power distribution
   ├─ Stage 3: ATECC608A + I2C
   ├─ Stage 4: Current sensor
   ├─ Stage 5: Relay control
   ├─ Stage 6: Watchdog (TPS3823)
   └─ Stage 7: EEPROM

Day 3-4: Firmware Setup
├─ ESP-IDF installation
├─ Secure boot configuration
├─ ATECC608A provisioning
└─ Driver implementation

Day 5-7: Testing & Calibration
├─ Pre-power safety checks
├─ DC-only power test
├─ Relay switching test
├─ Current sensor calibration
└─ Comprehensive testing
```

## ⚡ Critical Component Connections

### ESP32-S3 I2C Bus (GPIO8/GPIO9)
```
        3.3V
         │
    ┌────┴────┐
  2.2kΩ     2.2kΩ  ← CRITICAL: Pull-ups required!
    │         │
   SDA       SCL
    │         │
    ├─────────┼─── ATECC608A (0x60)
    ├─────────┼─── 24C256 (0x50)
    └─────────┘
```

### Current Sensor (SCT-013-030)
```
SCT-013 → [33Ω 1W] → GPIO4 (ADC)
          [1N4007]   Protection
          [100nF]    Noise Filter
             │
            GND
```

### Relay Control
```
ESP32 GPIO18 → [220Ω] → PC817 LED → GND
                         PC817 Photo → Relay Coil
                                       [1N4007] ← Flyback
```

### Watchdog & Tamper
```
GPIO11 → TPS3823 WDI (toggle watchdog)
GPIO10 → TPS3823 MR (tamper detect)
TPS3823 /RST → ESP32 EN (reset on tamper)
```

## 🔧 Essential Settings

### Soldering Iron Temperatures
```
Lead-free solder: 350°C
Leaded (60/40):  330°C
SOT23 (fine):    330°C
```

### I2C Addresses
```
ATECC608A:  0x60 (0xC0 >> 1)
24C256:     0x50 (A0/A1/A2 = GND)
```

### GPIO Assignments
```
GPIO4:  Current sensor ADC
GPIO8:  I2C SDA
GPIO9:  I2C SCL
GPIO10: Tamper detect input
GPIO11: Watchdog toggle output
GPIO18: Relay control
```

### Power Rails
```
3.3V: ESP32 regulator output → I2C devices
5V:   External charger → Relay coil
GND:  Common ground (DO NOT SEPARATE!)
```

## ⚠️ Critical Safety Warnings

### Pre-Power Checks (DO NOT SKIP!)
```bash
✓ 3.3V to GND: >50Ω (no short)
✓ 5V to GND: >100Ω
✓ No continuity between 3.3V and 5V
✓ Visual: No loose wires or solder balls
✓ Visual: All components correct orientation
```

### NEVER Do This
```
✗ Work on AC while powered
✗ Skip pull-up resistors on I2C
✗ Connect 5V to ESP32 3.3V pins
✗ Lock ATECC608A without backup
✗ Test AC without isolation checks
```

### ALWAYS Do This
```
✓ Test components before soldering
✓ Check continuity after each stage
✓ Add decoupling capacitors
✓ Pre-tin wires and pins
✓ Clean flux residue
✓ Label all connections
```

## 🔍 Troubleshooting Flowchart

### ESP32 Won't Boot
```
ESP32 not booting?
  │
  ├─ No USB device?
  │   └─→ Check USB cable
  │       Check drivers
  │       Try different port
  │
  ├─ USB works but no output?
  │   └─→ Measure 3.3V at ESP32
  │       │
  │       ├─ No 3.3V? → Check power supply
  │       │             Check 5V → 3.3V regulator
  │       │
  │       └─ Has 3.3V? → Check EN pin (should be HIGH)
  │                      Check for shorts (3.3V-GND)
  │
  └─ Boots but crashes?
      └─→ Remove other components
          Test ESP32 alone
          Add back one by one
```

### I2C Device Not Found
```
I2C device not detected?
  │
  ├─→ Run I2C scanner
  │    │
  │    ├─ No devices found?
  │    │   └─→ Check pull-ups (2.2kΩ to 3.3V)
  │    │       Check SDA/SCL wiring
  │    │       Check device power (3.3V)
  │    │
  │    └─ Wrong address shown?
  │        └─→ Verify address pins (A0/A1/A2)
  │            Check device datasheet
  │
  └─→ Device powered but no response?
      └─→ Check solder joints
          Check for bridges
          Try slower I2C speed (10kHz)
```

### Relay Not Switching
```
Relay not switching?
  │
  ├─ No click sound?
  │   └─→ Check relay coil voltage (should be 5V)
  │       Check GPIO output (should toggle 0V/3.3V)
  │       Check PC817 connections
  │       │
  │       └─ Test relay directly with 5V
  │
  └─ Clicks but no AC switching?
      └─→ Check AC connections to COM and NO
          Measure continuity: COM-NO when ON
          Check AC safety interlocks
```

## 📊 Test Results Template

### Component Verification Log
```
Device: ESP32-S3-WROOM-1-N8
Serial: ______________
[ ] Visual inspection: Pass / Fail
[ ] USB enumeration: Pass / Fail
[ ] 3.3V-GND resistance: _____ Ω (should be >100Ω)
Notes: _________________________________

Device: ATECC608A
[ ] I2C detection at 0x60: Pass / Fail
[ ] Power supply 3.3V: _____ V
[ ] Pull-up resistors: _____ kΩ
Notes: _________________________________

Device: SCT-013-030
[ ] Secondary resistance: _____ Ω (should be 30-40Ω)
[ ] Primary continuity: Open / Short (should be open)
Notes: _________________________________
```

### Power-On Test Checklist
```
Pre-Power Safety (DC only, no AC):
[ ] 3.3V-GND resistance: _____ Ω (>50Ω)
[ ] 5V-GND resistance: _____ Ω (>100Ω)
[ ] No 3.3V-5V continuity
[ ] Visual inspection complete
[ ] All components oriented correctly

First Power-On (5V DC only):
[ ] ESP32 boots to serial output
[ ] 3.3V rail measures: _____ V (3.15-3.45V)
[ ] 5V rail measures: _____ V (4.75-5.25V)
[ ] I2C scan shows devices:
    [ ] ATECC608A at 0x60
    [ ] 24C256 at 0x50
[ ] Current sensor ADC reads ~1.65V (no load)
[ ] Relay clicks when toggled

Calibration Results:
[ ] Zero offset: _____ V
[ ] 60W bulb test: _____ A (expected 0.26A)
[ ] 100W bulb test: _____ A (expected 0.43A)
[ ] Calibration error: _____ % (should be <2%)
```

## 💾 Quick Command Reference

### ESP-IDF Setup
```bash
# Install ESP-IDF
cd ~/esp
git clone -b v5.2.1 --recursive https://github.com/espressif/esp-idf.git
cd esp-idf
./install.sh esp32s3

# Activate environment
. ./export.sh

# Create project
idf.py create-project smartplug

# Configure
idf.py menuconfig

# Build
idf.py build

# Flash
idf.py -p /dev/ttyUSB0 flash monitor
```

### I2C Scanner Code
```c
// Quick I2C scanner for testing
void scan_i2c() {
    for (uint8_t addr = 1; addr < 127; addr++) {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true);
        i2c_master_stop(cmd);
        
        if (i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000/portTICK_PERIOD_MS) == ESP_OK) {
            printf("Device found at 0x%02X\n", addr);
        }
        i2c_cmd_link_delete(cmd);
    }
}
```

### Multimeter Tests
```bash
# Resistance test
Red probe → Point A
Black probe → Point B
Read resistance (Ω)

# Voltage test
Red probe → Positive point
Black probe → GND
Read voltage (V)

# Continuity test
Activate continuity mode (diode symbol)
Touch probes to two points
Beep = connected, No beep = open
```

## 🎯 Week 1 Success Criteria

### Hardware Deliverables
```
[ ] One fully assembled smart plug prototype
[ ] No solder bridges or cold joints
[ ] All components tested and working
[ ] Proper wire management and labeling
[ ] Enclosure ready for integration
```

### Software Deliverables
```
[ ] ESP-IDF project compiles and runs
[ ] Secure boot enabled (development keys)
[ ] ATECC608A responding on I2C
[ ] Current sensor reading calibrated (<2% error)
[ ] Relay control functional
[ ] Watchdog toggling verified
```

### Documentation Deliverables
```
[ ] Assembly photos at each stage
[ ] Component test results logged
[ ] Calibration certificate generated
[ ] Issues encountered and resolved documented
[ ] BOM verified with actual components used
```

### Safety Deliverables
```
[ ] Isolation tests passed (>1MΩ AC to DC)
[ ] No shorts between power rails
[ ] Fuse and MOV installed on AC side
[ ] Tamper detection verified
[ ] All safety warnings labeled
```

## 📞 Emergency Contacts & Resources

### If Something Goes Wrong
```
Electrical Safety Issue:
→ Immediately disconnect power
→ Do not touch any components
→ Verify isolation before investigating

Component Damaged:
→ Disconnect power
→ Remove damaged component
→ Test rest of circuit in isolation
→ Replace with known-good component

Can't Solve Problem:
→ Take clear photos of issue
→ Document exactly what happened
→ Post to project forum with details
→ Wait for expert guidance before proceeding
```

### Useful Resources
- Full Week 1 Guide: `docs/WEEK1_HARDWARE_MASTERY.md`
- Soldering Guide: `docs/soldering-checklist.md`
- Firmware README: `firmware/README.md`
- Security Guide: `docs/SECURITY.md`

---

**Remember**: Quality over speed. One perfect joint is better than ten rushed ones!

**Version**: 1.0  
**Last Updated**: January 2026
