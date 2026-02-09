# WEEK 4: SAFETY, ENCLOSURE & PCB DESIGN - DEEP DIVE

📅 **Timeline**: Days 22-28  
🎯 **Goal**: Transform breadboard prototypes into professional, safe, enclosed smart plugs with PCB design ready for manufacturing

## 📅 DAY 22-23: PCB DESIGN WITH SECURITY ZONES

⚠️ **IMPORTANT WORKFLOW CHANGE**: Before designing your PCB, you should first 
select and measure your commercial plug enclosure (see Day 24-25). However, 
if you're learning PCB design concepts first, you can read through this section 
then return to finalize dimensions after measuring your enclosure.

**Recommended Approach:**
1. Read through Days 22-23 to understand PCB design principles
2. Jump to Day 24-25 to select and measure your enclosure
3. Return to Days 22-23 to design PCB with correct dimensions
4. Continue with Days 26-28 for assembly and testing

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
│      Dimensions: Based on YOUR enclosure            │
│      (Example shown: 100mm x 80mm reference)        │
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
  Dimensions: MEASURE YOUR ENCLOSURE FIRST!
              Reference size: 100mm x 80mm (adjust to fit your plug)
              ⚠️ PCB dimensions will vary based on chosen enclosure
              See Day 24-25 for enclosure measurement instructions
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

## 📅 DAY 24-25: WORKING WITH PRE-EXISTING PLUG ENCLOSURES

### MORNING (9AM-12PM): Selecting a Commercial Plug

#### Step 1: Understand South African Plug Standards

```text
South African Plug Requirements (SANS 164):
[√] 3-pin configuration (Live, Neutral, Earth)
[√] 230V AC, 50Hz operation
[√] 10A - 16A rating typical
[√] SABS approved marking required
[√] Round pins (4mm diameter, 19mm apart for L and N)
[√] Earth pin larger (7mm diameter, offset)
```

**Why Use Pre-Existing Plugs:**
```
┌─────────────────────────────────────────────────────┐
│     COMMERCIAL PLUG vs 3D PRINTING COMPARISON        │
├─────────────────────────────────────────────────────┤
│                                                      │
│  Pre-Existing Commercial Plug:                      │
│  ✓ Already SABS certified                          │
│  ✓ Professional appearance                          │
│  ✓ Lower cost (R50-R150)                           │
│  ✓ Proven safety and durability                    │
│  ✓ Faster deployment (no printing time)            │
│  ✓ Consistent quality                               │
│  ✗ Fixed internal dimensions                        │
│  ✗ Requires modification planning                   │
│                                                      │
│  3D Printed Custom Enclosure:                       │
│  ✓ Custom dimensions                                │
│  ✓ Perfect component fit                            │
│  ✗ Not SABS certified (requires testing)           │
│  ✗ Higher cost (R300+ including materials/time)    │
│  ✗ Less professional appearance                     │
│  ✗ Durability concerns                              │
│  ✗ 8-16 hours printing time per unit               │
│                                                      │
└─────────────────────────────────────────────────────┘
```

#### Step 2: Select Appropriate Commercial Plug

**Recommended Plug Types for South Africa:**
```
Option 1: Standard SA 3-Pin Plug Enclosure
├─ Rating: 16A, 230V
├─ Internal Space: ~70mm x 50mm x 25mm (typical)
├─ Suppliers: Communica, Mantech, DIY Electronics
├─ Cost: R50-R80 per unit
├─ Pros: Widely available, easy to source
└─ Cons: Limited internal space

Option 2: Industrial Plug Housing
├─ Rating: 16A, 230V
├─ Internal Space: ~90mm x 60mm x 30mm
├─ Suppliers: RS Components, Voltex
├─ Cost: R120-R150 per unit
├─ Pros: More internal space, robust construction
└─ Cons: Bulkier, slightly more expensive

Option 3: Universal Extension Lead Plug
├─ Rating: 16A, 230V
├─ Internal Space: ~85mm x 55mm x 28mm
├─ Suppliers: Builders Warehouse, Makro
├─ Cost: R60-R100 per unit
├─ Pros: Good balance of space and cost
└─ Cons: May need more modifications
```

