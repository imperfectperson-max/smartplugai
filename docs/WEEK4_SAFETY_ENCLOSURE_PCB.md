# WEEK 4: SAFETY, ENCLOSURE & PCB DESIGN - DEEP DIVE

📅 **Timeline**: Days 22-28  
🎯 **Goal**: Transform breadboard prototypes into professional, safe, enclosed smart plugs with PCB design ready for manufacturing

## 📅 DAY 22-23: PCB DESIGN WITH SECURITY ZONES

### MORNING (9AM-12PM): Circuit Schematic Design

#### Step 1: Select PCB Design Software

```text
Checklist - Software Setup:
[√] Install KiCad 7.0+ (free, open-source)
[√] Install component libraries for ESP32-S3, ATECC608A
[√] Install custom footprints for SCT-013, MAX6316
[√] Set up design rules for 2-layer PCB
[√] Configure grid spacing (0.1" for through-hole, 0.05mm for SMD)
```

**Recommended PCB Software Options:**
```
Option 1: KiCad (Free, Open Source)
├─ Pros: Professional-grade, excellent community
├─ Cons: Learning curve
└─ Best for: Production-ready designs

Option 2: EasyEDA (Free, Web-based)
├─ Pros: Easy to learn, JLCPCB integration
├─ Cons: Less features than KiCad
└─ Best for: Quick prototypes

Option 3: Eagle (Fusion 360)
├─ Pros: Industry standard
├─ Cons: Paid license
└─ Best for: Professional designs
```

#### Step 2: Create Schematic with Isolated Zones

**File**: `hardware/schematics/smartplug_v1.kicad_sch`

**Schematic Block Diagram:**
```
┌─────────────────────────────────────────────────────────────┐
│                    SMART PLUG PCB ARCHITECTURE              │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  ┌──────────────────────────────────────────────────────┐  │
│  │          MAINS AC INPUT (Isolated Zone 1)            │  │
│  │                                                        │  │
│  │  Wall Socket → Fuse (3A) → MOV (275V) → Relay Input  │  │
│  │                                                        │  │
│  └────────────────────┬───────────────────────────────────┘  │
│                       │ Isolation Barrier                    │
│                       ↓ (3mm clearance)                      │
│  ┌──────────────────────────────────────────────────────┐  │
│  │          DC LOW VOLTAGE (Isolated Zone 2)            │  │
│  │                                                        │  │
│  │  5V Power → ESP32-S3 → ATECC608A → MAX6316          │  │
│  │                ↓                                       │  │
│  │           Relay Control                               │  │
│  │           (via PC817 optocoupler)                     │  │
│  │                                                        │  │
│  └──────────────────────────────────────────────────────┘  │
│                                                              │
│  ┌──────────────────────────────────────────────────────┐  │
│  │          MEASUREMENT ZONE (Isolated Zone 3)          │  │
│  │                                                        │  │
│  │  Load Current → SCT-013 (isolated) → Burden Resistor │  │
│  │                                      ↓                 │  │
│  │                                  ESP32 ADC             │  │
│  │                                                        │  │
│  └──────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
```

**Detailed Power Section Schematic:**
```
MAINS INPUT SECTION (AC 230V):
═══════════════════════════════════════

Wall Plug (L, N, E)
    │
    ├─── Line (L) ──┬─── [F1: 3A Fuse] ──┬─── [MOV1: 275V] ──┐
    │               │                     │                   │
    │               │                     └─ to GND ─────────┤
    │               │                                         │
    │               └─── [Relay Contact] ─────────────────────┼─── LOAD OUT
    │                    (10A rated)                          │
    │                                                         │
    ├─── Neutral (N) ────────────────────────────────────────┼─── LOAD OUT
    │                                                         │
    └─── Earth (E) ──────────────────────────────────────────┴─── LOAD OUT

Relay Control:
    ESP32 GPIO → [R1: 1kΩ] → PC817 LED → GND
                              PC817 Collector → Relay Coil (+)
                              PC817 Emitter → GND
                              Relay Coil (+) → 5V
                              [D1: Flyback Diode across coil]
```

**ESP32-S3 Core Section:**
```
MICROCONTROLLER SECTION (3.3V Logic):
═══════════════════════════════════════

5V Input (from phone charger)
    │
    └─── [LM1117-3.3] ──┬─── 3.3V Rail ──┬─── ESP32-S3 VDD
         (Voltage Reg)  │                 │
                        ├─── [C1: 10µF]  ├─── ATECC608A VCC
                        │    [C2: 100nF] │
                        │                 ├─── MAX6316 VDD
                        │                 │
                        └─── to GND ──────┴─── 24C256 VCC

ESP32-S3 Connections:
    GPIO4  → Relay Control (via PC817)
    GPIO5  → SCT-013 ADC Input (via burden resistor)
    GPIO21 → I2C SDA (ATECC608A, 24C256)
    GPIO22 → I2C SCL (ATECC608A, 24C256)
    GPIO18 → MAX6316 WDI (Watchdog Input)
    GPIO19 → MAX6316 RESET_OUT (Tamper Alert)
    EN     → MAX6316 WDO (Watchdog Output for MCU reset)
    GPIO0  → Boot Button (10kΩ pull-up)
    GPIO46 → Reset Button (10kΩ pull-up)
```

