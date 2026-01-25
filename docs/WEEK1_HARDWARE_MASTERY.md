# WEEK 1: HARDWARE MASTERY & BASIC FIRMWARE - DEEP DIVE

📅 **Timeline**: Days 1-7  
🎯 **Goal**: Build perfectly assembled smart plug prototype with secure firmware foundation

## 📅 DAY 1-2: PERFECT SOLDERING & ASSEMBLY

### MORNING (9AM-12PM): COMPONENT PREPARATION

#### Step 1: Workspace Setup

```text
Checklist - Workspace Preparation:
[√] Clean, well-lit table with anti-static mat
[√] Temperature-controlled soldering iron (350°C for lead-free)
[√] Fine tip (0.5mm) and chisel tip (2mm)
[√] Lead-free solder (0.7mm diameter)
[√] Flux pen, solder wick, 99% isopropyl alcohol
[√] Magnifying glass or head-mounted magnifier
[√] Multimeter (auto-ranging)
[√] Tweezers, wire strippers, helping hands
[√] Fume extractor or good ventilation
```

**Workspace Layout Diagram:**
```
┌─────────────────────────────────────────────────────────┐
│                    WORKBENCH LAYOUT                      │
├─────────────────────────────────────────────────────────┤
│                                                          │
│  [Fume Extractor]        [Magnifying Lamp]             │
│         ↓                        ↓                      │
│  ┌──────────────────────────────────────────┐          │
│  │                                           │          │
│  │     [Anti-Static Mat]                    │          │
│  │                                           │          │
│  │  [Helping Hands] [Component Tray]        │          │
│  │                                           │          │
│  │  [Solder Iron   [Work Area]  [Multimeter]│          │
│  │   & Stand]                               │          │
│  │                                           │          │
│  │  [Flux & Wick]  [Components]  [Wire Strip│          │
│  │                               & Cutters]  │          │
│  └──────────────────────────────────────────┘          │
│                                                          │
│  Left: Tools | Center: Assembly | Right: Testing       │
└─────────────────────────────────────────────────────────┘
```

#### Step 2: Component Verification

**Test EVERY component before soldering:**

##### ESP32-S3-WROOM-1-N8:

```bash
1. Visual inspection: No bent pins, clean solder pads
2. Resistance test: GND to 3.3V should be >100Ω (no short)
3. USB test: Connect to computer, check COM port appears
```

**ESP32-S3 Pin Inspection Diagram:**
```
     ESP32-S3-WROOM-1-N8 (Top View)
    ┌───────────────────────────┐
    │  ┌─────────────────────┐  │
    │  │    ESP32-S3         │  │
    │  │    Antenna          │  │
    │  │    Area             │  │
    │  └─────────────────────┘  │
    │                            │
    │ Pin 1  [•]  [•]  [•] ...  │
    │ 3.3V   [•]  [•]  [•] ...  │
    │ EN     [•]  [•]  [•] ...  │
    │ GPIO   [•]  [•]  [•] ...  │
    │  ...   [•]  [•]  [•] ...  │
    │ GND    [•]  [•]  [•] ...  │
    └───────────────────────────┘
     ▲                       ▲
     └─ Check for bent pins ─┘
        All should be straight
```

##### ATECC608A Breakout Board:

```bash
1. I2C address test: Connect 3.3V, GND, SDA, SCL to ESP32
2. Scan I2C: Should show device at 0x60 (C0 >> 1)
   # Use I2C scanner sketch
```

**ATECC608A Wiring Test:**
```
ESP32-S3              ATECC608A
┌─────────┐          ┌──────────┐
│         │          │          │
│ 3.3V ───┼─────────→│ VCC      │
│         │          │          │
│ GND  ───┼─────────→│ GND      │
│         │          │          │
│ GPIO8───┼─────────→│ SDA      │
│ (SDA)   │    2.2kΩ │ ↑        │
│         │    ┌─────┤ │        │
│ GPIO9───┼────┼────→│ SCL      │
│ (SCL)   │    │2.2kΩ│ ↑        │
│         │    │ ┌───┤ │        │
│         │    └─┴───┼→3.3V     │
│         │          │          │
└─────────┘          └──────────┘

Pull-up resistors: 2.2kΩ to 3.3V on both SDA and SCL
```

##### SCT-013-030 Current Sensor:

```bash
1. Continuity test: Primary winding (split core) should be open circuit
2. Resistance test: Secondary winding (3.5mm jack) ~30-40Ω
3. Polarity test: White stripe indicates current direction
```

