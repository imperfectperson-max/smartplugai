# Soldering Session Checklist

This checklist ensures you're prepared for the hardware assembly session and can safely build the Smart Plug AI secure prototype with bank-grade security features.

> 📍 **Roadmap Context**: This checklist supports the **Phase 1: Secure Prototype** stage (Months 9–12) of the [24-month Smart Plug AI roadmap](ROADMAP.md). The basic data collection plugs (Phase 0) used a simpler BOM without security hardware.

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
| **Relay Module** | 5V, 10A+ with optocoupler | 3 | R255 (R85 ea) | 🔴 CRITICAL | Must have JZC-S or SRD-05VDC-SL-C relay, PC817 optoisolation for safety. NEVER connect ESP32 GPIO directly to relay coil! |
| **AC-DC Converter** | 5V/2A phone charger (certified) | 3 | R150 (R50 ea) | 🔴 CRITICAL | MUST be safety-certified (CE/UL/TUV mark). Safer than bare HLK-PM01 for prototype. Min 1.5A output. |
| **Burden Resistor** | 33Ω, 1W for SCT-013 | 3 | R24 (R8 ea) | 🔴 CRITICAL | Most forgotten item! REQUIRED for SCT-013 calibration. Without this, current readings will be wrong/saturated. |
| **SA Plug + Socket** | New, professional grade | 3 sets | R270 (R90 ea) | 🔴 CRITICAL | Professional appearance, NOT salvaged |

### 🛡️ SAFETY & SECURITY COMPONENTS (NON-NEGOTIABLE)

| Item | Specification | Qty | Price (ZAR) | Priority | Notes |
|------|--------------|-----|-------------|----------|-------|
| **Fuse & Holder** | 3A quick-blow (IEC 60127) | 3 | R90 (R30 ea) | 🔴 CRITICAL | Safety protection for AC line - sized for 2A inrush + margin |
| **Varistor (MOV)** | 275V, 7mm diameter | 3 | R54 (R18 ea) | 🔴 CRITICAL | Surge protection, 275V = 1.2× SA grid nominal (230V) |
| **Optocouplers** | PC817 (extra isolation) | 6 | R48 (R8 ea) | 🔴 CRITICAL | Additional galvanic isolation for relay control - NEVER connect GPIO directly to relay! |
| **Tamper-Evident Seals** | Void-if-removed type | 10 | R30 | 🔴 CRITICAL | Physical security indicator on enclosure |

### Component Selection Rationale & Safety Notes

**Fuse (3A Quick-Blow):**
- Relay coil draws ~50-100mA continuous
- Load inrush current can reach 2A temporarily
- 3A provides safety margin while protecting wiring
- Quick-blow (not slow-blow) because DC side is low impedance
- MUST be installed in Line (hot) path, never in Neutral

**MOV Varistor (275V):**
- SA grid nominal: 230V AC RMS
- MOV clamping voltage: 275V = 1.2× nominal (protects against +20% overvoltage)
- 7mm diameter provides ~20A peak surge current capacity
- Lifetime: ~1000 joules per surge event
- **WARNING:** MOVs degrade with each surge. Replace every 2-3 years in areas with poor grid quality.

**Optocouplers (PC817):**
- **CRITICAL SAFETY:** Never connect ESP32 GPIO directly to relay coil (5V)!
- PC817 provides 5000V isolation between GPIO and relay circuit
- GPIO sources only 12mA → LED → optocoupler → relay control
- Protects ESP32 from relay back-EMF and voltage spikes
- **WARNING:** If optocoupler fails, relay won't switch. If bypassed, ESP32 can be destroyed.

### 🔧 BASIC ELECTRONICS KIT (SHARED ACROSS ALL 3 PLUGS)

