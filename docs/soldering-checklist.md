# Soldering Session Checklist

This checklist ensures you're prepared for the hardware assembly session and can safely build the Smart Plug AI secure prototype with bank-grade security features.

## 📦 Phase 1: Hardware Shopping List (3 Secure Plugs)

**Total Budget**: R2,736 - R3,182 (3 plugs with bank-grade security)

### 🔴 ESSENTIAL - MUST BUY FIRST (3-Plug Strategy)

| Item | Specification | Qty | Price (ZAR) | Priority | Notes |
|------|--------------|-----|-------------|----------|-------|
| **Microcontroller** | ESP32-S3-WROOM-1-N8 | 3 | R480 (R160 ea) | 🔴 CRITICAL | Hardware RSA/ECC crypto acceleration, secure boot, flash encryption |
| **Security Chip** | ATECC608A-TNGTLS | 3 | R195 (R65 ea) | 🔴 CRITICAL | Hardware secure element for key storage (ECDSA P256), never-readable private keys |
| **Current Sensor** | SCT-013-030 (30A/1V) | 3 | R315 (R105 ea) | 🔴 CRITICAL | With 3.5mm jack, most accurate for residential loads |
| **Tamper Detection** | MAX6316 watchdog | 3 | R120 (R40 ea) | 🔴 CRITICAL | Physical security: reset on tampering, battery-backed detection |
| **Secure Storage** | 24C256 EEPROM (32KB) | 3 | R75 (R25 ea) | 🔴 CRITICAL | Encrypted configuration storage (device certs, Wi-Fi credentials) |
| **Relay Module** | 5V, 10A+ with optocoupler | 3 | R255 (R85 ea) | 🔴 CRITICAL | Must be identical for consistency, optoisolation for safety |
| **AC-DC Converter** | 5V/2A phone charger (certified) | 3 | R150 (R50 ea) | 🔴 CRITICAL | Safer than HLK-PM01, must be certified (prototype phase) |
| **Burden Resistor** | 33Ω, 1W for SCT-013 | 3 | R24 (R8 ea) | 🔴 CRITICAL | Most forgotten item! Required for SCT-013 calibration |
| **SA Plug + Socket** | New, professional grade | 3 sets | R270 (R90 ea) | 🔴 CRITICAL | Professional appearance, NOT salvaged |

### 🛡️ SAFETY & SECURITY COMPONENTS (NON-NEGOTIABLE)

| Item | Specification | Qty | Price (ZAR) | Priority | Notes |
|------|--------------|-----|-------------|----------|-------|
| **Fuse & Holder** | 3A quick-blow | 3 | R90 (R30 ea) | 🔴 CRITICAL | Safety protection for AC line |
| **Varistor (MOV)** | 275V, 7mm diameter | 3 | R54 (R18 ea) | 🔴 CRITICAL | Surge protection, essential for South African grid |
| **Optocouplers** | PC817 (extra isolation) | 6 | R48 (R8 ea) | 🔴 CRITICAL | Additional galvanic isolation for relay control |
| **Tamper-Evident Seals** | Void-if-removed type | 10 | R30 | 🔴 CRITICAL | Physical security indicator on enclosure |

### 🔧 BASIC ELECTRONICS KIT (SHARED ACROSS ALL 3 PLUGS)

| Item | Specification | Qty | Price (ZAR) | Priority | Notes |
|------|--------------|-----|-------------|----------|-------|
| **Breadboard** | 830 points | 1 | R75 | 🔴 CRITICAL | For prototyping before PCB |
| **Jumper Wires** | M/M, M/F mix | 1 kit | R75 | 🔴 CRITICAL | Assorted lengths |
| **Resistor Kit** | Assorted values (1Ω-1MΩ) | 1 | R65 | 🔴 CRITICAL | Includes pull-ups for I2C |
| **Capacitor Kit** | Assorted values (pF-μF) | 1 | R65 | 🔴 CRITICAL | For filtering and decoupling |
| **Diode Kit** | 1N4148 + 1N4007 | 1 | R25 | 🔴 CRITICAL | Signal and power diodes |
| **LED Kit** | RGB + current-limiting resistors | 1 | R35 | 🔴 CRITICAL | Status indicators |