**ATECC608A Secure Element:**
```
SECURITY CHIP SECTION:
═══════════════════════

ATECC608A-TNGTLS (I2C Address: 0xC0)
    Pin 1 (NC)     → Not Connected
    Pin 2 (NC)     → Not Connected
    Pin 3 (NC)     → Not Connected
    Pin 4 (GND)    → Ground
    Pin 5 (SDA)    → ESP32 GPIO21 (with 4.7kΩ pull-up to 3.3V)
    Pin 6 (SCL)    → ESP32 GPIO22 (with 4.7kΩ pull-up to 3.3V)
    Pin 7 (NC)     → Not Connected
    Pin 8 (VCC)    → 3.3V (with 100nF bypass capacitor)

Note: ATECC608A stores:
- Device private key (ECDSA P-256)
- Device certificate
- Root CA certificate
- Configuration locks (set during provisioning)
```

**Current Sensor Integration:**
```
SCT-013-030 CURRENT SENSOR:
═══════════════════════════════

Load Wire (Hot) passes through SCT-013 clamp
    │
    └─── SCT-013 Output (3.5mm jack)
              ├─── Tip → [R_burden: 33Ω, 1W] → ESP32 GPIO5 (ADC)
              │                               └─── [C_filter: 10µF] → GND
              └─── Sleeve → GND

ADC Protection:
    ESP32 GPIO5 ← [R_series: 10kΩ] ← [Zener 3.3V] → GND
                  (Overvoltage protection)

Calibration Formula:
    Current (A) = (ADC_Reading * 3.3V / 4095) / (33Ω * SCT_ratio)
    SCT_ratio = 1800 turns (30A → 16.67mV per Amp with 33Ω)
```

### AFTERNOON (1PM-5PM): PCB Layout Design

#### Step 3: Component Placement with Security Zones

**PCB Layer Stack (2-layer board):**
```
┌────────────────────────────────────────┐
│  Top Layer (Component Side)            │
│  - All components                      │
│  - Signal traces                       │
│  - Ground pours                        │
├────────────────────────────────────────┤
│  Bottom Layer (Solder Side)            │
│  - Ground plane (95% coverage)         │
│  - Power traces (5V, 3.3V)            │
│  - Return paths                        │
└────────────────────────────────────────┘
```

**Component Placement Strategy:**
```
┌─────────────────────────────────────────────────────┐
│                  PCB LAYOUT (Top View)              │
│         100mm x 80mm (fits in enclosure)           │
├─────────────────────────────────────────────────────┤
│                                                      │
│  [ZONE 1: HIGH VOLTAGE] (Top Left - Red Zone)      │
│  ┌───────────────────────────────────────┐         │
│  │ AC Input   Fuse    MOV    Relay       │         │
│  │  Screw     Holder  275V   Module      │         │
│  │  Terminal                             │         │
│  │                                       │         │
│  │  ⚠️ DANGER: 230V AC - 3mm clearance  │         │
│  └───────────────────────────────────────┘         │
│                                                      │
│  ───────────────────────────────────────────────   │
│          3mm Isolation Gap (No Traces)              │
│  ───────────────────────────────────────────────   │
│                                                      │
│  [ZONE 2: LOW VOLTAGE] (Center - Green Zone)       │
│  ┌───────────────────────────────────────┐         │
│  │ 5V Input   LM1117   ESP32-S3          │         │
│  │ (Phone     Regulator WROOM-1-N8       │         │
│  │  Charger)                              │         │
│  │            ATECC608A   MAX6316         │         │
│  │  PC817     24C256      EEPROM          │         │
│  │  Opto      I2C Secure  Tamper WD       │         │
│  │                                       │         │
│  │  Programming   Reset                  │         │
│  │  Header        Button                 │         │
│  └───────────────────────────────────────┘         │
│                                                      │
│  [ZONE 3: MEASUREMENT] (Bottom - Blue Zone)        │
│  ┌───────────────────────────────────────┐         │
│  │ SCT-013    Burden     ADC              │         │
│  │ Input      Resistor   Protection       │         │
│  │ (3.5mm     33Ω, 1W    Zener            │         │
│  │  Jack)                                 │         │
│  └───────────────────────────────────────┘         │
│                                                      │
│  [MOUNTING HOLES] (4 corners - M3 size)            │
│   ●                                      ●          │
│                                                      │
│   ●                                      ●          │
└─────────────────────────────────────────────────────┘
```

#### Step 4: Design Rules & Manufacturing Specs

**PCB Manufacturing Specifications:**
```yaml
Board Specs:
  Dimensions: 100mm x 80mm
  Layers: 2 (Top + Bottom)
  Material: FR4 (1.6mm thickness)
  Copper Weight: 1oz (35µm) both sides
  Surface Finish: HASL (Lead-Free) or ENIG
  Solder Mask: Green (both sides)
  Silkscreen: White (both sides)

Design Rules:
  Minimum Trace Width: 0.25mm (signal), 0.5mm (power), 1.0mm (mains AC)
  Minimum Spacing: 0.25mm (signal), 3.0mm (mains to low voltage)
  Via Size: 0.8mm drill, 1.2mm pad
  Minimum Hole Size: 0.3mm
  Edge Clearance: 2mm from board edge

Safety Clearances (IEC 60950-1):
  Mains AC traces: 1.0mm wide minimum
  Mains to Ground: 3.0mm clearance
  Primary to Secondary: 3.0mm clearance (isolation barrier)
  Creepage Distance: 4.0mm (reinforced insulation)
```