**SCT-013 Physical Check:**
```
    SCT-013-030 Current Sensor
    ┌─────────────────────────┐
    │    ╱╲     Split Core    │  ← White stripe
    │   ╱  ╲    (Clamp)       │     (current direction)
    │  │    │                 │
    │  │ ◉  │ ← AC wire goes  │
    │  │    │   through here  │
    │   ╲  ╱                  │
    │    ╲╱                   │
    └─────┬───────────────────┘
          │
          │ 3.5mm jack cable
          │ (Secondary output)
          ▼
       ┌─────┐
       │ TIP │ ← Signal
       │RING │ ← Not used
       │SLVE │ ← Ground
       └─────┘
```

##### TPS3823 + SOT23 Adapter:

```c
// CRITICAL: Micro-soldering practice first!
1. Practice on dead electronics with similar SOT23 package
2. Use microscope/magnifier
3. Apply flux, tack one pin, align, solder others
```

**SOT23 Package Soldering Guide:**
```
    SOT23-5 Package (Actual size ~3mm)
    
    Top View:              Side View:
    ┌─────┐                    [Chip]
 1 ─┤ •   ├─ 5               ═╪═ Pin
 2 ─┤     ├─ 4                │
 3 ─┤     ├─                  ▼ PCB
    └─────┘                  ═════

Soldering Technique:
1. Apply flux to pads
2. Tack pin 1 with tiny solder dot
3. Align chip while reheating pin 1
4. Solder remaining pins with drag technique
5. Inspect under magnification for bridges
```

#### Step 3: Pre-Tinning

**Pre-tin these BEFORE assembly:**

```text
1. ESP32-S3 pins - Light coat on all pins
2. SOT23 adapter pads - Tiny dots of solder
3. Wire ends for power/ground connections
4. Terminal blocks screw terminals
```

**Pre-Tinning Technique:**
```
Good Pre-Tinning:        Bad Pre-Tinning:
     
     Shiny, thin coat       Too much solder
          ╱╲                     ╱══╲
    Pin  ││  Pin            Pin  ███  Pin
         ││                      ███
    ═════╧╧═════           ══════╧══════
    PCB   ▲▲  PCB          PCB      PCB
      Smooth dome             Blob

Temperature: 350°C
Time: 1-2 seconds per pin
Solder: 60/40 or lead-free 0.7mm
```

### AFTERNOON (1PM-5PM): ASSEMBLY

#### Assembly Sequence (DO NOT DEVIATE):

##### Stage 1: ESP32-S3 Base Board

```text
1. Insert female headers into breadboard
2. Place ESP32-S3 on headers
3. Solder ONE pin at each corner to secure
4. Check alignment, then solder all pins
5. Inspect: No bridges, shiny joints, no cold solder
```

**Breadboard Layout:**
```
     Breadboard (830 points)
┌────────────────────────────────┐
│ + + + + + + + + + + + + + + +  │ ← 3.3V Rail (Red)
│ - - - - - - - - - - - - - - -  │ ← GND Rail (Black)
│                                │
│    ESP32-S3 Placement:         │
│  ┌─────────────────────────┐  │
│  │ [ESP32-S3-WROOM-1-N8]   │  │
│  │                         │  │
│  │ [Pin headers soldered]  │  │
│  │ [to board below]        │  │
│  └─────────────────────────┘  │
│  ║ ║ ║ ║ ║ ║ ║ ║ ║ ║ ║ ║ ║  │
│  • • • • • • • • • • • • • •  │ ← Breadboard holes
│                                │
│ - - - - - - - - - - - - - - -  │ ← GND Rail (Black)
│ + + + + + + + + + + + + + + +  │ ← 5V Rail (Blue)
└────────────────────────────────┘
```

##### Stage 2: Power Distribution

```c
// Add power rails to breadboard
1. Red wire: 3.3V rail (from ESP32 3.3V pin)
2. Black wire: GND rail (from ESP32 GND)
3. Blue wire: 5V rail (from external 5V supply)
4. Add 100µF capacitor between 3.3V and GND
5. Add 0.1µF capacitor near each IC power pin
```

**Power Distribution Diagram:**
```
External 5V Supply           ESP32-S3
┌──────────┐                ┌─────────┐
│  5V/2A   │                │         │
│ Charger  │                │  3.3V ──┼──┐
│          │                │  REG    │  │
│  +5V ────┼────────────────┼→ VIN    │  │
│          │    │           │         │  │
│  GND ────┼────┼───────────┼→ GND    │  │
└──────────┘    │           └─────────┘  │
                │                        │
        ┌───────┴───────┐        ┌──────┴──────┐
        │               │        │             │
    5V Rail          GND Rail  3.3V Rail      │
    (Blue)           (Black)   (Red)          │
        │               │        │             │
        ├───────────────┼────────┼─────────┐   │
        │               │        │         │   │
     [Relay]       [100µF Cap]  │      [ATECC] │
      Coil         3.3V-GND     │      [24C256]│
                                └──────────────┘
                                Decoupling: 0.1µF
```