### 🎨 PROJECTS DAY POLISH (WEEK 3-4)

| Item | Specification | Qty | Price (ZAR) | Priority | Notes |
|------|--------------|-----|-------------|----------|-------|
| **Professional Enclosures** | ABS, anti-tamper screws | 3 | R450 (R150 ea) | 🔴 CRITICAL | Tamper-resistant with screw covers |
| **Terminal Blocks** | 2-pin, 5.08mm pitch | 15 | R45 (R3 ea) | 🔴 CRITICAL | For secure AC/DC connections |
| **OLED Display** | 0.96" I2C SSD1306 (optional) | 1 | R90 | 🟡 IMPORTANT | For device status and QR pairing |
| **RGB LEDs** | WS2812B NeoPixel | 3 | R60 (R20 ea) | 🟡 IMPORTANT | Status indication |
| **Tactile Buttons** | 6x6mm with covers | 6 | R20 | 🟢 NICE | Manual pairing/reset |

### 🛠️ TOOLS (ONE-TIME INVESTMENT)

| Item | Specification | Qty | Price (ZAR) | Priority | Notes |
|------|--------------|-----|-------------|----------|-------|
| **Multimeter** | Digital, auto-ranging | 1 | R280 | 🔴 CRITICAL | Essential for debugging and safety testing |
| **Soldering Iron** | Temperature-controlled | 1 | R200 | 🟡 IMPORTANT | 350-400°C for lead-free solder |
| **Screwdrivers** | Set with security bits | 1 | R100 | 🟡 IMPORTANT | For anti-tamper screws |
| **Wire Strippers** | 16-20 AWG | 1 | R80 | 🟡 IMPORTANT | For AC wiring |

---

## 🔧 Tools & Equipment

### Soldering Tools
- [ ] Soldering iron (temperature controlled, 350-400°C)
- [ ] Solder (60/40 or lead-free, 0.7mm diameter)
- [ ] Flux pen (essential for clean joints)
- [ ] Desoldering wick or pump (for mistakes)
- [ ] Soldering iron stand with tip cleaner
- [ ] Tip cleaner (brass wool + wet sponge)
- [ ] Helping hands or PCB holder
- [ ] Magnifying glass or head-mounted magnifier
- [ ] 99% isopropyl alcohol for flux cleanup

### Hand Tools
- [ ] Wire strippers (for 16-20 AWG)
- [ ] Wire cutters/flush cutters
- [ ] Needle nose pliers
- [ ] Screwdrivers (Phillips and flathead)
- [ ] Multimeter (essential!)
- [ ] Hot glue gun (for strain relief)

### Safety Equipment
- [ ] Safety glasses
- [ ] ESD wrist strap (optional but recommended)
- [ ] Heat-resistant mat
- [ ] Fire extinguisher nearby (seriously!)
- [ ] Ventilation or fume extractor

### Testing Equipment
- [ ] Multimeter (continuity, voltage, current measurement)
- [ ] USB cable for ESP32 programming
- [ ] Computer with Arduino IDE or PlatformIO
- [ ] Test load (light bulb, 40-60W)
- [ ] Breadboard (for initial component testing)

## 🎓 SOLDERING BEST PRACTICES & COMMON MISTAKES

### Perfect Soldering Technique

**The 5-Step Method:**

```text
Step 1: Clean & Prepare
- Clean tip on brass wool
- Apply fresh solder to tip (tinning)
- Component leads should be clean and pre-tinned

Step 2: Heat the Joint
- Touch iron to BOTH pad and component lead
- Hold for 1-2 seconds to preheat
- Temperature: 350°C for lead-free

Step 3: Apply Solder
- Feed solder to the JOINT, not the iron
- Solder should flow smoothly around joint
- Use just enough to form concave fillet

Step 4: Remove & Inspect
- Remove solder first, then iron
- Let cool naturally (don't blow on it)
- Joint should be shiny and smooth

Step 5: Clean
- Remove flux residue with isopropyl alcohol
- Inspect under magnification
```