**Trace Width Calculator:**
```
For 1oz copper (35µm thick):

3.3V Rail (500mA max):
  Trace Width: 0.4mm
  Temperature Rise: 10°C

5V Rail (1A max):
  Trace Width: 0.8mm
  Temperature Rise: 10°C

Mains AC (10A max):
  Trace Width: 2.0mm minimum
  Temperature Rise: 20°C

Use online calculator: https://www.4pcb.com/trace-width-calculator.html
```

### EVENING (5PM-8PM): Generate Manufacturing Files

#### Step 5: Export Gerber Files

```bash
# In KiCad, go to: File → Fabrication Outputs → Gerbers

Generate the following files:
1. F.Cu (Front Copper Layer)
2. B.Cu (Back Copper Layer)
3. F.SilkS (Front Silkscreen)
4. B.SilkS (Back Silkscreen)
5. F.Mask (Front Solder Mask)
6. B.Mask (Back Solder Mask)
7. Edge.Cuts (Board Outline)
8. PTH.drl (Plated Through-Holes drill file)
9. NPTH.drl (Non-Plated Through-Holes drill file)

# Zip all files together
cd gerbers/
zip smartplug_v1_gerbers.zip *.gbr *.drl

# Upload to JLCPCB or PCBWay for quote
# Expected cost: R150-R300 for 5 boards (2-3 day shipping)
```

**PCB Manufacturer Options (South Africa):**
```
Option 1: JLCPCB (China → SA)
├─ Cost: R200 for 5 boards + R150 shipping
├─ Turnaround: 7-10 days
├─ Quality: Excellent
└─ Website: jlcpcb.com

Option 2: PCBWay (China → SA)
├─ Cost: R250 for 5 boards + R180 shipping
├─ Turnaround: 7-14 days
├─ Quality: Excellent
└─ Website: pcbway.com

Option 3: Local PCB (Johannesburg)
├─ Cost: R800 for 5 boards
├─ Turnaround: 5-7 days
├─ Quality: Good
└─ Support: Local, face-to-face

⚠️ Note: Add 2 weeks buffer for delivery and potential customs delays
```

---

## 📅 DAY 24-25: 3D ENCLOSURE DESIGN & PRINTING

### MORNING (9AM-12PM): Enclosure Design in CAD

#### Step 1: Measure PCB and Components

```text
Checklist - Critical Measurements:
[√] PCB dimensions: 100mm x 80mm x 1.6mm
[√] Highest component (ESP32-S3): 3mm above PCB
[√] Relay module height: 15mm above PCB
[√] SCT-013 cable entry point: 10mm diameter
[√] Power input cable: 6mm diameter (phone charger cable)
[√] Programming header clearance: 5mm
[√] Ventilation requirements: 20% surface area
[√] Mounting hole positions: M3 screws, 4 corners
```

**Enclosure Design Requirements:**
```
┌─────────────────────────────────────────────────────┐
│          ENCLOSURE SPECIFICATIONS                    │
├─────────────────────────────────────────────────────┤
│                                                      │
│  Internal Dimensions:                               │
│  - Length: 110mm (PCB + 5mm each side)             │
│  - Width: 90mm (PCB + 5mm each side)               │
│  - Height: 30mm (components + 10mm clearance)      │
│                                                      │
│  Wall Thickness: 3mm (ABS plastic)                  │
│                                                      │
│  Features:                                          │
│  - Top/Bottom halves with snap-fit clips           │
│  - 4x M3 PCB mounting posts (heat-set inserts)     │
│  - Cable entry grommets (2x 10mm holes)            │
│  - Ventilation slots (3x 50mm x 3mm on sides)     │
│  - LED light pipes (status indicators)             │
│  - Tamper-evident screw holes (security bits)      │
│  - Anti-slip rubber feet (4 corners)               │
│                                                      │
│  Labels:                                            │
│  - Logo area (20mm x 20mm)                         │
│  - QR code area (25mm x 25mm for device pairing)   │
│  - Safety warnings (SABS compliance labels)         │
│  - Rating plate area (230V, 10A, 2300W)            │
│                                                      │
└─────────────────────────────────────────────────────┘
```

#### Step 2: Create 3D Model in Fusion 360 or FreeCAD

**File**: `hardware/enclosure/smartplug_enclosure_v1.f3d`

**Enclosure Assembly Diagram:**
```
                Top View of Bottom Half
        ┌───────────────────────────────────┐
        │                                   │
        │  ┌─────────────────────────────┐ │
        │  │                             │ │
        │  │      PCB Mounting Posts     │ │
        │  │      (4x M3 heat-set)       │ │
        │  │                             │ │
        │  │    ●                    ●   │ │
        │  │                             │ │
        │  │                             │ │
        │  │                             │ │
        │  │    ●                    ●   │ │
        │  │                             │ │
        │  └─────────────────────────────┘ │
        │                                   │
        │  [Cable Entry]      [Vent Slots] │
        │      ○○                  ≡≡≡      │
        │                                   │
        └───────────────────────────────────┘

                Side View (Cross-section)
        ┌─────────────────────────────────────┐
        │  Top Half                           │ ← Snap-fit clips
        │  ┌───────────────────────────────┐ │
        │  │  ~~~~~~~~~ (ventilation)      │ │
        │  └───────────────────────────────┘ │
        ├─────────────────────────────────────┤
        │  Components                         │
        │  ╔═══════════════════════════════╗ │
        │  ║ ESP32  ATECC608A  Relay       ║ │ ← PCB
        │  ╚═══════════════════════════════╝ │
        ├─────────────────────────────────────┤
        │  Bottom Half                        │
        │  │ │ │ │ (mounting posts)         │ │
        │  [○] [○] [○] [○] (rubber feet)    │ │
        └─────────────────────────────────────┘
```