##### Stage 3: ATECC608A Integration

```text
Wire ATECC608A to ESP32-S3:
ATECC608A → ESP32-S3
VCC    → 3.3V
GND    → GND
SDA    → GPIO8 (I2C_SDA)
SCL    → GPIO9 (I2C_SCL)

ADD: 2.2kΩ pull-up resistors from SDA/SCL to 3.3V
```

**I2C Bus Wiring:**
```
        3.3V Rail
           │
      ┌────┴────┐
    2.2kΩ     2.2kΩ
      │         │
      │         │
┌─────┴─────────┴─────┐
│                     │
│   I2C Bus (Shared)  │
│                     │
├──SDA────────────────┤
│  GPIO8              │
│   │                 │
│   ├─→ ATECC608A SDA │
│   ├─→ 24C256 SDA    │
│   └─→ (Future I2C)  │
│                     │
├──SCL────────────────┤
│  GPIO9              │
│   │                 │
│   ├─→ ATECC608A SCL │
│   ├─→ 24C256 SCL    │
│   └─→ (Future I2C)  │
└─────────────────────┘

Note: All I2C devices share same SDA/SCL lines
Unique addresses: ATECC608A (0x60), 24C256 (0x50)
```

##### Stage 4: Current Sensor Circuit

```text
SCT-013-030 Circuit:
1. 3.5mm jack tip → 33Ω 1W burden resistor → ESP32 GPIO4 (ADC1_CH3)
2. 3.5mm jack sleeve → GND
3. Add 100nF capacitor from ADC pin to GND (noise filtering)
4. Add 1N4007 diode across burden resistor (reverse polarity protection)

CALIBRATION POINT: Mark "SCT-013 Input" on breadboard
```

**Current Sensor Circuit Diagram:**
```
SCT-013-030                    ESP32-S3
(AC Current)                   ADC Input
┌──────────┐                  ┌─────────┐
│  Clamp   │                  │         │
│  around  │                  │ GPIO4 ←─┼─── Voltage
│  AC wire │                  │ (ADC1_  │    Reading
│          │                  │  CH3)   │
│   TIP ───┼──┐               │         │
│          │  │               │ GND   ──┼──┐
│  SLEEVE ─┼──┼──────┐        └─────────┘  │
└──────────┘  │      │                     │
              │      │                     │
           [33Ω 1W]  │                     │
           Burden    │                     │
           Resistor  │                     │
              │      │                     │
              ├──────┤                     │
              │   [1N4007]                 │
              │   Protection               │
              │   Diode                    │
              │      │                     │
              └──────┴─────────────────────┘
                     │
                 [100nF Cap]
                 Noise Filter
                     │
                    GND

Voltage at ADC = Current × 33Ω × Turns Ratio
For SCT-013-030: 30A → 1V output
Calibration factor: 1V/30A = 0.0333V/A
```

##### Stage 5: Relay Control with Optocoupler

```text
Relay Control Circuit:
ESP32 GPIO18 → 220Ω resistor → PC817 LED anode
PC817 LED cathode → GND

PC817 transistor collector → Relay coil+
Relay coil- → 5V rail
Relay COM → AC input (LATER)
Relay NO → AC output (LATER)

ADD: 1N4007 diode across relay coil (flyback protection)
```

**Relay Control Circuit:**
```
ESP32-S3                PC817              Relay Module
┌─────────┐          ┌─────────┐         ┌──────────┐
│         │          │   LED   │         │          │
│ GPIO18 ─┼──[220Ω]─→│  Anode  │         │  Coil+   │
│         │          │    ↓    │         │    │     │
│ GND    ─┼─────────→│ Cathode │         │  [1N4007]│
│         │          │         │         │    │     │
└─────────┘          │  Photo  │         │  Coil-   │
                     │  Trans. │    5V →─┼→   │     │
                     │    ├────┼─────────┼→ Coll.   │
                     │    └────┼─────────┼→ Emit.   │
                     └─────────┘         │          │
                          Optical        │ COM  NO  │
                          Isolation      │  │    │  │
                                         │  │    │  │
                                         └──┼────┼──┘
                                            │    │
                                         AC IN  AC OUT
                                        (Connect LATER
                                         in AC phase)

Flyback diode protects against voltage spikes
when relay coil is de-energized.
```

##### Stage 6: TPS3823 Watchdog Circuit