### Solder Joint Quality Visual Guide

```text
✓ GOOD JOINTS:              ✗ BAD JOINTS:

Shiny & Smooth              Dull (Cold Joint)
     ╱╲                          ╱══╲
  Pin││Pin                   Pin ████ Pin
     ││                          ████
  ═══╧╧═══                   ═════╧═════
      ▲                           ▲
   Perfect!                   REDO - Not heated enough

Concave Fillet              Too Much Solder
     ╱╲                          ╱══╲
  Pin││Pin                   Pin ███  Pin
    ╱  ╲                         ███
  ═══  ═══                   ════════════
      ▲                           ▲
   Perfect!                   Excess - Remove some

Good Coverage               Bridge (SHORT!)
   ╱╲                          ╱══╲
Pin││Pin                    Pin││  ││Pin
   ││                          ████████
═══╧╧═══                    ═══════════
    ▲                             ▲
 Perfect!                    DANGEROUS! Fix immediately

Smooth Cone                 Rosin Joint (No solder flow)
    ╱╲                           ╱╲
 Pin││Pin                     Pin  ╱╲  Pin
   ╱  ╲                           ▓▓▓▓
 ═══  ═══                    ═══════════
     ▲                             ▲
  Perfect!                    REDO - Flux not activated
```

### 10 Common Soldering Mistakes to Avoid

#### 1. Cold Solder Joint
**Problem**: Joint not heated enough, dull gray appearance  
**Cause**: Insufficient temperature or time  
**Fix**: Reheat joint properly (350°C, 2-3 seconds)  
**Prevention**: Preheat both pad and lead before applying solder

#### 2. Solder Bridges
**Problem**: Solder connecting adjacent pins  
**Cause**: Too much solder or shaky hand  
**Fix**: Use solder wick to remove excess, or drag technique with clean tip  
**Prevention**: Use less solder, work under magnification for fine pitch

#### 3. Overheating Components
**Problem**: Component damage from excessive heat  
**Cause**: Iron too hot or held too long  
**Fix**: Replace damaged component  
**Prevention**: 2-3 second rule, use heat sinks on sensitive parts

#### 4. Lifted Pads
**Problem**: PCB copper trace separated from board  
**Cause**: Too much force or heat  
**Fix**: Carefully solder wire directly to trace  
**Prevention**: Gentle pressure, proper desoldering technique

#### 5. Flux Residue
**Problem**: Sticky brown residue on board  
**Cause**: Not cleaning after soldering  
**Fix**: Clean with 99% isopropyl alcohol and soft brush  
**Prevention**: Clean immediately after soldering while still warm

#### 6. Wrong Solder Type
**Problem**: Poor joints with lead-free solder at low temp  
**Cause**: Using leaded solder temperature for lead-free  
**Fix**: Increase temperature to 350°C for lead-free  
**Prevention**: Check solder type, adjust temperature accordingly

#### 7. Disturbed Joint
**Problem**: Joint moved before cooling, cracked appearance  
**Cause**: Moving component while solder solidifies  
**Fix**: Reheat and let cool completely still  
**Prevention**: Hold still 3-5 seconds after removing iron

#### 8. Insufficient Solder
**Problem**: Weak mechanical connection  
**Cause**: Too little solder applied  
**Fix**: Add more solder to form proper fillet  
**Prevention**: Use enough solder to form small cone/fillet

#### 9. Oxidized Tip
**Problem**: Solder won't stick to iron tip  
**Cause**: Dirty or oxidized tip  
**Fix**: Clean on brass wool, re-tin with fresh solder  
**Prevention**: Keep tip tinned, clean frequently, use tip tinner

#### 10. Wrong Iron Temperature
**Problem**: Either cold joints or damage  
**Cause**: Temperature not calibrated  
**Fix**: Use temperature-controlled iron, test on scrap  
**Prevention**: 
- Lead-free: 350-370°C
- Leaded (60/40): 315-340°C
- Fine-pitch SMD: 315-330°C

### SOT23 & Fine-Pitch Soldering