**Design Features in Detail:**

1. **Snap-Fit Clips (6 total):**
```
Clip Design:
  Base Width: 8mm
  Clip Height: 2mm
  Engagement: 1.5mm
  Angle: 45° entry chamfer
  
Position: 2 on each long side, 1 on each short side
```

2. **Tamper-Evident Features:**
```
Security Screws:
  Type: Torx with center pin (T10H)
  Quantity: 4 (one per corner)
  Purpose: Requires special driver to open
  
Tamper Seals:
  Type: Holographic "VOID IF REMOVED" stickers
  Size: 10mm diameter
  Position: Over each screw
  
MAX6316 Integration:
  Micro-switch on lid hinge
  Triggers GPIO19 on ESP32 when opened
  Logs tamper event with timestamp to EEPROM
```

3. **Ventilation Design:**
```
Total Vent Area: 15cm² (20% of surface area)

Slot Configuration:
  6 slots total (3 per side)
  Each slot: 50mm x 3mm x 3mm deep
  Angled at 45° (prevents dust/water)
  
Airflow Path:
  Cold air in (bottom vents)
    ↓
  Heat rises from relay/regulator
    ↓
  Hot air out (top vents)
```

### AFTERNOON (1PM-5PM): 3D Printing & Post-Processing

#### Step 3: Prepare for 3D Printing

```bash
# Export STL files from CAD software
File → Export → STL (Binary)

# Files to generate:
1. enclosure_bottom.stl
2. enclosure_top.stl
3. cable_grommet.stl (flexible TPU)
4. light_pipe.stl (clear PETG)

# Slice in PrusaSlicer or Cura
Settings for ABS:
  Layer Height: 0.2mm
  Wall Thickness: 3mm (12 perimeters)
  Infill: 30% (gyroid pattern)
  Supports: Only for screw posts
  Bed Adhesion: Brim (5mm)
  Print Temperature: 240°C
  Bed Temperature: 100°C
  Print Time: ~8 hours per half
```

**3D Printing Tips:**
```
Material Selection:
  
ABS (Recommended):
  ✓ Heat resistant (up to 80°C)
  ✓ Strong and durable
  ✓ Good for electrical enclosures
  ✗ Requires enclosed printer (warping)
  ✗ Fumes (ventilation needed)

PETG (Alternative):
  ✓ Easy to print
  ✓ Good strength
  ✓ Slight flexibility
  ✗ Lower heat resistance (70°C max)

PLA (NOT Recommended):
  ✗ Low heat resistance (60°C)
  ✗ Brittle
  ✗ Not suitable for electrical devices
```

#### Step 4: Post-Processing

```text
Checklist - Finishing Steps:
[√] Remove support material with pliers
[√] Sand visible layer lines (220 grit → 400 grit)
[√] Install M3 heat-set inserts (8 total: 4 for PCB, 4 for case screws)
[√] Apply acetone vapor smoothing (ABS only, optional)
[√] Drill out cable entry holes to exact size (10mm)
[√] Test fit PCB and all components
[√] Clean with isopropyl alcohol
[√] Apply anti-slip rubber feet (self-adhesive)
```

**Heat-Set Insert Installation:**
```
Tools Needed:
  - Soldering iron (set to 200°C)
  - M3 brass heat-set inserts (8x)
  - Needle-nose pliers

Steps:
1. Place insert in hole (printed hole should be 4.5mm for M3)
2. Apply soldering iron tip to top of insert
3. Gently push insert into plastic (2-3mm deep)
4. Hold for 5 seconds until plastic cools
5. Test with M3 screw (should thread smoothly)

⚠️ Common Mistakes:
  - Too hot = melted plastic bulges out
  - Too cold = insert doesn't seat properly
  - Uneven pressure = insert goes in crooked
```

---

## 📅 DAY 26-27: MAINS WIRING & SAFETY TESTING

### MORNING (9AM-12PM): High Voltage Wiring ⚡

#### ⚠️ SAFETY FIRST ⚠️

```text
MANDATORY SAFETY PRECAUTIONS:
[√] Disconnect from mains power at ALL times during assembly
[√] Use insulated tools (1000V rated)
[√] Double-check all connections before powering on
[√] Use multimeter to verify NO continuity between AC and DC sides
[√] Install fuse LAST (after all wiring complete)
[√] Test with low voltage first (12V DC) to verify relay operation
[√] Have fire extinguisher nearby (Class C for electrical)
[√] Work with a partner who can cut power in emergency
[√] Wear safety glasses
[√] Use insulated work surface (rubber mat)
```

#### Step 1: Cable Preparation