```text
TPS3823 Wiring (on adapter board):
Pin 1 (VDD) → 3.3V
Pin 2 (GND) → GND
Pin 3 (MR) → GPIO10 (tamper switch input)
Pin 4 (WDI) → GPIO11 (watchdog toggle)
Pin 5 (/RST) → ESP32 EN pin (via 10kΩ pull-up to 3.3V)

Tamper switch: Normally closed across MR and GND
```

**Watchdog & Tamper Detection:**
```
        3.3V
         │
      [10kΩ]
         │
         ├──────────────┐
         │              │
    TPS3823           ESP32-S3
   ┌─────────┐       ┌─────────┐
   │ VDD  (1)├───┐   │         │
   │         │   │   │  EN ────┼←── Reset input
   │ GND  (2)├───┼───┼→ GND    │
   │         │   │   │         │
   │ MR   (3)├───┼───┼← GPIO10 │ (Tamper)
   │         │   │   │         │
   │ WDI  (4)├───┼───┼← GPIO11 │ (Watchdog Toggle)
   │         │   │   │         │
   │/RST  (5)├───┴───┼→ EN     │
   └─────────┘       └─────────┘
         │
    [Tamper SW]
    Normally
    Closed
         │
        GND

Tamper Switch opens when enclosure is opened,
pulling MR high, triggering reset via /RST pin.
```

##### Stage 7: EEPROM Storage

```text
24C256 EEPROM:
VCC → 3.3V
GND → GND
SDA → GPIO8 (shared with ATECC608A)
SCL → GPIO9 (shared with ATECC608A)
A0,A1,A2 → GND (address 0x50)

NOTE: I2C bus now has 3 devices (ATECC608A, EEPROM, ESP32)
```

**Complete I2C Bus:**
```
       3.3V
         │
    ┌────┴────┐
  2.2kΩ     2.2kΩ
    │         │
    │    Pull-up Resistors
    │         │
┌───┴─────────┴───────────────────────┐
│         I2C Bus                      │
│   SDA (GPIO8)    SCL (GPIO9)        │
├─────┬────────────────┬───────────────┤
│     │                │               │
│  ATECC608A        24C256         [Future]
│  Address: 0x60   Address: 0x50   Devices
│  (Secure Keys)   (Config Store)          │
│     │                │               │
│   VCC,GND         VCC,GND          │
│     │                │               │
│     │            A0,A1,A2           │
│     │               │               │
│     │              GND              │
└─────┴────────────────┴───────────────┘

Each device has unique I2C address.
All share same SDA/SCL lines.
```

### Visual Inspection Checklist

```bash
[ ] No solder bridges between pins
[ ] All joints shiny and concave
[ ] No cold solder joints (dull, cracked)
[ ] Wires neatly routed, not touching
[ ] Components properly oriented
[ ] No flux residue (clean with alcohol)
[ ] Labels for all connections
```

**Solder Joint Quality Guide:**
```
GOOD Joints:          BAD Joints:

Shiny & Smooth        Dull (Cold Joint)
     ╱╲                    ╱══╲
  Pin││Pin             Pin ████ Pin
     ││                   ████
  ═══╧╧═══            ═════╧═════
      ▲                     ▲
   Perfect!             Redo this!

Concave Fillet        Too Much Solder
     ╱╲                    ╱══╲
  Pin││Pin             Pin ███  Pin
    ╱  ╲                  ███
  ═══  ═══            ════════════
      ▲                     ▲
   Perfect!              Excess

Bridge (SHORT!)       Rosin Joint
  Pin││  ││Pin         Pin  ╱╲  Pin
     ████████             ▓▓▓▓
  ═══════════         ═══════════
        ▲                   ▲
   DANGEROUS!          Redo this!
```

### Continuity Tests

```bash
# POWER TESTS
1. 3.3V to GND: Should be >50Ω (no direct short)
2. 5V to GND: Should be >100Ω
3. Check no continuity between 3.3V and 5V rails

# SIGNAL TESTS  
1. I2C lines: SDA/SCL to 3.3V should be ~2.2kΩ (pull-ups)
2. ADC pin: Should show 33Ω to GND (through burden resistor)
3. Relay control: GPIO18 to GND should be ~220Ω (through opto LED)
```

## 📅 DAY 3-4: ESP-IDF DEEP DIVE

### ESP-IDF Setup (Linux/Windows/WSL2)

```bash
# Clean installation
mkdir ~/esp
cd ~/esp
git clone -b v5.2.1 --recursive https://github.com/espressif/esp-idf.git
cd esp-idf
./install.sh esp32s3  # Install for ESP32-S3 only

# Add to PATH (add to ~/.bashrc)
echo 'alias get_idf=". $HOME/esp/esp-idf/export.sh"' >> ~/.bashrc
source ~/.bashrc
get_idf
```

### Project Creation & Secure Boot Configuration