**Special Techniques for Small Components:**

```text
TPS3823 SOT23-5 Package (3mm wide!)

Method 1: Tack & Drag
1. Apply flux to all pads
2. Tack pin 1 with tiny solder dot
3. Align chip while reheating pin 1
4. Add small amount of solder to iron tip
5. Drag across all pins with light pressure
6. Inspect for bridges, remove with wick if needed

Method 2: Individual Pin
1. Apply flux to all pads
2. Tack one corner pin
3. Check alignment
4. Solder each pin individually
5. Use fine tip (0.5mm)
6. Touch pad + pin simultaneously
7. Feed solder (very small amount)
8. Remove and let cool

Equipment needed:
- Fine tip (0.5mm conical)
- Good magnification (10x minimum)
- Steady hands (rest wrists on table)
- Good lighting
- Flux pen
- Fine solder (0.5mm diameter)
```

### I2C Pull-up Resistor Installation

```text
Critical for ATECC608A and 24C256!

        3.3V Rail
           │
      ┌────┴────┐
    [2.2kΩ]  [2.2kΩ]
      │         │
      │         │
    (SDA)     (SCL)

Installation Tips:
1. Use 1/4W through-hole resistors (easier than SMD)
2. Bend leads 90° at resistor body
3. Insert into breadboard:
   - One end to 3.3V rail
   - Other end to signal line
4. Solder both connections
5. Trim excess leads
6. Test: Measure resistance from SDA to 3.3V should be ~2.2kΩ
7. Test: Measure resistance from SCL to 3.3V should be ~2.2kΩ

Common mistakes:
✗ Forgetting pull-ups entirely (I2C won't work!)
✗ Using wrong value (too high = slow, too low = current waste)
✗ Only one resistor (both SDA and SCL need pull-ups)
✗ Pull-up to 5V instead of 3.3V (can damage ESP32!)
```

### Assembly Sequence Best Practices

**The Right Order Matters:**

```text
✓ Recommended Assembly Order:

1. ESP32-S3 base board (establishes reference)
2. Power distribution (3.3V and GND rails)
3. Add decoupling capacitors (100µF + 0.1µF)
4. I2C pull-ups (2.2kΩ resistors)
5. ATECC608A (test I2C communication before proceeding)
6. 24C256 EEPROM (test I2C, should see both devices)
7. Current sensor circuit (with burden resistor)
8. Relay control (optocoupler first, then relay)
9. Watchdog circuit (TPS3823 - requires fine soldering)
10. Final wiring and cable management

Why this order?
- Power first ensures everything has supply
- I2C devices next allows early testing
- Complex circuits last (easier to troubleshoot if issues)
- Each stage can be tested before moving on

✗ Don't do this:
- Soldering everything at once without testing
- Starting with the hardest part (SOT23)
- Forgetting decoupling capacitors until the end
- Adding power supply last
```

### Troubleshooting Guide

```text
Problem: ESP32 won't boot
→ Check 3.3V present (measure with multimeter)
→ Check no shorts: 3.3V to GND should be >50Ω
→ Check EN pin not pulled low
→ Remove other components one by one to isolate issue

Problem: I2C device not detected
→ Check pull-up resistors (2.2kΩ to 3.3V on SDA/SCL)
→ Check power to device (3.3V at VCC pin)
→ Check no solder bridges on I2C pins
→ Verify correct I2C address (ATECC608A: 0x60, 24C256: 0x50)
→ Try I2C scanner sketch

Problem: Solder won't stick
→ Clean iron tip on brass wool
→ Re-tin tip with fresh solder
→ Check temperature (should be 350°C for lead-free)
→ Apply flux to component and pad
→ Component leads might be oxidized (gently scrape/sand)

Problem: Keep making solder bridges
→ Use less solder (small amount goes long way)
→ Use finer solder (0.5mm instead of 1mm)
→ Use magnification
→ Clean tip before each joint
→ Practice drag technique on scrap board

Problem: Components getting damaged
→ Reduce iron temperature if components getting hot
→ Reduce time on joint (2-3 seconds max)
→ Use heatsink (clip to lead between joint and component)
→ Work faster with proper technique

Problem: Joints look dull/grainy
→ Cold joint - reheat properly
→ Ensure both pad and lead heated before adding solder
→ Check iron temperature
→ Use fresh solder (old solder oxidizes)
→ Apply flux
```