**Required Cables:**
```
1. Mains Input Cable:
   - Type: 3-core H05VV-F (0.75mm²)
   - Length: 1.5m
   - Colors: Brown (L), Blue (N), Green/Yellow (E)
   - Rating: 230V, 10A

2. Internal Wiring:
   - Type: 18 AWG stranded wire
   - Colors: Red (5V), Black (GND), others for signals
   - Rating: 300V, 2A
   
3. Load Output (to SA socket):
   - Type: Same as input (0.75mm²)
   - Length: 15cm (internal to enclosure)
   - Strain relief required
```

**Cable Stripping & Preparation:**
```bash
# Strip lengths:
Mains cable outer sheath: 50mm
Individual wire insulation: 8mm (for screw terminals)
Internal wires: 5mm (for soldering)

# Tinning wire ends:
1. Strip insulation to expose copper
2. Apply flux to bare copper
3. Heat with soldering iron
4. Apply small amount of solder
5. Result: Smooth, shiny surface (prevents oxidation)
```

#### Step 2: Fuse and MOV Protection

**Fuse Installation:**
```
┌────────────────────────────────────┐
│     FUSE HOLDER CONNECTION         │
├────────────────────────────────────┤
│                                    │
│  Wall Plug   [Fuse Holder]  Relay │
│    Line  ─────●───[3A]───●──── → │
│      ↑         ↑                   │
│      │         └─ Fuse (3A Fast)  │
│      │                             │
│  Purpose:                          │
│  - Protects against overcurrent    │
│  - Melts if load exceeds 3A        │
│  - Must be accessible for replace  │
│                                    │
└────────────────────────────────────┘

Fuse Selection:
  Type: Glass tube, 3A, Fast-blow (F3A)
  Voltage: 250V AC
  Size: 5x20mm
  Brands: Littelfuse, Bussman
  Cost: R5 each (buy 10 spares)
```

**MOV (Metal Oxide Varistor) Protection:**
```
┌────────────────────────────────────┐
│     MOV SURGE PROTECTION           │
├────────────────────────────────────┤
│                                    │
│      Line ────────┬────→ Relay    │
│                   │                │
│                  ┌┴┐ MOV          │
│                  │ │ (275V)       │
│                  └┬┘               │
│                   │                │
│     Neutral ──────┴────→ Relay    │
│                                    │
│  Purpose:                          │
│  - Clamps voltage spikes           │
│  - Protects against lightning      │
│  - Shunts surge to neutral         │
│                                    │
└────────────────────────────────────┘

MOV Specifications:
  Type: Disc, 14mm diameter
  Voltage: 275V AC (varistor voltage)
  Energy: 70 Joules
  Clamping Voltage: 710V (at 50A)
  Part Number: S14K275 or V275LA20A
  Cost: R18 each
```

#### Step 3: Relay Wiring with Isolation

**Relay Module Connections:**
```
┌──────────────────────────────────────────────┐
│        RELAY WIRING DIAGRAM                   │
├──────────────────────────────────────────────┤
│                                               │
│  HIGH VOLTAGE SIDE (AC):                     │
│  ─────────────────────────────────────       │
│                                               │
│  Fuse → [Relay COM] ───┬─ [NC] Not Used     │
│                        │                      │
│                        └─ [NO] → Load Out    │
│                                               │
│  (When relay energized: COM connects to NO)  │
│                                               │
│  ────────────────────────────────────────    │
│                                               │
│  LOW VOLTAGE SIDE (DC):                      │
│  ─────────────────────────────────────       │
│                                               │
│  ESP32 GPIO4 → [1kΩ] → PC817 LED+ →         │
│                         PC817 LED- → GND      │
│                                               │
│         5V → Relay VCC                        │
│        GND → Relay GND                        │
│                                               │
│  PC817 Collector → Relay IN                  │
│  PC817 Emitter → GND                         │
│                                               │
│  [Flyback Diode] across relay coil           │
│  (Anode to GND, Cathode to VCC)             │
│                                               │
└──────────────────────────────────────────────┘
```

**PC817 Optocoupler Wiring (Critical for Safety):**
```
Purpose: Provides 2500V isolation between high and low voltage

ESP32 Side (Input):
  GPIO4 → [1kΩ Resistor] → Pin 1 (Anode)
  Pin 2 (Cathode) → GND
  
Relay Side (Output):
  5V → Pin 4 (Collector) → Relay IN
  Pin 3 (Emitter) → GND

⚠️ NEVER connect ESP32 GPIO directly to relay coil!
   This creates a safety hazard and can damage ESP32.
```

### AFTERNOON (1PM-5PM): Safety Testing & Verification

#### Step 4: Continuity and Resistance Testing

```text
Testing Checklist (POWER OFF):
══════════════════════════════════════════

1. Isolation Test (MOST IMPORTANT):
   [√] AC Line to DC GND: > 10MΩ (infinite resistance)
   [√] AC Neutral to DC GND: > 10MΩ
   [√] Relay NO to ESP32 VCC: > 10MΩ
   
   ⚠️ If ANY reading is < 10MΩ, DO NOT POWER ON!
      Find and fix the short circuit immediately.

2. DC Power Rails:
   [√] 5V to GND: Should be ~100Ω (when ESP32 off)
   [√] 3.3V to GND: Should be ~500Ω (when ESP32 off)
   [√] No shorts between power rails

3. Relay Operation (Bench Test with 5V):
   [√] Apply 5V to relay VCC
   [√] Connect relay IN to GND (simulate ON)
   [√] Hear audible "click"
   [√] Measure continuity: COM to NO should be 0Ω
   [√] Measure continuity: COM to NC should be infinite

4. Optocoupler Test:
   [√] Forward voltage of LED: ~1.2V at 10mA
   [√] CTR (Current Transfer Ratio): 50-600%
   [√] Isolation resistance: > 10MΩ

5. Fuse Continuity:
   [√] Fuse resistance: < 0.1Ω (good fuse)
   [√] After load test: Still < 0.1Ω (not damaged)
```