```bash
# Create project
cd ~/esp
cp -r $IDF_PATH/examples/get-started/hello_world smartplug-secure
cd smartplug-secure

# Configure for ESP32-S3 with security
idf.py set-target esp32s3
idf.py menuconfig
```

### Critical menuconfig Settings

```text
1. Component config → ESP32-S3-specific:
   - CPU frequency: 240 MHz
   - Flash SPI mode: QIO
   - Flash size: 8MB
   - Partition Table: Custom (edit partitions.csv)

2. Security features:
   - Enable hardware Secure Boot in bootloader
   - Enable flash encryption on boot
   - Enable NVS encryption
   - Encryption mode: Development (for now)
   - Enable secure boot v2
   - Secure bootloader mode: One-time flash

3. Component config → mbedTLS:
   - Enable hardware acceleration
   - Enable ATECC608A support
   - Certificate bundle: Enabled
   - TLS 1.3: Enabled

4. Component config → ESP-TLS:
   - Use secure element: ATECC608A
   - Enable server verification
   - Enable client verification

5. Serial flasher config:
   - Flash SPI speed: 80MHz
   - Verify flash on boot: Enabled
```

### ATECC608A Provisioning (MOST CRITICAL STEP)

```python
# Create provisioning script: provision_atecc608a.py
import subprocess
import secrets

# 1. Generate device unique ID
device_id = secrets.token_hex(8)
print(f"Device ID: {device_id}")

# 2. Generate ATECC608A configuration
config = f"""
; ATECC608A-TNGTLS Configuration
; Device: {device_id}
; Date: $(date)

[0:0] ; Slot 0 - Device Certificate
KeyType=ECC_P256
WriteConfig=Never
EncryptRead=Enabled
IsSecret=No
WriteKey=0
ReadKey=0

[0:1] ; Slot 1 - Private Key (NEVER READABLE)
KeyType=ECC_P256  
WriteConfig=Never
EncryptRead=Enabled
IsSecret=Yes
WriteKey=0
ReadKey=Never

[0:2] ; Slot 2 - TLS Session Keys
KeyType=AES
WriteConfig=Encrypt
EncryptRead=Enabled
IsSecret=Yes

; Lock configuration zone
LockConfig=Yes
LockData=Yes
LockValue=0x0123
"""

with open(f"atecc_config_{device_id}.txt", "w") as f:
    f.write(config)
```

### Secure Boot Key Generation

```bash
# Generate signing key (KEEP THIS SECURE!)
openssl ecparam -name prime256v1 -genkey -noout -out secure_boot_signing_key.pem

# Generate flash encryption key  
python $IDF_PATH/components/esptool_py/esptool/espsecure.py generate_flash_encryption_key flash_encryption_key.bin

# Generate NVS encryption key
python $IDF_PATH/components/nvs_flash/nvs_partition_generator/nvs_partition_gen.py generate-key --key_name nvs_key --keyfile nvs_key.bin

# Store keys in secure location
mkdir -p ~/esp/keys
mv *.pem *.bin ~/esp/keys/
chmod 600 ~/esp/keys/*
```

### Partition Table (partitions.csv)

```csv
# Name, Type, SubType, Offset, Size, Flags
nvs,      data, nvs,      0x9000,  0x6000,
otadata,  data, ota,      0xf000,  0x2000,
phy_init, data, phy,      0x11000, 0x1000,
secure,   data, nvs,      0x12000, 0x4000, encrypted
factory,  app,  factory,  0x20000, 1M,
ota_0,    app,  ota_0,    ,        1M,
ota_1,    app,  ota_1,    ,        1M,
coredump, data, coredump, ,        64K,
spiffs,   data, spiffs,   ,        1M,
```

### Day 4: Hardware Drivers Implementation

#### 1. ATECC608A Driver (main/components/cryptoauth/)

```c
// atecc608a.c
#include "cryptoauthlib.h"

static const char *TAG = "ATECC608A";

esp_err_t atecc608a_init() {
    ATCAIfaceCfg cfg = {
        .iface_type = ATCA_I2C_IFACE,
        .devtype = ATECC608A,
        .atcai2c.slave_address = 0xC0,
        .atcai2c.bus = I2C_NUM_0,
        .atcai2c.baud = 100000,
        .wake_delay = 1500,
        .rx_retries = 20
    };
    
    ATCA_STATUS status = atcab_init(&cfg);
    if (status != ATCA_SUCCESS) {
        ESP_LOGE(TAG, "ATECC608A init failed: %d", status);
        return ESP_FAIL;
    }
    
    // Check if device is configured
    uint8_t config_data[128];
    status = atcab_read_config_zone(config_data);
    if (status == ATCA_SUCCESS) {
        ESP_LOGI(TAG, "ATECC608A configured and ready");
    } else {
        ESP_LOGW(TAG, "ATECC608A needs provisioning");
        return ESP_ERR_NOT_FINISHED;
    }
    
    return ESP_OK;
}

esp_err_t atecc608a_generate_key_pair(uint8_t slot, uint8_t *public_key) {
    return atcab_genkey(slot, public_key);
}

esp_err_t atecc608a_sign_data(uint8_t slot, const uint8_t *data, size_t data_len, uint8_t *signature) {
    uint8_t digest[32];
    atcab_sha(data_len, data, digest);
    return atcab_sign(slot, digest, signature);
}
```