| Item | Specification | Qty | Price (ZAR) | Priority | Notes |
|------|--------------|-----|-------------|----------|-------|
| **Breadboard** | 830 points | 1 | R75 | 🔴 CRITICAL | For prototyping before PCB |
| **Jumper Wires** | M/M, M/F mix | 1 kit | R75 | 🔴 CRITICAL | Assorted lengths |
| **Resistor Kit** | Assorted values (1Ω-1MΩ) | 1 | R65 | 🔴 CRITICAL | Includes pull-ups for I2C |
| **Capacitor Kit** | Assorted values (pF-μF) | 1 | R65 | 🔴 CRITICAL | For filtering and decoupling |
| **Diode Kit** | 1N4148 + 1N4007 | 1 | R25 | 🔴 CRITICAL | Signal and power diodes |
| **LED Kit** | RGB + current-limiting resistors | 1 | R35 | 🔴 CRITICAL | Status indicators |

### 🎨 PROJECTS DAY POLISH (WEEK 3)

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
- [ ] Safety glasses with side protection (ANSI Z87.1 rated)
- [ ] ESD wrist strap (REQUIRED for ATECC608A and ESP32-S3 handling)
- [ ] Heat-resistant mat or silicone work surface
- [ ] Fire extinguisher nearby (Class C electrical - seriously!)
- [ ] Ventilation or fume extractor (flux fumes are harmful)
- [ ] Closed-toe shoes (no sandals - protect against dropped soldering iron)
- [ ] Natural fiber clothing (avoid synthetics that can melt)
- [ ] Hair tied back if long (fire hazard near soldering iron)
- [ ] First aid kit with burn treatment supplies

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
- **Lead-free solder (Sn96.5Ag3Cu0.5):** 350-370°C
- **Leaded solder (Sn60Pb40):** 315-340°C  
- **Fine-pitch SMD (SOT23):** 330-350°C (lower to prevent pad lifting)
- **Through-hole components:** 350-380°C
- **Note:** If components are getting too hot (touch test), reduce temperature by 10-20°C
- **Tip:** Use flux to lower required temperature by improving heat transfer

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
    [2.2kΩ]  [2.2kΩ]  ← MUST be 2.2kΩ (NOT 4.7kΩ or 10kΩ)
      │         │
      │         │
    (SDA)     (SCL)