#### Step 5: Low Voltage Functional Test

```bash
# Test with 12V DC power supply FIRST (not mains!)

Step-by-step:
1. Remove fuse from holder (safety measure)
2. Connect 12V power supply to relay COM and NO
3. Connect 12V light bulb as load
4. Power ESP32 with USB (isolated from relay)
5. Run test firmware:

// test_relay.ino
void setup() {
  pinMode(4, OUTPUT);  // Relay control GPIO
  Serial.begin(115200);
  Serial.println("Relay Test - 12V Safe Mode");
}

void loop() {
  Serial.println("Relay ON");
  digitalWrite(4, HIGH);
  delay(2000);
  
  Serial.println("Relay OFF");
  digitalWrite(4, LOW);
  delay(2000);
}

Expected Result:
- Bulb turns ON for 2 seconds
- Bulb turns OFF for 2 seconds
- Repeat continuously
- Relay clicks audibly each time
- NO smoke, NO unusual smells
- ESP32 temperature normal (<50°C)
```

#### Step 6: Mains Voltage Testing (⚠️ EXTREME CAUTION)

```text
FINAL MAINS TEST PROCEDURE:
══════════════════════════════════════════

⚠️⚠️⚠️ DANGER: HIGH VOLTAGE ⚠️⚠️⚠️

Pre-flight Checklist:
[√] All connections double-checked
[√] Isolation tested (> 10MΩ)
[√] Low voltage test passed
[√] Enclosure properly assembled
[√] Fuse installed (3A)
[√] Fire extinguisher ready
[√] Partner present to cut power
[√] Wearing safety glasses
[√] Using insulated tools only

Test Steps:
1. Plug into mains outlet (230V)
2. Monitor ESP32 serial output
3. Check 5V and 3.3V rails with multimeter
4. Expected readings:
   - 5V rail: 4.9-5.1V
   - 3.3V rail: 3.25-3.35V
5. Test relay switching:
   - Send ON command
   - Measure voltage at load output (should be 230V)
   - Send OFF command
   - Measure voltage at load output (should be 0V)

Load Test:
1. Connect 100W incandescent bulb as load
2. Turn relay ON
3. Bulb should light immediately
4. Monitor for 10 minutes:
   - Check for excessive heat
   - Check for burning smell
   - Check for voltage drop
5. Turn relay OFF
6. Bulb should turn off immediately

Final Checks:
[√] All components cool to touch (<50°C)
[√] No unusual noises or smells
[√] Relay switches reliably
[√] No voltage on load when OFF
[√] Full 230V on load when ON
[√] WiFi connects successfully
[√] MQTT commands work correctly
```

---

## 📅 DAY 28: TAMPER DETECTION & PHYSICAL SECURITY

### MORNING (9AM-12PM): MAX6316 Watchdog Integration

#### Step 1: Tamper Detection Circuit

**MAX6316 Watchdog Functionality:**
```
┌────────────────────────────────────────────┐
│      MAX6316 TAMPER WATCHDOG SYSTEM        │
├────────────────────────────────────────────┤
│                                             │
│  Purpose: Detect physical tampering        │
│           Reset MCU if case opened         │
│                                             │
│  Connections:                               │
│  Pin 1 (GND)  → Ground                     │
│  Pin 2 (WDI)  → ESP32 GPIO18 (Watchdog In)│
│  Pin 3 (WDO)  → ESP32 EN (Reset Out)      │
│  Pin 4 (MR)   → Tamper Switch              │
│  Pin 5 (VCC)  → 3.3V                       │
│                                             │
│  Tamper Switch:                             │
│  - Micro-switch on enclosure lid           │
│  - Normally Closed (NC) when case closed   │
│  - Opens when lid removed                  │
│  - Pulls MR pin LOW → Resets ESP32        │
│                                             │
│  Watchdog Operation:                        │
│  - ESP32 must toggle GPIO18 every 1s       │
│  - If no toggle → WDO goes LOW → Reset     │
│  - Prevents firmware hangs                 │
│                                             │
└────────────────────────────────────────────┘
```