#### 2. Current Sensor Driver with True RMS

```c
// current_sensor.c
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define CURRENT_SENSOR_ADC_CHANNEL ADC1_CHANNEL_3
#define ADC_ATTEN ADC_ATTEN_DB_11
#define ADC_WIDTH ADC_WIDTH_BIT_12
#define SAMPLE_COUNT 1000
#define LINE_FREQUENCY 50.0
#define SAMPLES_PER_CYCLE 200  // 10kHz sampling

typedef struct {
    float zero_offset;      // ADC reading at 0A
    float scale_factor;     // V/A
    float phase_correction; // degrees
    float temperature_coeff;
    uint32_t calibration_time;
} calibration_t;

static calibration_t calib;
static esp_adc_cal_characteristics_t *adc_chars;

void current_sensor_init() {
    // ADC calibration
    adc1_config_width(ADC_WIDTH);
    adc1_config_channel_atten(CURRENT_SENSOR_ADC_CHANNEL, ADC_ATTEN);
    
    // Characterize ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN, ADC_WIDTH, 
                            ESP_ADC_CAL_VAL_DEFAULT_VREF, adc_chars);
    
    // Load calibration
    load_calibration(&calib);
}

float read_current_rms() {
    int32_t samples[SAMPLES_PER_CYCLE];
    
    // Sample one complete cycle
    for (int i = 0; i < SAMPLES_PER_CYCLE; i++) {
        samples[i] = adc1_get_raw(CURRENT_SENSOR_ADC_CHANNEL);
        ets_delay_us(100); // 10kHz sampling
    }
    
    // Calculate RMS
    float sum_squares = 0;
    for (int i = 0; i < SAMPLES_PER_CYCLE; i++) {
        float voltage = esp_adc_cal_raw_to_voltage(samples[i], adc_chars) / 1000.0;
        sum_squares += voltage * voltage;
    }
    
    float voltage_rms = sqrt(sum_squares / SAMPLES_PER_CYCLE);
    float current = (voltage_rms - calib.zero_offset) / calib.scale_factor;
    
    return current;
}

void calibrate_zero_offset() {
    ESP_LOGI(TAG, "Calibrating zero offset - ensure NO LOAD connected");
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    
    float sum = 0;
    for (int i = 0; i < 1000; i++) {
        int raw = adc1_get_raw(CURRENT_SENSOR_ADC_CHANNEL);
        sum += esp_adc_cal_raw_to_voltage(raw, adc_chars) / 1000.0;
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    
    calib.zero_offset = sum / 1000.0;
    ESP_LOGI(TAG, "Zero offset: %.3fV", calib.zero_offset);
}
```

#### 3. Relay Control with Soft Start

```c
// relay_control.c
#include "driver/gpio.h"

#define RELAY_PIN GPIO_NUM_18
#define SOFT_START_DELAY_MS 100

void relay_init() {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << RELAY_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);
    gpio_set_level(RELAY_PIN, 0);
}

void relay_set(bool state, bool soft_start) {
    if (soft_start && state) {
        // Soft start for inductive loads
        for (int i = 0; i < 10; i++) {
            gpio_set_level(RELAY_PIN, 1);
            vTaskDelay(SOFT_START_DELAY_MS / 10 / portTICK_PERIOD_MS);
            gpio_set_level(RELAY_PIN, 0);
            vTaskDelay(SOFT_START_DELAY_MS / 10 / portTICK_PERIOD_MS);
        }
    }
    
    gpio_set_level(RELAY_PIN, state ? 1 : 0);
    ESP_LOGI(TAG, "Relay %s", state ? "ON" : "OFF");
}
```

#### 4. Watchdog Driver

