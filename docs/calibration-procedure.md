# Current Sensor Calibration Procedure

Detailed calibration procedure for the SCT-013-030 current transformer to achieve ±2% measurement accuracy across the full 0.1A - 30A range.

## 📋 Overview

**Purpose**: Calibrate current measurements to match reference meter within ±2% error

**Time Required**: 45-60 minutes per device

**Required Accuracy**: ±2% over full range (0.1A - 30A)

**Calibration Method**: Multi-point calibration with known resistive loads

---

## 🔧 Required Equipment

### Essential Equipment

1. **Reference Multimeter**: Fluke 87V or equivalent
   - Current measurement accuracy: ±1% or better
   - Calibration certificate current (annually)
   - True RMS capability

2. **Known Resistive Loads**:
   - 60W incandescent bulb (0.261A @ 230V)
   - 100W incandescent bulb (0.435A @ 230V)
   - 200W incandescent bulb (0.870A @ 230V)
   - 500W halogen lamp (2.17A @ 230V)
   - 1000W resistive heater (4.35A @ 230V)
   
   **⚠️ IMPORTANT**: Must use RESISTIVE loads (incandescent/halogen/heaters)
   - DO NOT use LED bulbs (non-linear load)
   - DO NOT use CFL bulbs (poor power factor)
   - DO NOT use motors (inductive, variable load)

3. **Test Equipment**:
   - Isolation transformer (recommended for safety)
   - Variable AC power supply (optional, for fine control)
   - Oscilloscope (optional, for waveform verification)
   - Temperature sensor (for ambient temperature logging)

### Safety Equipment

- Safety glasses (mandatory!)
- Insulated gloves
- Non-conductive mat
- GFCI/RCD protected outlet
- Fire extinguisher (ABC type)

---

## ⚠️ Safety Warnings

**DANGER - MAINS VOLTAGE CAN BE LETHAL**

- ⚠️ NEVER touch AC connections while powered
- ⚠️ Use isolation transformer if available
- ⚠️ Always use GFCI/RCD protected outlet
- ⚠️ One person working at a time on AC
- ⚠️ Emergency shutoff accessible
- ⚠️ Wait for loads to cool before touching (halogen gets HOT!)

---

## 📐 Pre-Calibration Setup

### 1. Hardware Verification

Verify the following BEFORE calibration:

```bash
1. SCT-013-030 correctly installed:
   - Only ONE wire (AC Live) through CT core
   - White stripe orientation noted
   - Core fully closed (audible click)
   - 3.5mm jack fully inserted

2. Burden resistor: 33Ω ±1%, 1W
   - Measure resistance: Should be 32.67Ω - 33.33Ω
   - Visual inspection: No signs of overheating

3. ADC circuit:
   - 100nF capacitor between ADC pin and GND
   - 1N4007 protection diode across burden resistor
   - Clean solder joints, no shorts

4. Power supply:
   - Stable 5V ±5% under load
   - ESP32 boots reliably
   - No brownouts during operation
```

### 2. Environmental Conditions

Record environmental conditions (affects accuracy):

```
Date: ________________
Time: ________________  
Ambient Temperature: ______°C (ideal: 20-25°C)
Humidity: ______% RH
Mains Voltage: ______V AC (measure with reference meter)
Mains Frequency: ______ Hz (typically 50Hz or 60Hz)
```

**⚠️ Note**: If ambient temperature varies >5°C from calibration temp, recalibration may be needed.

### 3. Reference Meter Setup

1. Connect reference meter in series with load:
   ```
   AC Source → Reference Meter → Load → AC Return
   ```

2. Set meter to AC current mode, appropriate range

3. Verify meter calibration certificate is current

4. Allow meter to stabilize for 5 minutes

---

## 🎯 Calibration Procedure

### Phase 1: Zero-Offset Calibration (No Load)

**Purpose**: Establish ADC baseline with no current flowing

**⚠️ CRITICAL**: Ensure NO load connected during this phase!

#### Steps:

1. **Prepare System**:
   ```bash
   - Disconnect ALL loads from output socket
   - Verify AC Live wire still passes through CT
   - Power on ESP32
   - Upload calibration firmware
   ```

2. **Run Zero Calibration**:
   ```c
   // In calibration firmware:
   ESP_ERROR_CHECK(current_sensor_calibrate_zero());
   
   // Expected serial output:
   // Starting zero-offset calibration...
   // ⚠️  ENSURE NO LOAD IS CONNECTED!
   // Waiting 5 seconds...
   // Taking 1000 samples...
   // Zero offset: 1.652V
   ```