### Pre-Assembly Testing

**Test components BEFORE soldering:**

```bash
ESP32-S3:
1. Visual: Check for bent pins, clean pads
2. USB test: Connect to computer, COM port appears
3. Resistance: GND to 3.3V should be >100Ω

ATECC608A:
1. Visual: Check for damage, verify markings
2. Breadboard test: Wire up with jumpers
3. I2C scan: Should detect at address 0x60

SCT-013-030:
1. Visual: Check cable for damage
2. Resistance: Secondary ~30-40Ω (tip to sleeve)
3. Continuity: Primary should be OPEN (split core)

Relay Module:
1. Visual: Check for damage, clean contacts
2. Resistance: Coil should be 50-100Ω
3. Test: Apply 5V to coil, relay should click

Why pre-test?
- Much easier to test before soldering
- Prevents wasted time if component faulty
- Can return defective parts if new
- Identifies bent pins before they break
```

## 🛡️ Safety Checklist

### Before Starting
- [ ] Read through entire assembly plan
- [ ] Clear, well-lit workspace with no clutter
- [ ] No liquids near work area
- [ ] Ventilation for soldering fumes
- [ ] Know location of power off switch/breaker
- [ ] First aid kit accessible

### During Assembly
- [ ] **NEVER** work on AC connections while powered
- [ ] Double-check all connections before applying power
- [ ] Use proper wire gauge for current ratings
- [ ] Ensure proper isolation between AC and DC circuits
- [ ] Keep fingers away from AC terminals
- [ ] Take breaks to avoid fatigue mistakes

### After Assembly
- [ ] Visual inspection: no solder bridges, cold joints, exposed conductors
- [ ] Continuity test: verify no shorts between AC line and ground
- [ ] Resistance test: ensure no short between AC and DC sides
- [ ] Insulation test: verify isolation integrity

## 📋 Assembly Steps

### Phase 1: Component Testing (Breadboard) - Week 1

1. [ ] **Test ESP32-S3**: Upload blink sketch, verify USB communication, test hardware crypto
2. [ ] **Test ATECC608A**: Verify I2C communication (address 0x60), test key generation
3. [ ] **Test SCT-013**: Connect with 33Ω burden resistor, verify readings with known load
4. [ ] **Test MAX6316**: Verify watchdog functionality and reset behavior
5. [ ] **Test 24C256 EEPROM**: Verify I2C communication, test read/write operations
6. [ ] **Test relay**: Apply 5V to control pin via PC817, verify clicking and switching
7. [ ] **Test power supply**: Measure output voltage (should be 5V ±0.25V), check isolation

### Phase 2: Secure Provisioning - Week 1

1. [ ] **ATECC608A Configuration**:
   - Generate unique device ID
   - Generate device private key (ECDSA P256) in Slot 0
   - Write server public key to Slot 1
   - Configure key slots (Slot 0: never readable, Slot 1: always readable)
   - Lock configuration (IRREVERSIBLE - test thoroughly first!)

2. [ ] **ESP32-S3 Secure Boot**:
   - Generate RSA-3072 signing key (store securely!)
   - Enable secure boot v2
   - Flash bootloader with secure boot enabled
   - Verify bootloader signature verification

3. [ ] **ESP32-S3 Flash Encryption**:
   - Enable flash encryption in eFuse
   - Generate unique encryption key (hardware-generated)
   - Flash firmware with encryption enabled
   - Verify encrypted flash contents

### Phase 3: DC Side Assembly - Week 1-2

1. [ ] Solder power supply output to ESP32-S3 VIN/GND or 5V pin
2. [ ] Connect ATECC608A to ESP32-S3:
   - VCC → 3.3V
   - GND → GND
   - SDA → GPIO 21 (configurable)
   - SCL → GPIO 22 (configurable)