**Example South African Suppliers:**
```
Communica (Online & Stores):
  Website: https://www.communica.co.za/
  Locations: Cape Town, Johannesburg, Durban
  Products: Standard plugs, industrial enclosures
  Delivery: 2-3 days nationwide
  
Mantech Electronics:
  Website: https://www.mantech.co.za/
  Locations: Nationwide stores
  Products: Electrical enclosures, plug housings
  Delivery: Same-day in major metros
  
RS Components South Africa:
  Website: https://za.rs-online.com/
  Products: Industrial-grade plug housings
  Delivery: Next-day for stock items
  
DIY Electronics:
  Website: https://www.diyelectronics.co.za/
  Locations: Cape Town
  Products: Hobby/maker-friendly enclosures
```

#### Step 3: Measure Internal Cavity

```text
Checklist - Enclosure Measurements:
[√] Disassemble plug safely (disconnect from power!)
[√] Measure internal length (longest dimension)
[√] Measure internal width (shortest dimension)
[√] Measure internal height (depth available for PCB)
[√] Identify existing screw posts/mounting points
[√] Measure cable entry hole diameter
[√] Check for ventilation openings
[√] Identify modification points (where to drill)
[√] Take photos for reference
[√] Note material type (ABS, PVC, polycarbonate)
```

**How to Measure Your Enclosure:**
```
Tools Needed:
  - Digital calipers (or ruler)
  - Notepad and pen
  - Camera (smartphone)
  - Small screwdriver set

Measuring Process:
  
1. External Dimensions (for reference):
   Length x Width x Height (mm)
   Example: 85mm x 55mm x 30mm

2. Internal Cavity (CRITICAL for PCB design):
   ┌──────────────────────┐
   │                      │ ← Width (W)
   │   Available Space    │
   │   for PCB & Parts    │ ← Height (H)
   │                      │
   └──────────────────────┘
   Length (L)
   
   Measure: L, W, H minus 2-3mm for clearance
   Example: 75mm x 48mm x 22mm usable space

3. Existing Mounting Points:
   - Location of screw posts (X, Y coordinates)
   - Diameter of screw holes
   - Height of mounting posts above base
   
4. Obstruction Assessment:
   - Any internal ribs or supports
   - Pin connector locations
   - Areas that cannot be used

⚠️ IMPORTANT: Design PCB to fit YOUR measured dimensions
(Note: 100mm x 80mm was the reference size for 3D printed enclosures.
Commercial plugs vary - use YOUR actual measurements!)
```

### AFTERNOON (1PM-5PM): PCB Sizing & Modification Planning

#### Step 4: Adjust PCB Dimensions to Fit

**PCB Sizing Guidelines:**
```
Based on YOUR Enclosure Measurements:

If Internal Space is 75mm x 48mm x 22mm:
  
PCB Dimensions:
  Length: 70mm (5mm clearance)
  Width: 43mm (5mm clearance)
  Height: Keep components under 18mm (4mm clearance)
  
Component Placement Strategy:
  ┌─────────────────────────────────┐
  │  [Power In]    [Relay]          │
  │                                 │
  │  [ESP32-S3]   [ATECC608A]      │
  │                                 │
  │  [SCT-013 Input]   [Regulator] │
  └─────────────────────────────────┘
  
  Compact Layout Required:
  - Use SMD components where possible
  - Stack modules if needed (ESP32-S3 is low profile)
  - Route cables carefully
  - May need smaller relay (HF115F-005-1ZS3 is 15.5mm)
```

**Component Height Considerations:**
```
Critical Height Measurements:
  
ESP32-S3 WROOM-1-N8: ~3mm profile
Relay Module (HF115F): 15mm height ⚠️ (may be too tall for enclosures with <20mm internal height)
ATECC608A (SOIC-8): 1.5mm (surface mount)
MAX6316 (SOT-23): 1mm (surface mount)
Screw Terminals: 10-12mm height
  
Height Reduction Options:
1. Use SMD relay (Omron G3VM series): 3mm height
2. Use horizontal-mount relay: 10mm height
3. Right-angle screw terminals: 8mm height
4. Remove ESP32 module, use bare IC: 1mm
   (Advanced - requires ESP32-S3-WROOM soldering)
```

#### Step 5: Plan Enclosure Modifications