3. **Record Results**:
   ```
   Zero Offset Voltage: _______ V
   
   Expected Range: 1.60V - 1.70V
   Sample Standard Deviation: _______ mV (should be <5mV)
   ```

4. **Verify**:
   - Offset voltage in expected range (1.60-1.70V)?
   - Low standard deviation (<5mV)?
   - No load connected during calibration?

**If any check fails, investigate before proceeding!**

---

### Phase 2: Single-Point Calibration (60W Load)

**Purpose**: Calculate scale factor using known load

#### Steps:

1. **Connect Known Load**:
   ```bash
   - Connect 60W incandescent bulb to output socket
   - Verify bulb lights when relay is ON
   - Allow 30 seconds for thermal stabilization
   ```

2. **Measure Reference Current**:
   ```bash
   - Use reference meter to measure actual current
   - Record reading: _______ A
   - Expected: ~0.261A @ 230V (P = V × I)
   - Actual may vary ±10% due to mains voltage variation
   ```

3. **Run Calibration**:
   ```c
   // In calibration firmware:
   float expected_current = 0.261; // Use ACTUAL measured value!
   ESP_ERROR_CHECK(current_sensor_calibrate_with_load(expected_current));
   
   // Serial output:
   // Starting load calibration...
   // Expected current: 0.261 A
   // Waiting 3 seconds for load to stabilize...
   // Measured current: 0.259 A
   // Scale factor: 0.033200 V/A
   // Error: 0.77%
   ```

4. **Record Results**:
   ```
   Reference Meter Reading: _______ A
   Device Reading (before cal): _______ A
   Device Reading (after cal): _______ A
   Scale Factor: _______ V/A
   Calibration Error: _______ %
   ```

5. **Verify**:
   - Error <2%?
   - Scale factor reasonable (~0.032-0.034 V/A)?

---

### Phase 3: Verification Point #1 (100W Load)

**Purpose**: Verify calibration accuracy at different load

#### Steps:

1. **Connect 100W Load**:
   ```bash
   - Replace 60W bulb with 100W bulb
   - Allow 30 seconds for stabilization
   ```

2. **Measure Both Meters**:
   ```
   Reference Meter: _______ A (expect ~0.435A @ 230V)
   Device Reading: _______ A
   Error: _______ % = |Reference - Device| / Reference × 100%
   ```

3. **Accept/Reject**:
   - Error <2%? → PASS, proceed to Phase 4
   - Error >2%? → FAIL, return to Phase 2 and recalibrate

**Troubleshooting if verification fails**:
- Check burden resistor value (should be exactly 33Ω ±1%)
- Verify CT is fully closed
- Check for noise (oscilloscope on ADC pin)
- Verify stable power supply
- Check reference meter calibration

---

### Phase 4: Multi-Point Calibration (Optional, Recommended)

**Purpose**: Correct for non-linearity across full range

**Note**: This step improves accuracy but is optional. Single-point calibration typically achieves ±2%.

#### Calibration Points:

Test at 5+ different loads across the range:

| Load | Expected Current @ 230V | Reference Reading | Device Reading | Error % |
|------|------------------------|-------------------|----------------|---------|
| 60W  | 0.261A | ______ A | ______ A | ______ % |
| 100W | 0.435A | ______ A | ______ A | ______ % |
| 200W | 0.870A | ______ A | ______ A | ______ % |
| 500W | 2.17A  | ______ A | ______ A | ______ % |
| 1000W| 4.35A  | ______ A | ______ A | ______ % |

**⚠️ Note**: Loads may not be exactly rated power. Use ACTUAL reference meter readings!

#### Steps:

For each load:

1. Connect load, wait 30 seconds
2. Record reference meter reading
3. Record device reading
4. Calculate error
5. Save calibration point:
   ```c
   // In firmware:
   add_calibration_point(reference_current, measured_current);
   ```

After all points collected:

```c
// Calculate and apply linearity correction
ESP_ERROR_CHECK(calculate_linearity_correction());
ESP_ERROR_CHECK(current_sensor_save_calibration());
```

The firmware will use linear interpolation between calibration points for improved accuracy.

---

### Phase 5: High-Current Verification (Optional)

**⚠️ WARNING**: Only perform if you have appropriate high-current load (>10A)

**Purpose**: Verify accuracy at rated current (30A for SCT-013-030)

#### Safety Precautions:

- Use heavy-duty loads rated for high current
- Verify all AC connections can handle current (10+ AWG wire)
- Monitor temperature continuously
- Limit test duration to <5 minutes
- Have fire suppression ready

#### Test Points:

| Current Level | Test Duration | Temperature Limit |
|--------------|---------------|-------------------|
| 10A | 5 min | Relay contacts: <80°C |
| 15A | 3 min | AC wiring: <70°C |
| 20A | 2 min | Burden resistor: <60°C |
| 30A | 1 min | CT: <80°C |

**⚠️ ABORT TEST IF**:
- Any component exceeds temperature limit
- Smoke or burning smell
- Relay chattering or arcing
- Power supply overheating

**Measurement**:
```
Test Current: ______ A (reference meter)
Device Reading: ______ A
Error: ______ %
Maximum Temperature Observed: ______ °C (at component: __________)
```

**Typical Results**:
- 10A: Error <2%
- 20A: Error <3%
- 30A: Error <5% (acceptable, rarely used)

---

## 💾 Save Calibration Data

### 1. Generate Calibration Certificate

Calibration data is saved to EEPROM and a certificate is generated:

```c
ESP_ERROR_CHECK(current_sensor_save_calibration());
// Generates JSON certificate with all calibration data
```

**Certificate Contents** (see example below):
- Device ID
- Calibration date and time
- Calibrated by (person)
- Reference equipment used
- Environmental conditions
- All calibration points (expected vs measured)
- Calibration constants (zero offset, scale factor)
- Error statistics (R², max error, RMS error)
- Notes and observations

### 2. Certificate Example

```json
{
  "device_id": "SP001",
  "serial_number": "2024-001",
  "calibration_date": "2024-01-15T14:30:00Z",
  "calibrated_by": "John Smith",
  "reference_equipment": {
    "multimeter": "Fluke 87V",
    "serial_number": "12345678",
    "calibration_cert": "CAL-2023-0542",
    "cert_expiry": "2024-12-31"
  },
  "environmental_conditions": {
    "ambient_temp_c": 23.5,
    "humidity_percent": 45,
    "mains_voltage_v": 231.2,
    "mains_frequency_hz": 50.0
  },
  "calibration_points": [
    {
      "load": "60W incandescent",
      "power_w": 60,
      "expected_current_a": 0.261,
      "reference_current_a": 0.259,
      "measured_current_a": 0.259,
      "error_percent": 0.0,
      "error_abs_a": 0.000
    },
    {
      "load": "100W incandescent",
      "power_w": 100,
      "expected_current_a": 0.435,
      "reference_current_a": 0.433,
      "measured_current_a": 0.433,
      "error_percent": 0.0,
      "error_abs_a": 0.000
    },
    {
      "load": "200W incandescent",
      "power_w": 200,
      "expected_current_a": 0.870,
      "reference_current_a": 0.868,
      "measured_current_a": 0.865,
      "error_percent": 0.35,
      "error_abs_a": 0.003
    }
  ],
  "calibration_constants": {
    "zero_offset_v": 1.652,
    "scale_factor_v_per_a": 0.033200,
    "phase_correction_deg": 0.0,
    "temperature_coeff": 0.001
  },
  "statistics": {
    "r_squared": 0.9998,
    "max_error_percent": 0.77,
    "rms_error_percent": 0.45,
    "calibration_points_count": 3
  },
  "notes": "Calibrated with Fluke 87V multimeter. All loads resistive (incandescent bulbs). Ambient conditions stable throughout calibration."
}
```

### 3. Store Certificate

1. **Save to device EEPROM**:
   - Ensures calibration persists across power cycles
   - Automatically loaded on boot

2. **Save to file**:
   ```bash
   # Save certificate to file
   echo '[certificate JSON]' > calibration/SP001_cal_2024-01-15.json
   ```

3. **Upload to backend** (optional):
   ```bash
   # Send to cloud storage for audit trail
   curl -X POST https://api.smartplugai.com/calibration \
     -H "Content-Type: application/json" \
     -d @calibration/SP001_cal_2024-01-15.json
   ```

---

## 📊 Post-Calibration Verification

### 1. Random Load Test

Test with random household loads to verify real-world accuracy:

| Load Type | Measured (Device) | Measured (Reference) | Error % |
|-----------|-------------------|---------------------|---------|
| LED lamp | ______ A | ______ A | ______ % |
| Laptop charger | ______ A | ______ A | ______ % |
| Electric kettle | ______ A | ______ A | ______ % |
| Hair dryer | ______ A | ______ A | ______ % |

**Expected**: All errors <5% (most <2%)