3. [ ] Connect 24C256 EEPROM to ESP32-S3:
   - VCC → 3.3V
   - GND → GND
   - SDA → GPIO 21 (shared I2C bus)
   - SCL → GPIO 22 (shared I2C bus)
   - Add 4.7kΩ pull-up resistors on SDA/SCL if needed
4. [ ] Connect MAX6316 watchdog:
   - VCC → 3.3V (battery backup optional)
   - GND → GND
   - OUT → ESP32 Reset pin
   - INPUT → Tamper detection circuit (enclosure switch)
5. [ ] Connect relay control via PC817 optocoupler:
   - PC817 LED anode → GPIO (e.g., GPIO 4) via 220Ω resistor
   - PC817 LED cathode → GND
   - PC817 collector → Relay VCC (5V)
   - PC817 emitter → Relay IN
   - Relay VCC → 5V
   - Relay GND → GND
6. [ ] Add decoupling capacitors (100nF ceramic + 10μF electrolytic) near ESP32-S3, ATECC608A, and 24C256

### Phase 4: AC Side Assembly (DANGER ZONE!) - Week 2

**⚠️ DISCONNECT FROM MAINS POWER BEFORE STARTING ⚠️**

1. [ ] Connect AC input to power supply input (L, N, Earth/Ground)
2. [ ] Install 3A quick-blow fuse in Line path
3. [ ] Install 275V MOV (varistor) between Line and Neutral (surge protection)
4. [ ] Connect AC line through SCT-013:
   - AC Line → SCT-013 primary (clamp around wire)
   - SCT-013 secondary → 33Ω burden resistor → GND
   - SCT-013 output → ESP32-S3 ADC pin (e.g., GPIO 1) via voltage divider
5. [ ] Connect relay to AC output:
   - AC Line (after fuse/MOV) → Relay Common (C)
   - Relay Normally Open (NO) → AC Output Line
6. [ ] Connect AC neutral and earth directly (bypass relay):
   - AC Neutral → AC Output Neutral
   - AC Earth/Ground → AC Output Earth/Ground
7. [ ] **Safety Verification**: 
   - No connection between AC and DC circuits except through isolated power supply
   - All AC connections in proper terminal blocks
   - No exposed conductors
   - Earth/ground properly connected

### Phase 5: Enclosure & Physical Security - Week 2

1. [ ] Mount all components in professional ABS enclosure
2. [ ] Install tamper detection switch on enclosure lid (connected to MAX6316 INPUT)
3. [ ] Use anti-tamper screws with security bits
4. [ ] Apply void-if-removed tamper-evident seals on enclosure screws
5. [ ] Label inputs and outputs clearly (AC IN, AC OUT, voltage warnings)
6. [ ] Ensure proper ventilation for power supply heat dissipation

### Phase 6: Testing & Validation - Week 2

**Start with low voltage/current testing**

1. [ ] **Visual inspection**: Check all connections, no exposed wires, proper insulation
2. [ ] **Continuity test**: Verify circuit paths match schematic
3. [ ] **Isolation test**: Use multimeter to confirm >1MΩ between AC and DC
4. [ ] **First power-on** (NO LOAD, AC disconnected):
   - Connect DC power only (5V from power supply)
   - ESP32-S3 should boot (check serial output)
   - Verify 5V and 3.3V rails
   - ATECC608A and 24C256 should respond on I2C bus
5. [ ] **AC power-on** (NO LOAD):
   - Connect to AC mains
   - Power supply should provide stable 5V
   - Check for any abnormal heating
6. [ ] **Relay test**: Control relay via GPIO, verify clicking and continuity change
7. [ ] **SCT-013 test**: Connect known load (40W incandescent bulb)
   - Verify current readings match expected values (I = P/V = 40W/230V ≈ 0.17A)
   - Calibrate if needed
8. [ ] **Tamper detection test**: Open enclosure, verify MAX6316 triggers reset
9. [ ] **Secure boot test**: Attempt to flash unsigned firmware (should fail)
10. [ ] **MQTT over TLS test**: Connect to MQTT broker over TLS 1.3, verify client cert auth
11. [ ] **Thermal test**: Run for 30 minutes with load, check for excessive heat
12. [ ] **Full load test** (if safe): Test with rated load (10A max), monitor temperature