```text
Checklist - Modification Plan:
[√] Identify cable entry points (power in, SCT-013, load out)
[√] Mark drill locations for cables (use masking tape)
[√] Plan ventilation holes if needed (relay heat dissipation)
[√] Identify PCB mounting strategy (screws, standoffs, adhesive)
[√] Check clearance for programming header access
[√] Plan LED light pipe locations (status indicators)
[√] Verify earth connection accessibility
[√] Document all planned modifications with photos/sketches
```

**Safe Modification Guidelines:**
```
┌─────────────────────────────────────────────────────┐
│     SAFETY GUIDELINES FOR MODIFYING PLUGS            │
├─────────────────────────────────────────────────────┤
│                                                      │
│  DO:                                                 │
│  ✓ Work with plug UNPLUGGED and de-energized       │
│  ✓ Use proper drill bits for plastic (high-speed)  │
│  ✓ Start with small pilot holes, then enlarge      │
│  ✓ Deburr all holes with sandpaper                 │
│  ✓ Maintain 3mm clearance between AC and DC       │
│  ✓ Use grommets for cable entry (strain relief)   │
│  ✓ Test fit components before final assembly       │
│  ✓ Document your modifications                      │
│                                                      │
│  DON'T:                                             │
│  ✗ Drill through existing safety barriers          │
│  ✗ Remove essential structural supports            │
│  ✗ Compromise the earth connection                 │
│  ✗ Create sharp edges that could cut wires        │
│  ✗ Over-drill holes (weakens enclosure)           │
│  ✗ Modify mains pins or connections                │
│                                                      │
└─────────────────────────────────────────────────────┘
```

**Modification Steps:**
```
1. Cable Entry Holes:
   Tools: Drill (2.5mm, 5mm, 8mm bits), step drill bit
   
   For Power Input Cable (6mm diameter):
   - Mark location on enclosure side
   - Drill pilot hole (2.5mm)
   - Enlarge to 6.5mm
   - Install rubber grommet (waterproofing)
   
   For SCT-013 Cable (10mm diameter):
   - Same process, final hole 10.5mm
   - Use larger grommet

2. Ventilation (if needed):
   If relay gets hot (>50°C in testing):
   - Drill 3-4 holes (5mm diameter)
   - Location: Top and side (convection flow)
   - Cover with mesh tape (dust prevention)

3. PCB Mounting:
   Option A - Existing Posts:
     - Check if existing screw posts align with PCB holes
     - May need to drill new holes in PCB to match
   
   Option B - Adhesive Standoffs:
     - Use M3 nylon standoffs (3M adhesive base)
     - Position to support PCB corners
     - Height: 3-5mm
   
   Option C - Hot Glue Dots:
     - Quick and removable
     - Apply dots at PCB corners
     - Not recommended for high-heat areas

4. Tamper Detection Integration:
   - Identify existing screw holes
   - Replace standard screws with security screws
   - Install micro-switch to detect opening
   - Wire switch to MAX6316 MR pin
```

#### Step 6: Assembly and Testing

```text
Assembly Process:
[√] Modify enclosure (drill holes, install grommets)
[√] Clean all plastic shavings (vacuum and wipe)
[√] Install PCB mounting standoffs
[√] Test fit PCB (no forcing!)
[√] Mount PCB securely
[√] Route cables through grommets
[√] Connect all wiring
[√] Verify no shorts with multimeter
[√] Check earth continuity
[√] Close enclosure (test fit before final assembly)
[√] Install security screws
[√] Add tamper-evident labels if desired
[√] Label device (rating plate, QR code)
```