**Tamper Detection Code:**
```cpp
// tamper_detection.ino

#include <Preferences.h>

#define WATCHDOG_PIN 18
#define TAMPER_DETECT_PIN 19
#define EEPROM_NAMESPACE "security"

Preferences prefs;
unsigned long lastWatchdogFeed = 0;
bool tamperDetected = false;

void setup() {
  Serial.begin(115200);
  
  // Watchdog output
  pinMode(WATCHDOG_PIN, OUTPUT);
  digitalWrite(WATCHDOG_PIN, LOW);
  
  // Tamper input (with pull-up)
  pinMode(TAMPER_DETECT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(TAMPER_DETECT_PIN), 
                  tamperISR, FALLING);
  
  // Load tamper log from EEPROM
  prefs.begin(EEPROM_NAMESPACE, false);
  int tamperCount = prefs.getInt("tamper_count", 0);
  
  Serial.printf("Previous tamper events: %d\n", tamperCount);
  
  // Check if device was tampered while powered off
  if (esp_reset_reason() == ESP_RST_EXT) {
    Serial.println("⚠️ ALERT: Device reset by tamper switch!");
    logTamperEvent("Power-off tamper");
  }
}

void loop() {
  // Feed watchdog every 500ms
  if (millis() - lastWatchdogFeed > 500) {
    digitalWrite(WATCHDOG_PIN, !digitalRead(WATCHDOG_PIN));
    lastWatchdogFeed = millis();
  }
  
  // Check for tamper event
  if (tamperDetected) {
    tamperDetected = false;
    logTamperEvent("Runtime tamper");
    sendTamperAlert();  // Send to cloud
  }
  
  // Normal operation continues...
  delay(10);
}

void IRAM_ATTR tamperISR() {
  tamperDetected = true;
}

void logTamperEvent(const char* eventType) {
  int count = prefs.getInt("tamper_count", 0);
  count++;
  prefs.putInt("tamper_count", count);
  
  // Store timestamp in EEPROM
  char key[20];
  sprintf(key, "tamper_%d", count);
  prefs.putULong(key, millis());
  
  Serial.printf("Tamper Event #%d: %s at %lu ms\n", 
                count, eventType, millis());
  
  // Flash LED rapidly (if installed)
  for (int i = 0; i < 10; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(50);
    digitalWrite(LED_PIN, LOW);
    delay(50);
  }
}

void sendTamperAlert() {
  // Send MQTT message to cloud
  String payload = "{\"alert\":\"tamper\",\"device\":\"";
  payload += getDeviceID();
  payload += "\",\"timestamp\":";
  payload += millis();
  payload += "}";
  
  mqttClient.publish("smartplug/alerts/tamper", payload.c_str());
  Serial.println("Tamper alert sent to cloud");
}
```

### AFTERNOON (1PM-5PM): Anti-Tamper Physical Features

#### Step 2: Security Screws and Seals

**Security Screw Installation:**
```
Screw Type: Torx with Center Pin (T10H)

Locations (4 total):
  1. Top-Left corner
  2. Top-Right corner
  3. Bottom-Left corner
  4. Bottom-Right corner

Installation:
  - Use T10H security driver
  - Torque to 1.2 Nm (finger-tight + 1/4 turn)
  - Do NOT overtighten (cracks plastic)
  
Cost: R30 for 100 screws (from RS Components)

Purpose:
  - Prevents casual opening with standard screwdriver
  - Deters unauthorized access
  - Professional appearance
```

**Tamper-Evident Seals:**
```
┌─────────────────────────────────────────┐
│     HOLOGRAPHIC SECURITY SEAL            │
├─────────────────────────────────────────┤
│                                          │
│          ╔══════════════╗               │
│          ║   VOID IF    ║               │
│          ║   REMOVED    ║               │
│          ║              ║               │
│          ║  [QR Code]   ║               │
│          ║  Device ID   ║               │
│          ╚══════════════╝               │
│                                          │
│  Features:                               │
│  - Holographic pattern (hard to forge)  │
│  - Leaves "VOID" text when removed      │
│  - Unique QR code per device            │
│  - Serial number printed                │
│                                          │
│  Placement:                              │
│  - One seal over each security screw    │
│  - One seal on bottom (covers SN label) │
│                                          │
└─────────────────────────────────────────┘

Purchase:
  - Supplier: Custom Security Printing (JHB)
  - Cost: R2 per seal (minimum order: 1000)
  - Lead time: 2 weeks
  
DIY Alternative (Prototype Phase):
  - Clear adhesive labels
  - Print "VOID IF REMOVED" text
  - Apply over screws
  - Cost: R0.10 per label
```

#### Step 3: Enclosure Labeling

**Required Labels and Markings:**
```
1. Rating Plate (mandatory):
   ┌───────────────────────────┐
   │   Smart Plug AI           │
   │   Model: SP-001           │
   │   230V~ 50Hz 10A Max      │
   │   2300W Max Load          │
   │   ⚠️ Indoor Use Only     │
   │                           │
   │   Serial: SP001-0001      │
   │   Made in South Africa    │
   └───────────────────────────┘
   
2. Safety Warnings:
   - ⚡ High Voltage - Risk of Electric Shock
   - 🔥 Do Not Exceed 2300W Load
   - 💧 Keep Dry - Not Water Resistant
   - ♻️ Do Not Dispose in Regular Trash
   
3. Certification Marks (when obtained):
   - SABS certification mark
   - ICASA approval number
   - CE mark (if exporting)
   
4. QR Code (for device pairing):
   - Links to: https://smartplugai.app/pair/SP001-0001
   - 25mm x 25mm size
   - High error correction (Level H)
   - Printed on durable label
```