```c
// watchdog.c
#include "driver/gpio.h"

#define WATCHDOG_PIN GPIO_NUM_11
#define TAMPER_PIN GPIO_NUM_10
#define WATCHDOG_TOGGLE_INTERVAL_MS 500

static bool tamper_detected = false;

void watchdog_init() {
    // Watchdog toggle output
    gpio_config_t wd_conf = {
        .pin_bit_mask = (1ULL << WATCHDOG_PIN),
        .mode = GPIO_MODE_OUTPUT,
    };
    gpio_config(&wd_conf);
    
    // Tamper detection input (normally closed switch)
    gpio_config_t tamper_conf = {
        .pin_bit_mask = (1ULL << TAMPER_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .intr_type = GPIO_INTR_NEGEDGE,
    };
    gpio_config(&tamper_conf);
    
    // Install ISR
    gpio_install_isr_service(0);
    gpio_isr_handler_add(TAMPER_PIN, tamper_isr_handler, NULL);
}

void IRAM_ATTR tamper_isr_handler(void* arg) {
    tamper_detected = true;
    ESP_LOGE(TAG, "TAMPER DETECTED! Enclosure opened.");
    
    // Immediate actions
    wipe_sensitive_data();
    log_tamper_event();
    
    // Stop toggling watchdog - let it reset system
    vTaskSuspend(watchdog_task_handle);
}

void watchdog_task(void* pvParameter) {
    while (1) {
        if (!tamper_detected) {
            gpio_set_level(WATCHDOG_PIN, 1);
            vTaskDelay(WATCHDOG_TOGGLE_INTERVAL_MS / 2 / portTICK_PERIOD_MS);
            gpio_set_level(WATCHDOG_PIN, 0);
            vTaskDelay(WATCHDOG_TOGGLE_INTERVAL_MS / 2 / portTICK_PERIOD_MS);
        } else {
            vTaskDelay(portMAX_DELAY);
        }
    }
}
```

## 📅 DAY 5-7: FIRST POWER-ON & CALIBRATION

### Safety-First Power-On Sequence

#### Step 1: Pre-Power Safety Checks

```bash
# VISUAL INSPECTION
1. No loose wires or solder balls
2. No components touching heatsinks/ground
3. All polarized components correct orientation
4. Fuse installed (3A)
5. MOV installed across Live/Neutral

# MULTIMETER TESTS
1. 3.3V to GND: >50Ω
2. 5V to GND: >100Ω  
3. Live to Neutral: OPEN CIRCUIT (relay off)
4. Live to Earth: >1MΩ
5. Neutral to Earth: >1MΩ

# ISOLATION TEST (if you have megohmmeter)
1. Live to DC ground: >10MΩ at 500V
2. Neutral to DC ground: >10MΩ at 500V
```

#### Step 2: DC-Only Power Test

```c
// Test firmware: test_dc_only.c
void app_main() {
    printf("=== DC-ONLY SAFETY TEST ===\n");
    
    // 1. Test ESP32 basic functions
    printf("1. Testing ESP32...\n");
    test_esp32_basic();
    
    // 2. Test I2C devices
    printf("2. Scanning I2C bus...\n");
    i2c_scan();
    
    // 3. Test ADC readings
    printf("3. Testing ADC (should be ~1.65V with no load)...\n");
    test_adc_zero();
    
    // 4. Test relay (optocoupler LED only)
    printf("4. Testing relay control (optocoupler LED)...\n");
    test_relay_control();
    
    // 5. Test watchdog
    printf("5. Testing watchdog toggle...\n");
    test_watchdog();
    
    printf("=== DC TESTS COMPLETE ===\n");
    printf("If all tests pass, proceed to Step 3\n");
}
```

#### Step 3: Add 5V Power Supply

```text
ACTION: Connect certified 5V/2A phone charger
TESTS:
1. Measure 5V output: Should be 5.0V ±5%
2. Measure ripple: <50mV p-p
3. Check temperature: Charger should not get hot
4. Verify ESP32 still boots
5. Verify relay coil gets 5V
```

#### Step 4: Relay Switching Test (NO AC LOAD)

```c
// Test relay switching with scope/multimeter
void test_relay_switching() {
    printf("Testing relay switching (NO AC CONNECTED):\n");
    
    for (int i = 0; i < 5; i++) {
        printf("Cycle %d: ", i+1);
        relay_set(true, false);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        
        // Verify relay clicked
        printf("ON - ");
        
        relay_set(false, false);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        printf("OFF\n");
    }
}
```

#### Step 5: Current Sensor Calibration

**Calibration Procedure:**

```c
void perform_full_calibration() {
    ESP_LOGI(TAG, "=== CURRENT SENSOR CALIBRATION ===");
    
    // PHASE 1: Zero offset (no load)
    calibrate_zero_offset();
    
    // PHASE 2: Known load calibration
    ESP_LOGI(TAG, "Connect 60W incandescent bulb (230V, 0.26A)");
    ESP_LOGI(TAG, "Press button when ready...");
    wait_for_button();
    
    float load_current = 0.26; // 60W at 230V
    calibrate_with_known_load(load_current);
    
    // PHASE 3: Verify with second known load
    ESP_LOGI(TAG, "Connect 100W bulb (230V, 0.435A)");
    ESP_LOGI(TAG, "Press button when ready...");
    wait_for_button();
    
    verify_calibration(0.435);
    
    // PHASE 4: Save calibration to EEPROM
    save_calibration_to_eeprom();
    
    // PHASE 5: Generate calibration certificate
    generate_calibration_certificate();
}
```