**Cost Comparison:**
```
Commercial Plug Approach:
  Plug enclosure: R80
  Modification time: 1 hour
  Tools needed: Drill, bits, grommets (R50 if not owned)
  
  Total if tools owned: R80 + 1 hour labor
  Total if tools needed: R130 + 1 hour labor
  
vs
  
3D Printing Approach:
  Filament cost: R150 (100g ABS)
  Print time: 16 hours (both halves)
  Heat-set inserts: R30
  Post-processing time: 2 hours
  
  Total: R180 + 18 hours + printer access
  
Savings: R50-R100 + 17 hours per device!
(Plus no need to find/access 3D printer)
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

#### Step 1b: Adapting Tamper Detection for Pre-Existing Enclosures

**Working with Existing Enclosure Screws:**
```
┌────────────────────────────────────────────────────┐
│   TAMPER DETECTION FOR COMMERCIAL PLUG ENCLOSURES  │
├────────────────────────────────────────────────────┤
│                                                     │
│  Challenge: Pre-existing plugs have fixed screw    │
│  locations and may not have room for micro-switch  │
│                                                     │
│  Solution Options:                                  │
│                                                     │
│  Option 1: Use Existing Screw Holes                │
│  ─────────────────────────────────                 │
│  - Replace standard screws with security screws    │
│  - Install micro-switch at screw post              │
│  - Switch actuator contacts screw head when closed │
│  - When screw removed, switch opens                │
│                                                     │
│  Installation:                                      │
│  1. Identify screw post inside enclosure           │
│  2. Hot-glue micro-switch next to post             │
│  3. Position so switch is pressed when assembled   │
│  4. Wire switch NC terminal to MAX6316 MR pin      │
│  5. Test: Open case should trigger reset           │
│                                                     │
│  Option 2: Reed Switch (Magnetic)                  │
│  ───────────────────────────────                   │
│  - Glue small magnet to inside of lid              │
│  - Mount reed switch on PCB                        │
│  - When lid closed, magnet closes reed switch      │
│  - When lid opened, switch opens                   │
│                                                     │
│  Pros: No mechanical pressure needed               │
│  Cons: Magnet can fail, less reliable              │
│  Cost: R15 per reed switch + R5 per magnet         │
│                                                     │
│  Option 3: Conductive Tape Method                  │
│  ───────────────────────────────────               │
│  - Apply conductive tape across enclosure seam     │
│  - Connect tape to MAX6316 MR pin                  │
│  - Opening case breaks tape circuit                │
│                                                     │
│  Pros: Very cheap (R1 per device)                  │
│  Cons: One-time use, must replace after each open  │
│  Use Case: Shipping/deployment, not for testing    │
│                                                     │
│  Option 4: No Tamper Detection (Basic Builds)      │
│  ───────────────────────────────────────           │
│  - Skip MAX6316 entirely                           │
│  - Rely only on security screws                    │
│  - Use tamper-evident seals only                   │
│                                                     │
│  Trade-off: Lower cost, reduced security           │
│  Acceptable for: Home use, prototypes              │
│                                                     │
└────────────────────────────────────────────────────┘
```

**Micro-Switch Mounting Examples:**
```
Scenario A: Enclosure with Screw Posts
        ┌─────────────────────┐
        │  [Lid - Closed]     │
        │      [Screw Head]   │ ← Presses switch
        └───────────┬─────────┘
                    ↓
        ┌───────────┴─────────┐
        │   ●←─ Micro-switch  │
        │   │  (hot-glued to  │
        │   │   screw post)   │
        │  [ ] Screw Post     │
        └─────────────────────┘

Scenario B: Flat Enclosure (No Posts)
        ┌─────────────────────┐
        │  [Lid]              │
        │   [Magnet] ←─ Glued inside
        └───────────┬─────────┘
                    ↓ Magnetic field
        ┌───────────┴─────────┐
        │  [Reed Switch]      │ ← On PCB
        │  [PCB]              │
        └─────────────────────┘

Scenario C: Tight Space (No Room)
        Use security screws + tamper seals only
        Skip electronic tamper detection
        Document in device notes
```

**Installation Steps for Option 1 (Micro-Switch):**
```text
[√] Purchase micro-switch (SPDT, rated 125V, 1A)
    Recommended: Omron D2F-01 (R8 each)
[√] Test switch with multimeter (verify NC contact)
[√] Identify screw post location inside enclosure
[√] Clean surface with isopropyl alcohol
[√] Apply small amount of hot glue to switch base
[√] Position switch so actuator aligns with screw head
[√] Hold for 30 seconds while glue sets
[√] Wire NC terminal to MAX6316 MR pin (GPIO)
[√] Wire COM terminal to ground
[√] Leave NO terminal unconnected
[√] Test: Close enclosure, switch should be pressed
[√] Test: Open enclosure, ESP32 should reset/alert
[√] Add strain relief for wires (small cable tie)
```

**Alternative: Software-Only Tamper Detection:**
```cpp
// For enclosures without physical switch
// Use accelerometer to detect movement/opening
// ⚠️ NOTE: This is an OPTIONAL advanced feature
// Add MPU6050 (~R60) or ADXL345 (~R50) to your BOM if using this method

