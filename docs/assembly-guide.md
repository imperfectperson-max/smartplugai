# Assembly Guide for Smart Plug AI

Complete step-by-step assembly instructions for building a Smart Plug AI device with bank-grade security features.

## 📋 Table of Contents

- [Before You Begin](#before-you-begin)
- [Required Components](#required-components)
- [Required Tools](#required-tools)
- [Safety Warnings](#safety-warnings)
- [Assembly Steps](#assembly-steps)
- [Testing & Validation](#testing--validation)
- [Troubleshooting](#troubleshooting)

---

## Before You Begin

**⏱️ Estimated Time**: 4-6 hours for first device, 3-4 hours for subsequent devices

**👥 Skill Level**: Intermediate to Advanced (soldering experience required, electrical knowledge highly recommended)

**⚠️ Safety**: This project involves mains voltage (230V AC) which can be LETHAL. If you are not comfortable working with mains voltage, please consult a licensed electrician.

### Pre-Assembly Checklist

- [ ] Read entire assembly guide before starting
- [ ] Review [Week 1 Development Summary](WEEK1_DEVELOPMENT_SUMMARY.md) for detailed procedures
- [ ] Review [Soldering Checklist](soldering-checklist.md) for hardware requirements
- [ ] Workspace clean, well-lit, and organized
- [ ] All components verified and tested individually
- [ ] All tools available and in good condition
- [ ] Safety equipment ready (glasses, fire extinguisher, first aid kit)
- [ ] Sufficient time allocated (do not rush!)

---

## Required Components

See [Soldering Checklist](soldering-checklist.md) for complete Bill of Materials.

### Essential Components (per device):
- ESP32-S3-WROOM-1-N8 development board
- ATECC608A-TNGTLS secure element breakout
- SCT-013-030 current transformer (30A/1V)
- TPS3823-33 watchdog timer + SOT23 adapter
- 24C256 EEPROM (32KB, I2C)
- 5V/2A certified phone charger (power supply)
- 5V relay module with optocoupler (10A minimum)
- 33Ω 1W burden resistor
- PC817 optocouplers (2x for isolation)
- 3A quick-blow fuse + holder
- 275V MOV (metal oxide varistor)
- Professional ABS enclosure
- Tamper detection switch (normally closed)
- Anti-tamper screws and void-if-removed seals

### Passive Components:
- Resistors: 2.2kΩ (4x), 220Ω (2x), 10kΩ (2x)
- Capacitors: 100µF electrolytic, 100nF ceramic (5x), 10µF (2x)
- Diodes: 1N4007 (3x)
- LEDs with current-limiting resistors (optional, for status indication)

---

## Required Tools

### Soldering Tools:
- Temperature-controlled soldering iron (350-400°C)
- Fine tip (0.5mm) and chisel tip (2mm)
- Lead-free solder (0.7mm diameter)
- Flux pen or paste
- Solder wick / desoldering pump
- 99% isopropyl alcohol for cleaning
- Magnifying glass or head-mounted magnifier (essential for SOT23!)
- Helping hands or PCB holder

### Hand Tools:
- Wire strippers (16-20 AWG)
- Wire cutters / flush cutters
- Needle nose pliers
- Phillips and flathead screwdrivers
- Security bit screwdrivers (for anti-tamper screws)
- Hot glue gun (for strain relief)

### Testing Equipment:
- **Digital multimeter (essential!)**: Auto-ranging, with continuity tester
- **Megohmmeter (highly recommended)**: For isolation testing (500V)
- USB cable for ESP32 programming
- Computer with Arduino IDE or PlatformIO
- Breadboard for component testing
- Test loads: 60W and 100W incandescent bulbs

### Safety Equipment:
- Safety glasses (mandatory!)
- Heat-resistant mat
- Fume extractor or good ventilation
- Fire extinguisher (ABC type)
- First aid kit
- ESD wrist strap (recommended)

---

## Safety Warnings

### ⚡ Electrical Safety (CRITICAL!)

**DANGER - MAINS VOLTAGE CAN KILL**

- ⚠️ **NEVER** work on AC circuits while powered
- ⚠️ **ALWAYS** disconnect from mains before opening enclosure
- ⚠️ Use **ISOLATION TRANSFORMER** during testing if available
- ⚠️ Test with **GFCI/RCD PROTECTED** outlet (mandatory!)
- ⚠️ Follow **ONE HAND IN POCKET** rule when probing AC circuits
- ⚠️ **DISCHARGE CAPACITORS** before touching AC components
- ⚠️ Use **INSULATED TOOLS** for all AC work
- ⚠️ Maintain >10mm **CREEPAGE AND CLEARANCE** between AC and DC
- ⚠️ Verify **PROPER GROUNDING** of all AC connections
- ⚠️ Follow **LOCAL ELECTRICAL CODES** and regulations

**IF YOU ARE NOT 100% CONFIDENT, HIRE A LICENSED ELECTRICIAN!**

### 🔥 Soldering Safety

- Always work in well-ventilated area
- Never leave soldering iron unattended
- Keep flammable materials away from work area
- Use fume extractor - solder fumes are hazardous
- Let components cool before touching
- Clean up any solder balls/splatter immediately

### 🔐 Security Warnings

- **ATECC608A configuration lock is IRREVERSIBLE** - test on spare chip first!
- **Back up all keys** to encrypted offline storage
- **Document all provisioning** steps and configuration
- **Never commit keys** to version control

---

## Assembly Steps

### Stage 1: Component Preparation & Testing (1-2 hours)

#### 1.1 Workspace Setup

1. Clean work surface with anti-static mat
2. Organize components in labeled bins/trays
3. Set up soldering iron to 350°C (lead-free)
4. Prepare flux, solder wick, isopropyl alcohol
5. Position magnifying glass/lamp
6. Connect fume extractor

#### 1.2 Component Verification

Test EVERY component before soldering:

**ESP32-S3 Test**:
```bash
1. Visual inspection: No bent pins, clean pads
2. Resistance test: GND to 3.3V = >100Ω (no short)
3. USB test: Connect to computer, verify COM port appears
4. Upload blink sketch: Verify basic functionality
```

**ATECC608A Test**:
```bash
1. Connect to ESP32 on breadboard:
   - VCC → 3.3V
   - GND → GND  
   - SDA → GPIO 8 (with 2.2kΩ pull-up to 3.3V)
   - SCL → GPIO 9 (with 2.2kΩ pull-up to 3.3V)
2. Upload I2C scanner (see firmware/templates/i2c_scanner.ino)
3. Verify device detected at address 0x60
```

**SCT-013 Test**:
```bash
1. Continuity test: Primary (split core) = open circuit ✓
2. Resistance test: Secondary (3.5mm jack) = 30-40Ω ✓
3. Polarity check: Note white stripe orientation
```

**24C256 EEPROM Test**:
```bash
1. Connect to ESP32 on breadboard (same I2C bus as ATECC608A)
2. Run I2C scanner
3. Verify device detected at address 0x50
```

**Relay Module Test**:
```bash
1. Connect VCC to 5V, GND to GND
2. Apply 3.3V to IN pin (via 220Ω resistor if no optocoupler)
3. Listen for audible click
4. Measure continuity between COM and NO (should be closed when energized)
```

**TPS3823 Test** (Optional, can test after soldering to adapter):
```bash
1. Solder to SOT23-to-DIP adapter first (practice on scrap board!)
2. Connect VDD to 3.3V, GND to GND
3. Toggle WDI pin every 500ms
4. Verify /RST pin stays HIGH
5. Stop toggling, verify /RST goes LOW within 1.6s
```

**⚠️ IMPORTANT**: Any failed component should be replaced before assembly!

---

### Stage 2: DC Circuit Assembly (2-3 hours)

Follow the exact sequence to prevent rework:

#### 2.1 ESP32-S3 Base Installation

1. Insert female headers into breadboard (spacing matches ESP32 pins)
2. Place ESP32-S3 on headers (component side up)
3. Solder ONE pin at each corner (4 pins total)
4. Check alignment - should be flat and centered
5. If misaligned, reheat corner pins and adjust
6. Solder all remaining pins
7. **Inspect**: No bridges, shiny concave joints, no cold solder

**Quality Check**: 
- Visual: All joints shiny and smooth
- Continuity: Test adjacent pins - should be OPEN (no bridges)

#### 2.2 Power Rails Distribution

1. **3.3V Rail** (RED wire):
   - Connect ESP32 3.3V pin to breadboard positive rail
   - Route to ATECC608A, 24C256, TPS3823
   
2. **GND Rail** (BLACK wire):
   - Connect ESP32 GND to breadboard negative rail
   - Common ground for all components
   
3. **5V Rail** (BLUE wire):
   - From external 5V power supply
   - Route to relay module VCC
   - Add 100µF electrolytic capacitor between 5V and GND (near power input)
   
4. **Decoupling Capacitors**:
   - 100nF ceramic capacitor between 3.3V and GND (near ESP32)
   - 10µF electrolytic between 3.3V and GND
   - 100nF near ATECC608A VCC pin
   - 100nF near 24C256 VCC pin

**Verify Power Rails**:
```bash
Multimeter checks (ESP32 powered via USB):
- 3.3V rail to GND: Expect ~3.28V ±0.05V
- Check for shorts: 3.3V to 5V rail should be OPEN
```

#### 2.3 I2C Bus Wiring

**Critical**: I2C bus shared by ATECC608A and 24C256

```
ATECC608A → ESP32-S3:
  VCC → 3.3V rail
  GND → GND rail
  SDA → GPIO 8
  SCL → GPIO 9

24C256 EEPROM → ESP32-S3:
  VCC → 3.3V rail
  GND → GND rail
  SDA → GPIO 8 (shared with ATECC608A)
  SCL → GPIO 9 (shared with ATECC608A)
  A0, A1, A2 → GND (sets address to 0x50)

⚠️ MANDATORY: Add 2.2kΩ pull-up resistors:
  - One resistor from SDA to 3.3V
  - One resistor from SCL to 3.3V
```

**Verify I2C Bus**:
```bash
1. Upload I2C scanner to ESP32
2. Expected output:
   - 0x50: 24C256 EEPROM
   - 0x60: ATECC608A Crypto
3. If devices not detected, check pull-ups and connections
```

#### 2.4 Current Sensor Circuit

**SCT-013-030 Wiring**:
```
1. 3.5mm jack tip → 33Ω 1W burden resistor → ESP32 GPIO 4
2. 3.5mm jack sleeve → GND
3. Add 100nF capacitor from GPIO 4 to GND (noise filtering)
4. Add 1N4007 diode across burden resistor (protection)
   - Cathode (stripe) to GPIO 4
   - Anode to GND

⚠️ Use 33Ω ±1% resistor for accurate calibration!
📍 Label this connection clearly: "SCT-013 Input"
```

**Verify**:
- Resistance from GPIO 4 to GND: Should read ~33Ω
- No shorts to 3.3V or 5V rails

#### 2.5 Relay Control with Optocoupler

**PC817 Optocoupler Circuit**:
```
Stage 1: ESP32 → PC817 LED
  ESP32 GPIO 18 → 220Ω resistor → PC817 pin 1 (anode)
  PC817 pin 2 (cathode) → GND

Stage 2: PC817 Transistor → Relay
  5V rail → PC817 pin 4 (collector)
  PC817 pin 3 (emitter) → Relay IN pin
  Relay VCC → 5V rail
  Relay GND → GND rail

Stage 3: Relay Protection
  1N4007 diode across relay coil (flyback protection):
    - Cathode to VCC
    - Anode to IN
```

**Verify**:
- Set GPIO 18 HIGH → LED in PC817 should light → Relay should click
- Measure continuity: Relay COM to NO should be CLOSED when energized

#### 2.6 Watchdog Circuit (TPS3823)

**⚠️ ADVANCED**: SOT23 package - practice soldering first!

**TPS3823 Wiring** (on SOT23-to-DIP adapter):
```
Pin 1 (VDD) → 3.3V
Pin 2 (GND) → GND
Pin 3 (MR) → GPIO 10 (manual reset / tamper input)
Pin 4 (WDI) → GPIO 11 (watchdog input - must toggle)
Pin 5 (/RST) → ESP32 EN pin (via 10kΩ pull-up to 3.3V)

Tamper Switch:
  - Normally closed switch
  - One side to GPIO 10
  - Other side to GND
  - Opening switch triggers tamper detection
```

**Verify**:
- With GPIO 11 toggling every 500ms, /RST pin should stay HIGH
- Stop toggling, /RST should go LOW within 1.6s (resets ESP32)

---

### Stage 3: AC Circuit Assembly (1 hour)

**⚠️⚠️⚠️ DANGER - MAINS VOLTAGE - LETHAL ⚠️⚠️⚠️**

**NEVER WORK ON AC CIRCUITS WHILE POWERED!**

#### 3.1 Pre-AC Safety Checks

Before connecting ANY AC:
```bash
1. Visual inspection: No loose wires, solder balls, touching conductors
2. All polarized components correct orientation
3. Proper wire gauge: 16-18 AWG minimum for AC
4. Strain relief on all AC connections
5. Creepage/clearance: >10mm between AC and DC traces/wires
```

#### 3.2 AC Input Wiring

```
AC Mains Input → Fuse → MOV → Power Supply & Relay
  
1. AC Live (brown/red) → 3A quick-blow fuse holder
2. Fuse output → One side of 275V MOV
3. MOV other side → AC Neutral (blue/black)
4. After fuse → Power supply Live input
5. After fuse → Relay Common (C) terminal
6. AC Neutral → Power supply Neutral input
7. AC Neutral → AC Output Neutral (direct connection)
8. AC Ground (green/yellow) → AC Output Ground (direct connection)

⚠️ Fuse: 3A quick-blow (rated for 250VAC minimum)
⚠️ MOV: 275V, 7mm diameter (surge protection)
```

#### 3.3 SCT-013 Installation

```
AC Live wire (after fuse) passes through SCT-013:
1. Open split-core CT
2. Pass Live wire through center
3. Close CT (should click)
4. Note white stripe orientation (current direction)
5. Connect CT 3.5mm jack to burden resistor circuit
```

**⚠️ IMPORTANT**: Only ONE wire through CT (Live only, not Neutral!)

#### 3.4 Relay to AC Output

```
Relay Switching:
  Relay Common (C) → AC Live (from fuse/MOV)
  Relay Normally Open (NO) → AC Output Live terminal
  
AC Output Socket:
  Live → From relay NO
  Neutral → Direct from AC input
  Ground → Direct from AC input (never switched!)
```

#### 3.5 AC Safety Verification

**⚠️ MANDATORY CHECKS** (with AC disconnected, using multimeter):
```bash
1. 3.3V to AC Live: >1MΩ (isolation)
2. 3.3V to AC Neutral: >1MΩ (isolation)
3. GND to AC Live: >1MΩ (isolation)
4. GND to AC Neutral: >1MΩ (isolation)
5. AC Live to AC Neutral: OPEN (relay off)
6. AC Live to AC Ground: >1MΩ
7. AC Neutral to AC Ground: >1MΩ

WITH MEGOHMMETER (500V test voltage):
1. AC Live to DC GND: >10MΩ ✓
2. AC Neutral to DC GND: >10MΩ ✓
```

**If ANY measurement fails, DO NOT PROCEED! Find and fix the issue!**

---

### Stage 4: Enclosure Integration (1 hour)

#### 4.1 Component Mounting

1. **Position components in enclosure**:
   - ESP32 near bottom (away from AC)
   - Relay module near AC terminals
   - Power supply in separate compartment (if possible)
   - Current sensor near AC input
   
2. **Mount with standoffs or hot glue**:
   - ESP32: Use standoffs or hot glue corners
   - Relay: Secure with screws or strong double-sided tape
   - Power supply: Must be firmly mounted
   
3. **Wire routing**:
   - AC wires: Thick gauge, short routes, secured
   - DC wires: Thin gauge, neat routing, avoid AC wires
   - Maintain >10mm separation between AC and DC

#### 4.2 Tamper Detection

1. **Install tamper switch on enclosure lid**:
   - Normally closed switch
   - Opens when enclosure is opened
   - Connect to GPIO 10 (MR pin of TPS3823)
   
2. **Test tamper detection**:
   - Close enclosure → Switch closed → GPIO 10 LOW
   - Open enclosure → Switch opens → GPIO 10 HIGH → ESP32 resets

#### 4.3 Final Enclosure Assembly

1. **Cable management**:
   - Use cable ties for neat wire bundles
   - Strain relief on all external cables
   - No wires pinched by enclosure
   
2. **Labeling**:
   - INPUT: AC power input
   - OUTPUT: AC socket output
   - Warning labels: "DANGER - HIGH VOLTAGE"
   - Device ID label
   
3. **Secure enclosure**:
   - Anti-tamper screws
   - Void-if-removed seals on screw heads
   - Document seal serial numbers

---

## Testing & Validation

### Test Sequence (DO NOT SKIP ANY STEP!)

#### Test 1: DC-Only Power-On

**⚠️ AC NOT CONNECTED YET!**

```bash
1. Connect USB to ESP32 (for programming and power)
2. Upload test firmware (see firmware/templates/test_dc_only.c)
3. Open serial monitor
4. Verify:
   - ESP32 boots successfully
   - I2C devices detected (0x50, 0x60)
   - ADC reading ~1.65V (current sensor with no load)
   - Relay clicks when GPIO 18 toggled
   - Watchdog pin toggling every 500ms
   - Tamper detection works (open/close enclosure)
```

**Expected Output**:
```
=== DC-ONLY SAFETY TEST ===
1. Testing ESP32... PASS
2. Scanning I2C bus... PASS (2 devices found)
3. Testing ADC... PASS (1.64V)
4. Testing relay control... PASS (click heard)
5. Testing watchdog toggle... PASS (toggling)
=== DC TESTS COMPLETE ===
```

#### Test 2: Add 5V Power Supply

```bash
1. Disconnect USB power
2. Connect certified 5V/2A phone charger output to 5V rail
3. Verify:
   - ESP32 boots from external 5V
   - 5V rail measures 4.9-5.1V
   - 3.3V rail still ~3.28V
   - Relay coil gets 5V
4. Check temperature after 10 minutes:
   - ESP32: <50°C
   - Power supply: <60°C
   - Relay: <40°C (when not energized)
```

#### Test 3: Relay Switching (No Load)

```bash
1. Upload relay test firmware
2. Cycle relay 10 times
3. Verify:
   - Audible click each time
   - COM to NO continuity when ON
   - COM to NO open when OFF
   - No excessive heating
   - No arcing sounds
```

#### Test 4: First AC Power-On (No Load)

**⚠️ USE GFCI/RCD PROTECTED OUTLET!**

```bash
SAFETY CHECKS FIRST:
1. Enclosure closed and sealed
2. All AC connections tight and insulated
3. Isolation test passed (>10MΩ)
4. Fire extinguisher nearby
5. One person only working on AC
6. Emergency shutoff accessible

POWER-ON PROCEDURE:
1. Plug into GFCI-protected outlet
2. Verify LED power indicator on power supply
3. Check ESP32 boots
4. Measure 5V output: 4.9-5.1V ✓
5. No smoke, no buzzing, no burning smell ✓
6. Let run for 5 minutes
7. Check temperatures:
   - Power supply: <60°C
   - Relay: <40°C
   - ESP32: <50°C
```

#### Test 5: Current Sensor Calibration

**Calibration Procedure** (see firmware/templates/current_sensor_driver.c):

```bash
1. Zero offset (no load):
   - Run calibration firmware
   - NO load connected
   - Take 1000 samples
   - Expected: 1.60-1.70V

2. Known load (60W bulb):
   - Connect 60W incandescent bulb to output
   - Expected current: ~0.261A @ 230V
   - Run calibration with known load
   - Target error: <2%

3. Verification (100W bulb):
   - Connect 100W bulb
   - Expected: ~0.435A @ 230V
   - Measure actual current
   - Target error: <2%

4. Save calibration:
   - Store to EEPROM
   - Generate calibration certificate
```

#### Test 6: Full Load Testing

```bash
1. Start with low load (40W bulb)
2. Monitor for 15 minutes:
   - Current reading stable
   - No excessive heating
   - Relay operates normally

3. Increase to rated load (8-10A):
   - Use appropriate resistive load
   - Monitor for 30 minutes
   - Temperature checks:
     - Relay contacts: <70°C
     - AC wiring: <50°C
     - Power supply: <70°C

⚠️ DO NOT EXCEED 10A! Relay rated for 10A maximum
```

#### Test 7: Safety Function Tests

```bash
1. Tamper Detection:
   - Open enclosure
   - Verify immediate tamper alert
   - Verify sensitive data wiped
   - Verify system reset

2. Watchdog Function:
   - Stop watchdog toggle task
   - Verify system resets within 1.6s

3. Over-Current (if equipped):
   - Gradually increase load
   - Verify relay opens at threshold

4. GFCI Test:
   - Verify GFCI trips if ground fault occurs
```

---

## Troubleshooting

### Issue: ESP32 Won't Boot

**Possible Causes**:
- Insufficient power supply
- Short circuit
- Damaged ESP32

**Solutions**:
1. Check 3.3V rail with multimeter (should be 3.28V ±0.05V)
2. Disconnect all peripherals, try booting ESP32 alone
3. Check for solder bridges on ESP32 pins
4. Try different USB cable/port
5. Check EN pin - should be HIGH (3.3V)

### Issue: I2C Devices Not Detected

**Possible Causes**:
- Missing pull-up resistors
- Wrong GPIO pins
- Bad solder joints
- Incorrect I2C address

**Solutions**:
1. Verify 2.2kΩ pull-ups on SDA/SCL to 3.3V
2. Check wiring: SDA to GPIO 8, SCL to GPIO 9
3. Inspect solder joints under magnification
4. Try different I2C addresses (ATECC608A can be 0x60, 0x6A-0x6E)
5. Run I2C scanner at different speeds (100kHz, 400kHz)

### Issue: Relay Won't Switch

**Possible Causes**:
- Insufficient voltage
- Bad relay
- Optocoupler not working
- GPIO not toggling

**Solutions**:
1. Measure GPIO 18 output: Should be 3.3V HIGH, 0V LOW
2. Check optocoupler LED with multimeter (should drop ~1.2V when on)
3. Bypass optocoupler temporarily - connect GPIO directly to relay IN (via 220Ω)
4. Test relay with direct 5V application to coil
5. Replace relay if faulty

### Issue: Current Readings Inaccurate

**Possible Causes**:
- Wrong burden resistor value
- Missing calibration
- Noise/interference
- CT installed wrong

**Solutions**:
1. Verify 33Ω ±1% burden resistor
2. Run zero calibration (no load)
3. Run known-load calibration (60W bulb)
4. Add/check 100nF noise filter capacitor
5. Verify CT orientation (white stripe)
6. Ensure only ONE wire through CT
7. Check ADC voltage: Should be ~1.65V with no load

### Issue: Watchdog Constantly Resetting System

**Possible Causes**:
- Watchdog task not running
- Task blocked/delayed
- Wrong GPIO pin
- TPS3823 timeout too short

**Solutions**:
1. Verify watchdog task has high priority
2. Check watchdog task is not blocked on I/O
3. Verify GPIO 11 toggling with oscilloscope
4. Increase task priority to configMAX_PRIORITIES - 1
5. Check TPS3823 connections

### Issue: Tamper Detection Not Working

**Possible Causes**:
- Switch wired wrong
- Interrupt not configured
- GPIO floating

**Solutions**:
1. Test switch continuity: Closed when enclosure closed
2. Verify internal pull-up on GPIO 10
3. Check interrupt is configured for POSEDGE
4. Test manually: Connect GPIO 10 to 3.3V → Should trigger tamper

### Issue: High Voltage at Low Side (DC)

**⚠️ CRITICAL SAFETY ISSUE!**

**Possible Causes**:
- Power supply failure
- Insulation breakdown
- Capacitive coupling

**IMMEDIATE ACTIONS**:
1. **DISCONNECT AC POWER IMMEDIATELY!**
2. DO NOT TOUCH ANY PART OF DEVICE
3. Measure isolation with megohmmeter (must be >10MΩ)
4. If <10MΩ, device is UNSAFE - do not use!
5. Inspect for damaged insulation, pinched wires, solder bridges
6. Replace power supply if internal fault
7. Re-test isolation before any further use

---

## Quality Checklist

Before declaring device complete:

### Visual Inspection
- [ ] No solder bridges
- [ ] All joints shiny and concave
- [ ] No cold solder joints
- [ ] No loose wires
- [ ] Proper wire routing (AC separated from DC)
- [ ] Strain relief on all external cables
- [ ] No exposed conductors
- [ ] Enclosure closes properly
- [ ] Tamper seals applied

### Electrical Tests
- [ ] 3.3V rail: 3.28V ±0.05V
- [ ] 5V rail: 4.95V ±0.10V
- [ ] I2C devices detected (0x50, 0x60)
- [ ] Current sensor: 1.60-1.70V with no load
- [ ] Relay switches audibly
- [ ] Watchdog toggles every 500ms
- [ ] Tamper detection triggers on enclosure open

### Safety Tests
- [ ] Isolation >10MΩ @ 500V (AC to DC)
- [ ] No shorts between AC Live and Neutral
- [ ] No shorts between AC and Ground
- [ ] All AC connections tight
- [ ] Fuse installed (3A quick-blow)
- [ ] MOV installed (275V)
- [ ] GFCI protection verified

### Functional Tests
- [ ] ESP32 boots successfully
- [ ] ATECC608A provisioned
- [ ] Secure boot enabled
- [ ] Flash encryption enabled
- [ ] Current sensor calibrated (±2% accuracy)
- [ ] Relay operates under load
- [ ] Tamper detection functional
- [ ] Watchdog protects against hangs

### Documentation
- [ ] Device ID assigned and labeled
- [ ] Calibration certificate generated
- [ ] Assembly photos taken
- [ ] Test results recorded
- [ ] Tamper seal serial numbers logged
- [ ] Build date documented

---

## Next Steps After Assembly

1. **Firmware Setup**:
   - Flash production firmware
   - Configure Wi-Fi credentials (encrypted in NVS)
   - Test MQTT connection over TLS
   - Verify device attestation

2. **Extended Testing**:
   - 48-hour burn-in test
   - Load testing at various power levels
   - Temperature monitoring
   - Stability verification

3. **Documentation**:
   - Generate calibration certificate
   - Take final assembly photos
   - Update device registry
   - Create user manual

4. **Deployment**:
   - Final enclosure seal
   - Apply tamper-evident stickers
   - Document seal serial numbers
   - Ship/install device

---

## Support & Resources

**Documentation**:
- [Week 1 Development Summary](WEEK1_DEVELOPMENT_SUMMARY.md) - Detailed procedures
- [Soldering Checklist](soldering-checklist.md) - BOM and shopping list
- [Firmware Templates](../firmware/templates/README.md) - Code examples

**External Resources**:
- ESP32-S3 Datasheet: https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf
- ATECC608A Datasheet: https://www.microchip.com/ATECC608A
- SCT-013 Guide: http://openenergymonitor.org/emon/buildingblocks/ct-sensors-interface
- TPS3823 Datasheet: https://www.ti.com/product/TPS3823

**Contact**:
- Hardware Support: hardware@smartplugai.com
- Firmware Support: firmware@smartplugai.com
- Security Questions: security@smartplugai.com

---

**Document Version**: 1.0  
**Last Updated**: 2024-01-15  
**Author**: Smart Plug AI Development Team