**Label Printing Options:**
```
Option 1: Laser Engraving (Best)
├─ Permanent, won't fade
├─ Professional appearance
├─ Cost: R50 per device (setup fee + per-unit)
└─ Supplier: Local laser cutting service

Option 2: Vinyl Labels (Good)
├─ Durable, weather-resistant
├─ Full color possible
├─ Cost: R5 per device
└─ Supplier: Print shop (laminated vinyl)

Option 3: Printed Labels (Acceptable for Prototypes)
├─ Easy to produce
├─ Can fade over time
├─ Cost: R0.50 per device
└─ Method: Inkjet + clear laminate + adhesive backing
```

---

## 🎯 WEEK 4 DELIVERABLES CHECKLIST

```text
Hardware Deliverables:
[√] PCB design completed in KiCad
[√] Gerber files generated and ready for manufacturing
[√] 3D enclosure design completed
[√] Enclosures 3D printed (2x top, 2x bottom for 3 devices)
[√] PCBs ordered from JLCPCB/PCBWay (5-board batch)
[√] Heat-set inserts installed in enclosures
[√] All 3 devices assembled with proper isolation
[√] Fuse and MOV protection installed on each device
[√] Relay wiring completed with PC817 optocoupler isolation
[√] Tamper detection circuit installed and tested
[√] Security screws and tamper-evident seals applied
[√] Device labels printed and applied (rating plate, QR code)

Testing Deliverables:
[√] Isolation testing passed (>10MΩ between AC and DC)
[√] Low voltage functional test passed (12V DC)
[√] Mains voltage test passed (230V AC with 100W load)
[√] Relay switching verified (ON/OFF commands work)
[√] Tamper detection tested (case opening triggers alert)
[√] Watchdog functionality verified (resets on hang)
[√] Temperature monitoring during 1-hour load test (<50°C)
[√] All safety checks passed

Documentation Deliverables:
[√] PCB schematic (PDF export)
[√] PCB layout screenshots (top and bottom layers)
[√] 3D enclosure STL files
[√] Bill of Materials (BOM) for PCB assembly
[√] Assembly instructions with photos
[√] Safety test report (isolation, voltage, temperature)
[√] Tamper event log export from EEPROM
[√] Device serial number tracking spreadsheet

Software Deliverables:
[√] OTA firmware update system designed
[√] Tamper detection firmware integrated
[√] Watchdog feed routine in main loop
[√] EEPROM logging for security events
[√] Test firmware for relay control
[√] Calibration routine for SCT-013 current sensor

Next Week Preview:
→ Week 5 focuses on automation, rule engine, and advanced features
→ Hardware is now production-ready and enclosed
→ Shift focus to software features and user experience
```

---

## 📚 Additional Resources

**PCB Design References:**
- KiCad Tutorial: https://www.youtube.com/watch?v=vaCVh2SAZY4
- PCB Design Best Practices: https://www.tempoautomation.com/blog/pcb-design-best-practices/
- IEC 60950-1 Clearances: https://www.electrical-installation.org/enwiki/Clearances_and_creepage_distances

**3D Printing Resources:**
- Enclosure Design Guide: https://www.fictiv.com/articles/how-to-design-a-plastic-enclosure
- Heat-Set Insert Installation: https://www.youtube.com/watch?v=cyof7fYFcI4
- ABS Vapor Smoothing: https://rigid.ink/pages/acetone-vapor-smoothing

**Safety Standards:**
- SANS 60950-1: South African safety standard for electrical equipment
- SABS Certification Process: https://www.sabs.co.za/certification/
- ICASA Type Approval: https://www.icasa.org.za/

**Component Suppliers (South Africa):**
- RS Components: https://za.rs-online.com/
- Communica: https://www.communica.co.za/
- Mantech Electronics: https://www.mantech.co.za/
- DIY Electronics: https://www.diyelectronics.co.za/

---

## ⚠️ Common Pitfalls and Solutions

### PCB Design Mistakes:
```
❌ Mistake: Insufficient clearance between AC and DC traces
✓ Solution: Use 3mm minimum, add ground pour as shield

❌ Mistake: Thin traces for high current paths
✓ Solution: Use trace width calculator, 2mm for AC mains

❌ Mistake: No thermal relief for ground pours
✓ Solution: Enable thermal relief in KiCad settings

❌ Mistake: Forgetting mounting holes
✓ Solution: Add 4x M3 holes in corners, 5mm from edge
```

### Enclosure Design Mistakes:
```
❌ Mistake: Tight tolerances (PCB doesn't fit)
✓ Solution: Add 0.5mm clearance on all sides

❌ Mistake: No ventilation (overheating)
✓ Solution: Add vent slots totaling 20% of surface area

❌ Mistake: Weak snap-fit clips (break easily)
✓ Solution: Use 2mm height, 45° entry angle, test fit

❌ Mistake: No cable strain relief (wires pull out)
✓ Solution: Add cable clamps or tie-down points
```

### Wiring Mistakes:
```
❌ Mistake: Swapping Line and Neutral
✓ Solution: Double-check with tester, follow color code

❌ Mistake: No optocoupler isolation
✓ Solution: Always use PC817 between ESP32 and relay

❌ Mistake: Forgot flyback diode on relay coil
✓ Solution: Add 1N4007 diode, cathode to VCC

❌ Mistake: Loose screw terminals (fire hazard)
✓ Solution: Tighten to 0.5 Nm, check periodically
```

---

**End of Week 4 Guide** ✅  
**Next**: [WEEK5_AUTOMATION_RULES.md](WEEK5_AUTOMATION_RULES.md)