Why 2.2kΩ specifically?
- ESP32-S3 I2C high-level spec: 0.7 × VDD = 2.31V minimum
- 4.7kΩ causes excessive voltage drop at 100kHz I2C speed
- 2.2kΩ provides adequate pull-up while maintaining voltage levels
- Too low (<1kΩ) wastes power; too high (>4.7kΩ) causes signal integrity issues

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
✗ Using wrong value like 4.7kΩ or 10kΩ (too high = communication errors)
✗ Only one resistor (both SDA and SCL need pull-ups)
✗ Pull-up to 5V instead of 3.3V (can damage ESP32!)
✗ Using pull-down resistors instead of pull-up (won't work at all)
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

**Decision Tree: When to Stop and Seek Help**

```text
┌─────────────────────────────────────┐
│  Is smoke or burning smell present? │
└─────────────┬───────────────────────┘
              │
         Yes  │  No
              ↓
    ┌─────────────────┐
    │  STOP IMMEDIATELY│  ← Continue troubleshooting
    │  Disconnect Power│     below
    │  Use Extinguisher│
    │  Seek Help       │
    └──────────────────┘

┌──────────────────────────────────────┐
│ Did component get hot during testing?│
└─────────────┬────────────────────────┘
              │
         Yes  │  No
              ↓
    ┌─────────────────┐
    │ Too hot to touch?│  ← Continue troubleshooting
    └────┬─────────────┘
         │
    Yes  │  No (slightly warm OK)
         ↓
    ┌─────────────────┐
    │ STOP - Component│  ← Identify which component
    │ likely damaged  │  ← Check for shorts first
    │ or shorted      │  ← Replace if necessary
    └──────────────────┘

┌──────────────────────────────────────┐
│ AC/DC isolation test failed (<10MΩ)? │
└─────────────┬────────────────────────┘
              │
         Yes  │  No
              ↓
    ┌─────────────────┐
    │ STOP - Do NOT   │  ← Continue to next test
    │ power from mains│
    │ Find short first│
    └──────────────────┘

┌──────────────────────────────────────┐
│ Are you uncomfortable with any step? │
└─────────────┬────────────────────────┘
              │
         Yes  │  No
              ↓
    ┌─────────────────┐
    │ STOP - Ask for  │  ← Proceed carefully
    │ help. AC voltage│  ← Follow all safety checks
    │ can be lethal.  │
    └──────────────────┘
```

**Common Troubleshooting Scenarios:**

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

### Safety Standards & Compliance

**⚠️ This project involves mains voltage (230V AC) which can be lethal.**

**Relevant Safety Standards:**
- **SANS 60950-1** (South Africa): Safety of IT equipment including power supplies
- **SANS 164-2** (South Africa): Wiring regulations for low voltage installations
- **IEC 60950-1**: International safety standard for IT equipment
- **IEC 60335-1**: Safety of household electrical appliances

**Key Safety Requirements:**
- **Clearance**: Minimum 3mm between AC and DC traces/conductors
- **Creepage**: Minimum 4mm along PCB surface between AC and DC
- **Isolation**: Minimum 10MΩ resistance between AC and DC circuits
- **Wire Gauge**: Minimum 18 AWG (0.82mm²) for 3A, recommend 16 AWG (1.31mm²) for margin
- **Insulation Voltage**: All AC-side wire insulation must be rated ≥600V

**Wire Gauge Selection Guide:**

| Current Rating | Minimum AWG | Recommended AWG | Wire Diameter | Use Case |
|---------------|-------------|------------------|---------------|----------|
| 3A (Fused)    | 18 AWG      | 16 AWG          | 1.31mm²       | AC Line to relay |
| 10A (Load)    | 14 AWG      | 12 AWG          | 3.31mm²       | Relay to AC output |
| DC 5V (2A)    | 22 AWG      | 20 AWG          | 0.52mm²       | Power supply to ESP32 |
| GPIO/Signal   | 24 AWG      | 22 AWG          | 0.33mm²       | ESP32 to peripherals |

**Note:** These ratings assume ≤30cm wire runs and ≤30°C ambient temperature.

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
- [ ] **MANDATORY: AC/DC Isolation test (see detailed procedure below)**
- [ ] Insulation test: verify isolation integrity

#### Detailed AC/DC Isolation Testing Procedure

**⚠️ CRITICAL: This test MUST pass before connecting to mains power**

**Equipment needed:**
- Digital multimeter (auto-ranging, 10MΩ minimum range)
- Multimeter test leads (1000V rated minimum)
- Well-lit workspace
- Safety glasses

**Test Sequence (perform in order):**

**Step 1: 3.3V Rail Isolation**
1. Set multimeter to Resistance (Ohms) mode, highest range
2. Probe from 3.3V rail to DC GND: Should read >50Ω
   - If <50Ω: STOP. Short circuit on 3.3V rail. Find and fix.
3. Probe from 3.3V rail to AC Line: Should read >10MΩ (usually shows "OL" = open)
   - If <10MΩ: STOP. Insufficient isolation. Check for solder bridges or damaged insulation.
4. Probe from 3.3V rail to AC Neutral: Should read >10MΩ
   - If <10MΩ: STOP. Same as above.

**Step 2: 5V Rail Isolation**
1. Probe from 5V rail to DC GND: Should read >100Ω
   - If <100Ω: STOP. Check for short circuit on 5V rail.
2. Probe from 5V rail to AC Line: Should read >10MΩ
   - If <10MΩ: STOP. Isolation failure. Check power supply isolation barrier.
3. Probe from 5V rail to AC Neutral: Should read >10MΩ

**Step 3: GPIO Isolation (Relay Control)**
1. Probe from relay control GPIO (e.g., GPIO 4) to AC Line: Should read >10MΩ
   - If <10MΩ: STOP. Optocoupler isolation failure or wiring error.
2. Probe from relay control GPIO to AC Neutral: Should read >10MΩ

**Step 4: AC Side Verification**
1. Probe from AC Line to AC Neutral: Should read >1MΩ (high impedance when unpowered)
2. Probe from AC Line to Earth/Ground: Should read >1MΩ
3. Probe from AC Neutral to Earth/Ground: Should read <5Ω (bonded at distribution panel)
   - If >100Ω: WARNING. Earth connection may be poor. Verify wiring.

**Pass Criteria Summary:**
- All AC-to-DC measurements: >10MΩ
- DC rail to GND: >50Ω (3.3V) or >100Ω (5V)
- AC Neutral to Earth: <5Ω

**If ANY test fails:**
1. Disconnect all power immediately
2. Document which test failed and the measured value
3. Do NOT proceed to mains testing
4. Inspect for:
   - Solder bridges (especially near relay and optocoupler)
   - Damaged wire insulation
   - Incorrectly positioned components
   - Metal debris on PCB/breadboard
5. Fix the issue and re-test from Step 1

**Only proceed to mains power testing if ALL isolation tests pass.**

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

**⚠️ WARNING: Operations in this phase are IRREVERSIBLE. Test on spare hardware first!**

1. [ ] **ATECC608A Configuration**:
   - Generate unique device ID
   - Generate device private key (ECDSA P256) in Slot 0
   - Write server public key to Slot 1
   - Configure key slots (Slot 0: never readable, Slot 1: always readable)
   - **🔴 CRITICAL:** Lock configuration (IRREVERSIBLE - test thoroughly first!)
   - **⚠️ WARNING:** Once locked, configuration CANNOT be changed. Device is bricked if misconfigured.
   - **BEST PRACTICE:** Test entire provisioning flow on a spare ATECC608A before locking production devices.

2. [ ] **ESP32-S3 Secure Boot**:
   - Generate RSA-3072 signing key (store securely offline!)
   - Enable secure boot v2
   - Flash bootloader with secure boot enabled
   - Verify bootloader signature verification
   - **⚠️ WARNING:** Secure boot is enabled via eFuse which is one-time programmable (OTP).
   - **⚠️ CRITICAL:** Back up your signing key! If lost, you cannot update firmware.

3. [ ] **ESP32-S3 Flash Encryption**:
   - Enable flash encryption in eFuse
   - Generate unique encryption key (hardware-generated)
   - Flash firmware with encryption enabled
   - Verify encrypted flash contents
   - **⚠️ WARNING:** Flash encryption is enabled via eFuse (OTP - cannot be reversed).
   - **⚠️ CRITICAL:** Once enabled, you can only flash encrypted firmware. Cannot read back firmware.
   - **BEST PRACTICE:** Test on spare ESP32-S3 module first. Have multiple modules available.

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
   - Add 2.2kΩ pull-up resistors on SDA/SCL (REQUIRED, NOT optional)
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

**⚠️ MANDATORY PRE-MAINS-POWER SAFETY GATE ⚠️**

**Before connecting to 230V AC mains, EVERY item below MUST pass:**

#### Pre-Mains Safety Checklist (ALL MUST BE TRUE)

**Visual Inspection:**
- [ ] No solder bridges visible (inspect under magnification)
- [ ] All solder joints are shiny, not dull (no cold joints)
- [ ] All wires properly insulated with no exposed copper
- [ ] AC side wiring secured in proper terminal blocks
- [ ] Fuse holder contains correct 3A fuse
- [ ] Earth/ground connection is solid and secure
- [ ] No loose components or metal debris on board
- [ ] Enclosure has proper ventilation holes
- [ ] All components are correctly oriented (check IC pin 1, diode polarity)

**Resistance Testing (DC Power OFF, AC Disconnected):**
- [ ] 3.3V to DC GND: >50Ω (no short circuit)
- [ ] 5V to DC GND: >100Ω (no short circuit)
- [ ] AC Line to DC GND: >10MΩ (excellent isolation)
- [ ] AC Line to 3.3V: >10MΩ (excellent isolation)
- [ ] AC Neutral to Earth: <5Ω (proper earth bond)
- [ ] **If ANY isolation test <10MΩ: STOP. Find and fix the fault.**

**Component Response Testing (DC Power Only, AC Disconnected):**
- [ ] ESP32-S3 boots and enumerates on USB
- [ ] Serial console shows boot messages (no crashes)
- [ ] 3.3V rail measures 3.25V - 3.35V (stable)
- [ ] 5V rail measures 4.75V - 5.25V (stable)
- [ ] ATECC608A responds on I2C at address 0x60
- [ ] 24C256 EEPROM responds on I2C at address 0x50-0x57
- [ ] No abnormal heat from any component (touch test)
- [ ] No burning smell or visible smoke
- [ ] Relay clicks when GPIO toggled (via optocoupler)

**Safety Environment:**
- [ ] Fire extinguisher (Class C electrical) within arm's reach
- [ ] No other people in immediate work area (2m safety boundary)
- [ ] AC outlet is properly grounded (use outlet tester)
- [ ] Wearing safety glasses and closed-toe shoes
- [ ] Work surface is non-conductive and dry
- [ ] Have emergency power-off plan (know where breaker is)
- [ ] Mobile phone available to call for help if needed

**Documentation & Knowledge:**
- [ ] Familiar with AC electrical safety procedures
- [ ] Know what to do if smoke/fire occurs (disconnect power, use extinguisher)
- [ ] Have device specifications and schematic available
- [ ] Have documented all previous test results
- [ ] Second person aware you're doing mains testing (best practice)

**🔴 CRITICAL: If ANY item above fails, DO NOT proceed to AC testing.**

**Fix all issues, retest, and only proceed when 100% of checks pass.**

---

#### AC Power Testing Procedure (Only After Passing All Above)

**Start with low voltage/current testing**

1. [ ] **Visual inspection**: Check all connections, no exposed wires, proper insulation
2. [ ] **Continuity test**: Verify circuit paths match schematic
3. [ ] **Isolation test**: Confirm >10MΩ between AC and DC (from checklist above)
4. [ ] **First power-on** (NO LOAD, AC disconnected):
   - Connect DC power only (5V from power supply)
   - ESP32-S3 should boot (check serial output)
   - Verify 5V and 3.3V rails
   - ATECC608A and 24C256 should respond on I2C bus
5. [ ] **AC power-on** (NO LOAD):
   - **Stand back during first connection**
   - Connect to AC mains with fuse installed
   - Power supply should provide stable 5V
   - Check for any abnormal heating (wait 5 minutes)
   - Check for any unusual sounds or smells
   - Monitor voltage stability on oscilloscope if available
6. [ ] **Relay test**: Control relay via GPIO, verify clicking and continuity change
7. [ ] **SCT-013 test**: Connect known load (40W incandescent bulb)
   - Verify current readings match expected values (I = P/V = 40W/230V ≈ 0.17A)
   - Calibrate if needed (±10% accuracy is acceptable)
8. [ ] **Tamper detection test**: Open enclosure, verify MAX6316 triggers reset
9. [ ] **Secure boot test**: Attempt to flash unsigned firmware (should fail)
10. [ ] **MQTT over TLS test**: Connect to MQTT broker over TLS 1.3, verify client cert auth
11. [ ] **Thermal test**: Run for 30 minutes with load, check for excessive heat
    - Touch relay case: Should be warm but not too hot to touch (< 60°C)
    - Touch power supply: Should be warm (< 50°C)
    - Touch ESP32: Should be slightly warm (< 45°C)
12. [ ] **Full load test** (if safe): Test with rated load (10A max), monitor temperature
    - Use proper test load (resistive, not motor)
    - Monitor continuously for first 10 minutes
    - If any component >70°C, reduce load or improve ventilation

## 🌡️ Thermal Management & Component Temperatures

### Safe Operating Temperatures

**Component Temperature Limits:**

| Component | Normal Operating | Warning Level | Critical (Shut Down) | Notes |
|-----------|-----------------|---------------|---------------------|-------|
| ESP32-S3 | 30-45°C | 60°C | 85°C | Higher temps reduce WiFi performance |
| ATECC608A | 25-40°C | 70°C | 85°C | Cryptographic operations generate heat |
| Relay Coil | 40-60°C | 75°C | 85°C | Coil resistance increases with temperature |
| Relay Contacts | 30-50°C | 70°C | 100°C | High current causes contact heating |
| Power Supply | 35-50°C | 65°C | 80°C | Most critical component for thermal management |
| PC817 Optocoupler | 25-40°C | 70°C | 85°C | LED generates heat at high current |
| SCT-013 Sensor | 25-35°C | 50°C | 60°C | Accuracy degrades above 40°C |
| 33Ω Burden Resistor | 30-45°C | 70°C | 100°C | 1W rating, but cooler is better |

**Temperature Measurement:**
- **Touch test**: Warm is OK, too hot to hold (>60°C) needs attention
- **IR thermometer**: Most accurate non-contact method (±2°C)
- **Thermocouple**: Most accurate contact measurement (±1°C)

### Ventilation Requirements

**Enclosure must have adequate ventilation holes:**
- **Minimum vent area**: 10-15% of enclosure surface
- **Hole size**: 3-5mm diameter
- **Hole spacing**: 10-15mm between holes
- **Positioning**: Bottom intake (cool air), top/side exhaust (hot air)
- **Protection**: Fine mesh to prevent debris/insects

**Power Dissipation:** ~2.5W peak per plug (power supply generates most heat)

**Thermal Rise:** With proper ventilation, expect +15°C above ambient. Without ventilation, can reach +30°C or more (UNACCEPTABLE).

### Thermal Troubleshooting

| Symptom | Likely Cause | Solution |
|---------|--------------|----------|
| Power supply very hot (>65°C) | Overload or poor ventilation | Reduce load, add ventilation, check for shorts |
| Relay coil very hot (>75°C) | Continuous high-duty operation | Normal for relays, ensure proper ventilation |
| ESP32 very hot (>60°C) | Heavy WiFi usage or CPU load | Reduce update frequency, optimize code |
| Entire enclosure hot | Insufficient ventilation | Add more vent holes, improve airflow |
| One spot very hot (>80°C) | Short circuit or component failure | STOP immediately, disconnect power, inspect |

### 24-Hour Burn-In Test

**Protocol:**
1. **Hours 0-1**: Light load (40W) - record temps every 15 min
2. **Hours 1-2**: Medium load (100W) - monitor for temp rise
3. **Hours 2-4**: Heavy load (10A) - continuous monitoring
4. **Hours 4-24**: Cycling load (30 min on/off)

**Acceptance Criteria:**
- ✓ No component exceeds warning temperature
- ✓ Temperatures stabilize within 30 minutes
- ✓ No burning smell or discoloration
- ✓ All measurements remain within ±5% throughout test

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
   
3. **App integration** (Week 3): Connect mobile app to device for real-time data
   - Secure QR pairing with challenge-response
   - Real-time encrypted telemetry display
   - Manual control with signed commands
   - Tamper alert notifications
   
4. **Cloud integration** (Week 3-4): Deploy to secure cloud backend
   - MQTT broker over TLS 1.3
   - Field-level encryption for sensitive data
   - Audit logging for all device events
   
5. **Security audit** (Week 6): Independent security assessment
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
   
7. **Iteration planning** (Week 7+): Identify improvements for v2 based on learnings
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
- **Emergency Safety Issues**: Use GitHub issues with `safety` label for urgent matters

**Safety Compliance References**:
- **SANS 60950-1**: Safety of IT equipment including power supplies
- **SANS 164-2**: Wiring regulations for low voltage installations in South Africa
- **IEC 60950-1**: International safety standard for IT equipment
- **IEC 60335-1**: Safety of household electrical appliances

**Last Updated**: February 2025 (Enhanced safety procedures and component specifications)  
**Version**: 2.2 - Major safety improvements:
- Added mandatory pre-mains-power safety checklist with pass/fail criteria
- Standardized I2C pull-up resistors to 2.2kΩ with detailed reasoning
- Enhanced AC/DC isolation testing procedure (>10MΩ requirement)
- Added comprehensive PPE requirements and safety standards references
- Added thermal management section with component temperature limits
- Expanded component-specific warnings (eFuse, ATECC608A lock, optocoupler)
- Added wire gauge selection guide with current ratings
- Added troubleshooting decision tree for critical safety decisions

**Next Update**: As Phase 1 progresses (Week 2, Week 4, Week 7)