### Phase 7: Firmware & Security Testing - Week 2

1. [ ] Flash production firmware (signed)
2. [ ] Configure Wi-Fi credentials (stored encrypted in NVS)
3. [ ] Test device attestation (firmware hash, secure boot status)
4. [ ] Test secure pairing (QR code + challenge-response)
5. [ ] Test signed commands (verify signature verification works)
6. [ ] Test tamper alert (physically tamper with device, verify alert sent)
7. [ ] Test OTA updates (signed firmware only)

## 📊 Data Collection & Calibration - Week 2

### Initial Testing Data
Record in lab notebook or spreadsheet:
- [ ] No-load voltage readings (AC: 230V nominal in SA, DC: 5V ±0.25V)
- [ ] Known load measurements:
  - 40W incandescent bulb (≈0.17A @ 230V)
  - 10W LED bulb (≈0.04A @ 230V)
  - 100W appliance (≈0.43A @ 230V)
  - Laptop charger (~60W, ≈0.26A @ 230V)
- [ ] SCT-013 readings vs. reference meter (calibration factor)
- [ ] Relay switching time (if measurable with oscilloscope)
- [ ] Power consumption of ESP32-S3 + circuitry (idle and active)
- [ ] Temperature measurements at various loads (power supply, relay, ESP32)
- [ ] ATECC608A key generation time (for performance benchmarks)
- [ ] Secure boot verification time
- [ ] MQTT over TLS connection time

### Calibration Data
- [ ] SCT-013 calibration values for ±1% accuracy
- [ ] Voltage measurement corrections (if using voltage divider)
- [ ] Current sense scaling factor
- [ ] Compare readings with commercial power meter (±2% reference)
- [ ] Temperature compensation factors (if needed)

### Security Validation Data
- [ ] Device attestation report (firmware version, hash, secure boot status)
- [ ] ATECC608A key slot configuration
- [ ] Certificate chain (device cert, intermediate, root CA)
- [ ] Tamper detection sensitivity (enclosure open/close cycles)
- [ ] Secure boot verification time
- [ ] Flash encryption overhead (performance impact)

## 🐛 Troubleshooting Common Issues

| Issue | Possible Cause | Solution |
|-------|---------------|----------|
| ESP32-S3 won't boot | No power or insufficient current | Check power supply output (5V), verify connections, check for shorts |
| ATECC608A not detected | I2C connection issue or wrong address | Verify SDA/SCL pins (GPIO 21/22), check pull-up resistors (4.7kΩ), try I2C scanner, verify address 0x60 |
| 24C256 EEPROM not detected | I2C connection issue | Same as ATECC608A, verify address 0x50-0x57 (depends on A0-A2 pins) |
| Relay won't switch | Control signal issue or bad relay | Verify GPIO output (3.3V), test relay independently with 5V, check PC817 optocoupler |
| Inaccurate SCT-013 readings | Calibration needed or wrong burden resistor | Verify 33Ω burden resistor, calibrate with known loads, check for electromagnetic interference |
| ESP32-S3 resets randomly | Power supply noise, brownout, or watchdog | Add decoupling caps (100nF + 10μF), check power supply stability, increase brownout threshold, check MAX6316 connection |
| Relay chattering | Electrical noise or mechanical issue | Add RC snubber (100Ω + 100nF) across relay contacts, check for loose connections |
| Secure boot fails | Wrong keys or corrupted bootloader | Re-flash bootloader, verify signing key, check eFuse configuration |
| Flash encryption fails | eFuse already blown or wrong config | Check eFuse status (`espefuse.py summary`), encryption is irreversible - test on separate board first! |
| ATECC608A config lock fails | Already locked or wrong configuration | Configuration lock is IRREVERSIBLE - verify config thoroughly before locking! |
| MQTT over TLS fails | Certificate issue or wrong CA | Verify CA cert, client cert, and key files, check TLS 1.3 support on broker |
| Tamper detection not working | MAX6316 connection or switch issue | Verify watchdog circuit, test switch continuity, check battery backup (if used) |

