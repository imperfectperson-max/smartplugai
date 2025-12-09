# Soldering Session Checklist

This checklist ensures you're prepared for the hardware assembly session and can safely build the Smart Plug AI prototype.

## 📦 Parts & Components

### Essential Components
- [ ] **ESP32 Development Board** (ESP32-DevKitC or similar)
  - Verify: 30 GPIO pins, Wi-Fi, Bluetooth
  - Check: USB connection works, can flash test firmware
  
- [ ] **INA219 Power Monitor Module**
  - Current sensing: 0-3.2A (verify range for your use case)
  - Voltage sensing: 0-26V
  - I2C interface (default address 0x40)
  
- [ ] **Relay Module** (5V single channel with optocoupler)
  - Rating: 10A @ 250VAC minimum
  - Verify: Normally Open (NO) and Common terminals
  - Test: Relay clicks when 5V applied to control pin
  
- [ ] **AC-DC Power Supply** (5V, 2A minimum)
  - Isolated HLK-PM01 or similar
  - Output: 5V DC for ESP32 and relay
  
- [ ] **Connectors & Terminals**
  - [ ] AC input socket (C14 or compatible)
  - [ ] AC output socket (C13 or compatible)
  - [ ] Screw terminals or wire connectors
  - [ ] Wire gauge: 16-18 AWG for AC connections
  
- [ ] **Passive Components**
  - [ ] Resistors for LED indicators (if adding)
  - [ ] Capacitors for power filtering (100nF, 10uF)
  - [ ] Pull-up resistors for I2C if needed (4.7kΩ)

### Optional Components
- [ ] **Case/Enclosure** (design ready or 3D print files)
- [ ] **Status LED** (power, relay state, Wi-Fi)
- [ ] **Push button** (for reset or pairing mode)
- [ ] **Fuse** (2A slow-blow for AC line protection)
- [ ] **Heat sink** (for power supply if needed)

## 🔧 Tools & Equipment

### Soldering Tools
- [ ] Soldering iron (temperature controlled, 350-400°C)
- [ ] Solder (60/40 or lead-free)
- [ ] Flux (optional but recommended)
- [ ] Desoldering wick or pump (for mistakes)
- [ ] Soldering iron stand
- [ ] Tip cleaner (sponge or brass wool)

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

### Phase 1: Component Testing (Breadboard)
1. [ ] Test ESP32: Upload blink sketch, verify USB communication
2. [ ] Test INA219: Connect to breadboard, read I2C address, verify readings with known load
3. [ ] Test relay: Apply 5V to control pin, verify clicking sound and switching
4. [ ] Test power supply: Measure output voltage (should be 5V ±0.25V)

### Phase 2: DC Side Assembly
1. [ ] Solder power supply output to ESP32 VIN/GND or 5V pin
2. [ ] Connect INA219 to ESP32:
   - VCC → 3.3V or 5V (check INA219 spec)
   - GND → GND
   - SDA → GPIO 21 (default) or your chosen pin
   - SCL → GPIO 22 (default) or your chosen pin
3. [ ] Connect relay control:
   - Relay VCC → 5V
   - Relay GND → GND
   - Relay IN → GPIO (e.g., GPIO 4, 5, or your choice)
4. [ ] Add decoupling capacitors near ESP32 and INA219

### Phase 3: AC Side Assembly (DANGER ZONE!)
**⚠️ DISCONNECT FROM MAINS POWER BEFORE STARTING ⚠️**

1. [ ] Connect AC input to power supply input (L, N, GND)
2. [ ] Connect AC line through INA219:
   - AC Line → INA219 VIN+
   - INA219 VIN- → Relay Common (C)
3. [ ] Connect relay to AC output:
   - Relay Normally Open (NO) → AC Output
4. [ ] Connect AC neutral and ground directly (bypass relay):
   - AC Neutral → AC Output Neutral
   - AC Ground → AC Output Ground
5. [ ] **Verify**: No connection between AC and DC circuits except through isolated power supply

### Phase 4: Testing & Validation
**Start with low voltage/current testing**

1. [ ] Visual inspection: Check all connections, no exposed wires
2. [ ] Continuity test: Verify circuit paths match schematic
3. [ ] Isolation test: Use multimeter to confirm >1MΩ between AC and DC
4. [ ] First power-on: Connect to AC mains with NO LOAD
   - ESP32 should boot (LED blinks)
   - Measure 5V at ESP32 power pins
   - INA219 should read near-zero current
5. [ ] Relay test: Control relay via GPIO, verify clicking and continuity change
6. [ ] Load test: Connect low-power load (LED or small light)
   - Verify relay switches load on/off
   - Check INA219 readings match expected values
7. [ ] Thermal test: Run for 15-30 minutes, check for heat buildup
8. [ ] Full load test: Test with rated load (if safe to do so)

## 📊 Data Collection Plan

### Initial Testing Data
Record in lab notebook or spreadsheet:
- [ ] No-load voltage readings (AC and DC)
- [ ] Known load measurements:
  - 40W incandescent bulb
  - 10W LED bulb
  - Laptop charger (~60W)
- [ ] INA219 readings vs. reference meter
- [ ] Relay switching time (if measurable)
- [ ] Power consumption of ESP32 + circuitry
- [ ] Temperature measurements at various loads

### Calibration Data
- [ ] INA219 calibration values for accuracy
- [ ] Voltage divider corrections (if used)
- [ ] Current sense resistor value verification
- [ ] Compare readings with commercial power meter

## 🐛 Troubleshooting Common Issues

| Issue | Possible Cause | Solution |
|-------|---------------|----------|
| ESP32 won't boot | No power or insufficient current | Check power supply output, verify connections |
| INA219 not detected | I2C connection issue | Verify SDA/SCL pins, check pull-up resistors, try I2C scanner |
| Relay won't switch | Control signal issue or bad relay | Verify GPIO output voltage (3.3V), test relay independently |
| Inaccurate readings | Calibration needed | Verify with known loads, adjust calibration values |
| ESP32 resets randomly | Power supply noise or brownout | Add decoupling caps, check power supply stability |
| Relay chattering | Bounce or electrical noise | Add RC snubber across relay contacts |

## 📸 Documentation

During assembly, capture:
- [ ] Photo of all components before starting
- [ ] Step-by-step assembly photos
- [ ] Wiring diagram as actually built (may differ from plan)
- [ ] Closeup of any tricky connections
- [ ] First successful test with measurements
- [ ] Any issues encountered and how they were resolved

## ✅ Final Checklist Before Use

- [ ] All connections secure and strain-relieved
- [ ] No exposed conductors on AC side
- [ ] Case closed and secured (if using enclosure)
- [ ] Proper labeling (input, output, voltage warnings)
- [ ] Firmware loaded and tested
- [ ] Emergency shut-off plan in place
- [ ] Understand limitations (max current, voltage)

## 🚀 Post-Soldering Next Steps

1. **Firmware refinement**: Load production firmware with MQTT, OTA updates
2. **Extended testing**: 24-hour burn-in test with real appliances
3. **App integration**: Connect mobile app to device for real-time data
4. **Documentation**: Update hardware docs with actual build notes
5. **Iteration planning**: Identify improvements for v2 based on learnings

---

**Remember**: Safety first! If you're unsure about any step, especially AC connections, consult with someone experienced in electrical work or hire a qualified electrician.

**Last Updated**: December 2025