#include <Wire.h>
#include <MPU6050.h>  // Or ADXL345

MPU6050 accel;
bool suddenMovementDetected = false;

void setup() {
  Wire.begin();
  accel.initialize();
  accel.setAccelerometerPowerOnDelay(50);
  
  // Set motion detection threshold
  accel.setMotionDetectionThreshold(20);  // Sensitivity
  accel.setMotionDetectionDuration(10);   // Time window
  
  attachInterrupt(digitalPinToInterrupt(ACCEL_INT_PIN), 
                  motionISR, RISING);
}

void loop() {
  if (suddenMovementDetected) {
    suddenMovementDetected = false;
    logTamperEvent("Possible physical tampering - movement detected");
    // Send alert but don't reset (could be false positive)
  }
}

void IRAM_ATTR motionISR() {
  suddenMovementDetected = true;
}

// Cost: ~R60 for MPU6050 breakout board
// Pros: Detects any enclosure movement
// Cons: False positives possible, uses extra GPIO
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
[√] PCB design completed in KiCad (dimensions based on enclosure)
[√] Gerber files generated and ready for manufacturing
[√] Commercial plug enclosure selected and measured
[√] PCB dimensions finalized based on enclosure (varies per device)
[√] Enclosure modification plan documented
[√] PCBs ordered from JLCPCB/PCBWay (5-board batch)
[√] Enclosures modified (cable holes, mounting, ventilation)
[√] All 3 devices assembled with proper isolation
[√] Fuse and MOV protection installed on each device
[√] Relay wiring completed with PC817 optocoupler isolation
[√] Tamper detection circuit installed and tested (adapted to enclosure)
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
[√] Enclosure modification photos and measurements
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

**Enclosure Modification & Safety:**
- Electrical Enclosure Safety Guide: https://www.electrical-installation.org/enwiki/Enclosures
- Plug Modification Best Practices: Work safely with power disconnected
- Cable Grommet Installation: Strain relief for all cable entries
- Tamper-Evident Seal Options: Research security label suppliers

**Safety Standards:**
- SANS 60950-1: South African safety standard for electrical equipment
- SABS Certification Process: https://www.sabs.co.za/certification/
- ICASA Type Approval: https://www.icasa.org.za/

**Component & Enclosure Suppliers (South Africa):**
- RS Components: https://za.rs-online.com/ (Industrial plugs, enclosures)
- Communica: https://www.communica.co.za/ (Electrical plugs, housings)
- Mantech Electronics: https://www.mantech.co.za/ (Maker-friendly supplies)
- DIY Electronics: https://www.diyelectronics.co.za/ (Hobby components)
- Builders Warehouse: Retail stores nationwide (Basic plug enclosures)
- Voltex: https://www.voltex.co.za/ (Industrial electrical supplies)

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

❌ Mistake: Designing PCB before measuring enclosure
✓ Solution: Always measure enclosure first, then design PCB to fit
```

### Enclosure Modification Mistakes:
```
❌ Mistake: Drilling in wrong location (damages structure)
✓ Solution: Mark with masking tape, start with pilot holes, measure twice

❌ Mistake: Compromising electrical safety (drilling through barriers)
✓ Solution: Maintain 3mm AC-to-DC clearance, never drill safety barriers

❌ Mistake: PCB too large for enclosure
✓ Solution: Measure internal cavity accurately, design PCB 5mm smaller

❌ Mistake: No cable strain relief (wires pull out)
✓ Solution: Use rubber grommets for all cable entries, secure with cable ties

❌ Mistake: Inadequate PCB mounting (loose, unstable)
✓ Solution: Use adhesive standoffs, hot glue corners, or drill into existing posts

❌ Mistake: Components too tall for enclosure
✓ Solution: Use low-profile components (SMD relay, horizontal terminals)

❌ Mistake: Poor ventilation (relay overheating)
✓ Solution: Add ventilation holes if relay exceeds 50°C in testing

❌ Mistake: Sharp edges from drilling (cuts wires)
✓ Solution: Deburr all holes with sandpaper or file
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