**Calibration Certificate Template:**

```json
{
  "device_id": "SP001",
  "calibration_date": "2024-01-15",
  "calibrated_by": "Your Name",
  "reference_loads": [
    {
      "type": "incandescent_bulb",
      "power_w": 60,
      "expected_current_a": 0.261,
      "measured_current_a": 0.259,
      "error_percent": 0.77
    },
    {
      "type": "incandescent_bulb", 
      "power_w": 100,
      "expected_current_a": 0.435,
      "measured_current_a": 0.433,
      "error_percent": 0.46
    }
  ],
  "calibration_constants": {
    "zero_offset_v": 1.652,
    "scale_factor_v_per_a": 0.0332,
    "temperature_coeff": 0.001,
    "r_squared": 0.9998
  },
  "notes": "Calibrated with Fluke 87V multimeter reference"
}
```

### Day 7: Comprehensive Testing

#### Test Suite Execution

```python
# test_suite.py
tests = {
    "hardware": [
        ("Power Supply", test_power_supply),
        ("Relay Switching", test_relay_switching),
        ("Current Sensor", test_current_sensor),
        ("Watchdog", test_watchdog),
        ("Tamper Detection", test_tamper),
    ],
    "software": [
        ("Secure Boot", test_secure_boot),
        ("Flash Encryption", test_flash_encryption),
        ("ATECC608A", test_atecc608a),
        ("TLS Handshake", test_tls_handshake),
    ],
    "safety": [
        ("Over-Current", test_overcurrent),
        ("Isolation", test_isolation),
        ("Temperature", test_temperature),
        ("Response Time", test_response_time),
    ]
}

def run_all_tests():
    results = {}
    for category, test_list in tests.items():
        print(f"\n=== {category.upper()} TESTS ===")
        for test_name, test_func in test_list:
            print(f"\n{test_name}...", end="")
            try:
                result = test_func()
                results[f"{category}.{test_name}"] = result
                print(" PASS" if result else " FAIL")
            except Exception as e:
                print(f" ERROR: {e}")
                results[f"{category}.{test_name}"] = False
    return results
```

#### Final Hardware Validation Checklist

```bash
[√] ESP32 boots with secure boot
[√] ATECC608A detected and responding
[√] Current sensor reads 0.00A with no load
[√] Current sensor reads 0.26A ±2% with 60W bulb
[√] Relay switches audibly
[√] Watchdog toggling (measure with scope)
[√] Tamper detection triggers on switch open
[√] EEPROM reads/writes data
[√] Temperature sensor reading (ESP32 internal)
[√] All LEDs illuminate correctly
[√] No components overheating
[√] No audible buzzing or arcing
[√] Enclosure closes properly
[√] Tamper switch engages when closed
```

## 🎯 WEEK 1 DELIVERABLES

### Hardware:
- One perfectly assembled smart plug prototype
- Calibration certificate with <2% accuracy
- High-resolution assembly photos/video
- Test results document

### Software:
ESP-IDF project with:
- Secure boot enabled
- Flash encryption working
- ATECC608A integration
- Current sensor driver with calibration
- Relay control with soft start
- Watchdog and tamper detection
- EEPROM storage

### Documentation:
- Assembly guide with photos
- Calibration procedure
- Test results
- Bill of Materials (BOM)
- Circuit diagrams

### Safety Certification:
- Isolation test results
- Over-current protection test
- Tamper response verification
- Watchdog functionality proof

## ⚠️ CRITICAL WARNINGS FOR WEEK 1

### ATECC608A:
- **BACKUP CONFIGURATION** before locking
- **TEST PROVISIONING** on breadboard first
- **DON'T LOCK** until you're 100% sure

### Power Safety:
- **NEVER** work on AC side while powered
- Use **ISOLATION TRANSFORMER** if available
- Start with **GFCI/RCD PROTECTED** outlet
- **ONE HAND IN POCKET** rule when testing AC

### Soldering:
- **NO COLD SOLDER JOINTS** - redo if not perfect
- **CHECK FOR BRIDGES** with magnifier
- **TEST CONTINUITY** after each section
- **DON'T RUSH** - quality over speed

---

**Version**: 1.0  
**Last Updated**: January 2026  
**Next Review**: After Week 1 completion