**Note**: Non-resistive loads (LED, charger) may have slightly higher error due to harmonics and poor power factor.

### 2. Long-Term Stability Test

Monitor calibration stability over time:

```bash
Week 1: Error ______ % (60W bulb)
Week 2: Error ______ % (60W bulb)
Week 4: Error ______ % (60W bulb)
Week 12: Error ______ % (60W bulb)
```

**Recalibration Recommended If**:
- Error exceeds ±3% for known load
- Ambient temperature changed >10°C from calibration
- Hardware modifications made (new CT, burden resistor, etc.)
- One year elapsed since last calibration

---

## 🔧 Troubleshooting

### Issue: Zero Offset Outside Range (Not 1.60-1.70V)

**Possible Causes**:
- Wrong burden resistor (not 33Ω)
- ADC reference voltage incorrect
- CT has residual magnetization
- Power supply issue

**Solutions**:
1. Measure burden resistor: Should be 33Ω ±1%
2. Check ADC reference: Should be 1.1V internal reference
3. Degauss CT: Pass strong AC magnet near CT core
4. Verify 3.3V rail stable under load

### Issue: High Calibration Error (>2%)

**Possible Causes**:
- Reference meter inaccurate
- Unstable mains voltage
- CT not fully closed
- Wrong calibration load used

**Solutions**:
1. Verify reference meter calibration certificate current
2. Monitor mains voltage during calibration (should be stable ±2%)
3. Check CT core fully closed (audible click)
4. Use RESISTIVE loads only (incandescent, halogen, heaters)
5. Allow loads to thermally stabilize (30+ seconds)

### Issue: Inconsistent Readings

**Possible Causes**:
- Electrical noise
- Poor CT installation
- Loose connections
- Software issues

**Solutions**:
1. Add/check 100nF capacitor for noise filtering
2. Verify CT wired correctly (tip → burden → ADC, sleeve → GND)
3. Check all connections tight, good solder joints
4. Increase sampling rate or sample count in firmware
5. Use oscilloscope to check for noise on ADC pin

### Issue: Readings Drift Over Time

**Possible Causes**:
- Temperature variation
- Component aging
- CT demagnetization

**Solutions**:
1. Implement temperature compensation in firmware
2. Recalibrate annually
3. Store in stable temperature environment
4. Consider temperature sensor for real-time correction

---

## 📋 Calibration Checklist

Print this checklist for each device:

### Pre-Calibration
- [ ] Hardware verified (CT, burden resistor, ADC circuit)
- [ ] Environmental conditions recorded
- [ ] Reference meter verified (calibration current)
- [ ] Safety equipment ready
- [ ] Test loads available

### Phase 1: Zero Offset
- [ ] NO load connected
- [ ] Zero calibration run
- [ ] Result: _______ V (1.60-1.70V?)
- [ ] Standard deviation <5mV?

### Phase 2: Single-Point (60W)
- [ ] 60W load connected
- [ ] Reference reading: _______ A
- [ ] Calibration run
- [ ] Error: _______ % (<2%?)

### Phase 3: Verification (100W)
- [ ] 100W load connected
- [ ] Reference reading: _______ A
- [ ] Device reading: _______ A
- [ ] Error: _______ % (<2%?)

### Phase 4: Multi-Point (Optional)
- [ ] 200W: Error _______ %
- [ ] 500W: Error _______ %
- [ ] 1000W: Error _______ %
- [ ] Linearity correction applied

### Phase 5: High Current (Optional)
- [ ] 10A: Error _______ %, Temp _______ °C
- [ ] 20A: Error _______ %, Temp _______ °C
- [ ] 30A: Error _______ %, Temp _______ °C

### Save & Document
- [ ] Calibration saved to EEPROM
- [ ] Certificate generated
- [ ] Certificate saved to file
- [ ] Certificate uploaded to backend (optional)
- [ ] Device ID and calibration date labeled on enclosure

### Final Verification
- [ ] Random load tests passed
- [ ] All errors within specification
- [ ] Long-term stability monitoring scheduled
- [ ] Next calibration date scheduled: ____________

---

## 📞 Support

**Questions**:
- Hardware calibration: hardware@smartplugai.com
- Firmware issues: firmware@smartplugai.com

**Resources**:
- SCT-013 Guide: http://openenergymonitor.org/emon/buildingblocks
- ESP32 ADC Calibration: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html

---

**Document Version**: 1.0  
**Last Updated**: 2024-01-15  
**Author**: Smart Plug AI Development Team