## 📸 Documentation

During assembly, capture:
- [ ] Photo of all components before starting (for inventory verification)
- [ ] Step-by-step assembly photos (especially AC wiring)
- [ ] Wiring diagram as actually built (may differ from plan - document changes!)
- [ ] Closeup of security chip connections (ATECC608A, 24C256, MAX6316)
- [ ] Closeup of any tricky connections (SCT-013, relay, optocoupler)
- [ ] Enclosure tamper detection switch installation
- [ ] First successful test with measurements (multimeter readings)
- [ ] Any issues encountered and how they were resolved
- [ ] Security provisioning steps (device ID, keys, certificates)
- [ ] Tamper-evident seal placement

## ✅ Final Checklist Before Use

- [ ] All connections secure and strain-relieved
- [ ] No exposed conductors on AC side
- [ ] Fuse (3A quick-blow) installed in Line path
- [ ] MOV (275V varistor) installed for surge protection
- [ ] Case closed and secured with anti-tamper screws
- [ ] Tamper-evident seals applied and documented
- [ ] Proper labeling (input, output, voltage warnings, device ID)
- [ ] ATECC608A provisioned and configuration locked
- [ ] Secure boot enabled and verified
- [ ] Flash encryption enabled and verified
- [ ] Firmware loaded and tested (signed version)
- [ ] Wi-Fi credentials stored encrypted in NVS
- [ ] Device registered in backend with certificate
- [ ] Tamper detection tested and verified
- [ ] Emergency shut-off plan in place
- [ ] Understand limitations (max current: 10A, max voltage: 250VAC)
- [ ] Safety documentation completed
- [ ] User manual for device operation

## 🚀 Post-Soldering Next Steps

1. **Firmware refinement** (Week 2-3): Load production firmware with full security features
   - MQTT over TLS 1.3 with client certificates
   - Signed command verification
   - Device attestation reporting
   - Tamper detection integration
   - Secure OTA updates with code signing
   
2. **Extended testing** (Week 3): 24-hour burn-in test with real appliances
   - Monitor for stability, heat, and security alerts
   - Log all telemetry data for analysis
   - Test tamper detection multiple times
   
3. **App integration** (Week 3-4): Connect mobile app to device for real-time data
   - Secure QR pairing with challenge-response
   - Real-time encrypted telemetry display
   - Manual control with signed commands
   - Tamper alert notifications
   
4. **Cloud integration** (Week 4-6): Deploy to secure cloud backend
   - MQTT broker over TLS 1.3
   - Field-level encryption for sensitive data
   - Audit logging for all device events
   
5. **Security audit** (Week 11-12): Independent security assessment
   - Penetration testing
   - Code review
   - Vulnerability assessment
   - Compliance verification (SABS, ICASA readiness)

6. **Documentation** (Ongoing): Update hardware docs with actual build notes
   - Bill of Materials (BOM) with exact part numbers
   - Assembly instructions with photos
   - Calibration procedures
   - Security configuration guide
   - Troubleshooting guide
   
7. **Iteration planning** (Week 12+): Identify improvements for v2 based on learnings
   - Custom PCB design (eliminate breadboard)
   - Cost optimization
   - Manufacturing process refinement
   - Additional security features
   - Certification preparation (SABS, ICASA)

---

**Phase 1 Budget Reminder**: R2,736 - R3,182 for 3 secure prototypes

**Remember**: Safety AND security first! If you're unsure about any step, especially AC connections or cryptographic operations, consult with someone experienced or hire a qualified professional. ATECC608A configuration lock and ESP32 flash encryption are IRREVERSIBLE - test thoroughly first!

**Security Contacts**:
- **Security Questions**: security@smartplugai.com
- **Hardware Issues**: hardware@smartplugai.com

**Last Updated**: December 2024  
**Version**: 2.0 (Security-Enhanced)  
**Next Update**: As Phase 1 progresses (Week 2, Week 4, Week 7)
