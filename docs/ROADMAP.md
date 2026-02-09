# Smart Plug AI Roadmap

This roadmap outlines the phased development plan for Smart Plug AI, with a **security-first approach** from Day 1. The project is structured in three phases over 12 months, with detailed week-by-week milestones for Phase 1.

## 🎯 Vision

Build South Africa's most secure IoT platform for smart plug systems that provides real-time power monitoring, intelligent insights, and seamless user experience across mobile and web platforms, with bank-grade security built-in from the ground up.

---

## 📊 Phase Overview

| Phase | Timeline | Budget | Focus |
|-------|----------|--------|-------|
| **Phase 1** | Now - 7 weeks | R5,000 | Secure MVP Development (3-4 person team) |
| **Phase 2** | Months 5-8 | R3,000,000 | Pilot & Refinement |
| **Phase 3** | Months 9-12 | R6,000,000 | Commercial Launch |

---

## 📅 Phase 1: Secure MVP Development (Now - 7 Weeks)

**Goal**: Build a working secure prototype with 3 devices, mobile app with 2FA, and secure cloud backend.

**Team Structure**: 3-4 developers (2 software, 1-2 hardware) working in parallel streams.

### Week-by-Week Implementation Plan

#### **Weeks 1-2: Hardware & Firmware Foundation** (Parallel Development)

**Team Split**:
- **Hardware Team (1-2 people)**: Use the existing detailed `docs/WEEK1_HARDWARE_MASTERY.md` guide
  - [ ] Secure breadboard setup (3 plugs)
  - [ ] ESP32-S3 + ATECC608A provisioning
    - Generate unique device IDs
    - Provision ATECC608A with device private keys (ECDSA P256)
    - Lock ATECC608A configuration (irreversible)
  - [ ] Enable secure boot + flash encryption on ESP32-S3
  - [ ] Relay control via GPIO with optoisolation (PC817)
  - [ ] SCT-013 calibration with 33Ω burden resistors
  - [ ] Tamper detection circuit testing (MAX6316 watchdog)
  - [ ] Encrypted configuration storage (24C256 EEPROM)

- **Software Team (2 people)**: Leverage existing pre-hardware work
  - [ ] Finalize Flutter mobile app with Firebase Auth + 2FA (already in progress in `/app/`)
  - [ ] Complete React web dashboard with mock data (already in progress in `/web/`)
  - [ ] Set up Firebase backend infrastructure
  - [ ] Implement device pairing flow UI

**Deliverable**: 3 working breadboard prototypes + Mobile app & web dashboard ready for real device integration

---

#### **Week 3: Backend Integration & Real Device Connection**

**Full Team Collaboration**:
- [ ] Connect mobile app to actual ESP32 devices via MQTT/TLS
- [ ] Implement signed command generation (ECDSA)
- [ ] Firebase Firestore with field-level encryption (AES-256-GCM)
- [ ] Real-time power display from actual sensors
- [ ] WebSocket over TLS for dashboard
- [ ] Device registration and attestation endpoints
- [ ] Cloud Functions with input validation and rate limiting

**Deliverable**: End-to-end working system (hardware → backend → mobile/web)

---

#### **Week 4: Safety, Enclosure & PCB Design**

**Full Team Collaboration**: Hardware team designs physical security while software team implements remote security features.

##### **Hardware Team Focus**

###### **PCB Design with Security Zones**

**Objective**: Create a professional PCB layout with physically isolated AC and DC circuits to prevent electrical interference and enhance safety.

**Security Zones Implementation**:
- **AC Zone (Red Zone)**: High-voltage mains input, relay contacts, fuse, MOV protection
  - 2mm minimum trace spacing for 230V AC
  - Pour ground plane underneath for EMI shielding
  - Keep-out zone: 5mm clearance from DC circuits
- **DC Zone (Green Zone)**: ESP32-S3, ATECC608A, sensors, low-voltage components
  - Standard 0.3mm trace spacing for 3.3V logic
  - Separate ground plane connected via single point
- **Isolation Barrier**: PC817 optocoupler for relay control (2.5kV isolation rating)

**PCB Design Steps**:
1. **Schematic Finalization** (Day 1)
   - Review breadboard circuit and finalize component values
   - Add test points for debugging (TP1-TP10)
   - Include JTAG header for secure bootloader programming
2. **PCB Layout** (Days 2-3)
   - Use KiCad or Altium Designer
   - 2-layer PCB (cost-effective for prototype)
   - Place components with security in mind:
     - ATECC608A and EEPROM near ESP32-S3 (short trace = less attack surface)
     - Temperature sensor (DS18B20) near relay for thermal monitoring
     - MAX6316 watchdog with tamper detection pins accessible
3. **Design for Manufacturing (DFM)** (Day 3)
   - Minimum hole size: 0.3mm
   - Minimum trace width: 0.25mm (DC), 0.8mm (AC)
   - Solder mask: Green with white silkscreen
   - Surface finish: ENIG (gold plating for corrosion resistance)
4. **Gerber Generation & Review** (Day 4)
   - Generate Gerber files and drill files
   - Run DRC (Design Rule Check)
   - Send to JLCPCB or PCBWay for quotation (estimated R500 for 5 boards)

**Expected Challenges**:
- **Challenge**: AC/DC isolation requirements
  - **Solution**: Use 5mm clearance and review IEC 60950-1 standards
- **Challenge**: EMI interference from relay switching
  - **Solution**: Add RC snubber (100Ω + 100nF) across relay coil

###### **3D-Printed Enclosure v1 (Tamper-Resistant)**

**Objective**: Design a secure enclosure that protects the PCB, prevents tampering, and integrates with MAX6316 watchdog for physical security monitoring.

**Enclosure Specifications**:
- **Material**: PLA or PETG (heat-resistant up to 80°C)
- **Dimensions**: 100mm x 60mm x 40mm (compact form factor)
- **Wall Thickness**: 3mm (sufficient strength without excessive material)
- **Features**:
  - Ventilation slots for heat dissipation (but no direct PCB access)
  - Cable gland for mains input/output (IP40-rated)
  - LED light pipes for status indicators (power, WiFi, relay state)
  - Mounting bosses for M3 anti-tamper screws (Torx T10 security screws)

**Tamper Detection Integration**:
- **Magnetic Reed Switch**: Embedded in enclosure to detect case opening
  - Connected to MAX6316 watchdog input
  - When case opens, watchdog triggers:
    - Device logs tamper event to cloud (signed with ATECC608A)
    - Relay switches off (safety first)
    - Device enters secure mode (requires re-attestation)
- **Void-If-Removed Seals**: Apply holographic security stickers over screw holes
  - Custom printed with "Smart Plug AI - Void if Removed" text
  - Supplier: Security Label Warehouse (R2 per sticker)

**3D Printing Process** (Day 1-2):
1. Design in Fusion 360 or FreeCAD
2. Export as STL files (top and bottom halves)
3. Slice with Cura (0.2mm layer height, 20% infill)
4. Print on Ender 3 V2 or equivalent (8 hours per enclosure)
5. Post-processing: Sand edges, drill out screw holes, test fit PCB

**Expected Challenges**:
- **Challenge**: Warping during 3D printing
  - **Solution**: Use heated bed (60°C) and brim adhesion
- **Challenge**: Tight tolerances for cable gland
  - **Solution**: Design with 0.5mm clearance, adjust after test fit

###### **Mains Wiring with Fuse + MOV Protection**

**CRITICAL SAFETY WARNING**: Only qualified electricians should perform mains wiring. Follow local electrical codes (SANS 10142-1 in South Africa).

**Wiring Procedure**:
1. **Fuse Selection**: 3A quick-blow fuse (ceramic body)
   - Rationale: Smart plug rated for 500W max (230V × 3A = 690W safety margin)
   - Fuse holder: PCB-mount 5x20mm fuse holder
   - Location: First component after mains input (fail-safe)
2. **MOV (Metal Oxide Varistor) Protection**: 275V varistor across Live-Neutral
   - Rationale: Protects against voltage spikes (lightning, inductive loads)
   - Model: Littelfuse V275LA20AP (clamping voltage 430V)
   - Location: Parallel to mains input, before fuse
3. **Wire Gauge**: 18 AWG (1.0mm²) for internal connections
   - Rated for 10A continuous, well within 3A usage
   - Insulation: 300V-rated PVC or silicone
4. **Terminal Blocks**: Screw terminals for user connections
   - Input: 2-pole terminal block (Live, Neutral)
   - Output: 2-pole terminal block (Live, Neutral)
   - Ground: Separate PE (Protective Earth) terminal with yellow/green wire
5. **Wiring Color Code** (South African standard):
   - Live: Brown
   - Neutral: Blue
   - Ground: Yellow/Green
6. **Assembly Checklist**:
   - [ ] All connections tight (torque screwdriver: 0.5Nm)
   - [ ] No exposed conductor (heat shrink tubing on splices)
   - [ ] Continuity test: Verify Live-Neutral open when relay off
   - [ ] Insulation test: 500V megger test between AC and DC zones (>10MΩ)
   - [ ] Ground continuity: <0.1Ω from input ground to output ground

**Expected Challenges**:
- **Challenge**: Limited space in enclosure
  - **Solution**: Pre-cut and pre-strip wires to exact lengths
- **Challenge**: Ensuring proper fuse rating
  - **Solution**: Test with 500W load (hairdryer) to verify fuse holds

###### **Temperature Monitoring Safety Cut-Off**

**Objective**: Prevent fire hazards by monitoring relay temperature and cutting power if overheating detected.

**Implementation**:
1. **Temperature Sensor**: DS18B20 digital temperature sensor
   - Placement: Affixed to relay body with thermal adhesive
   - Accuracy: ±0.5°C from -10°C to +85°C
   - Connection: 1-Wire protocol to GPIO10 on ESP32-S3
2. **Software Safety Logic** (in firmware):
   ```c
   #define TEMP_WARNING_THRESHOLD 60.0   // °C - log warning
   #define TEMP_CRITICAL_THRESHOLD 70.0  // °C - cut power
   #define TEMP_CHECK_INTERVAL 5000      // ms - every 5 seconds
   
   void temperature_safety_task(void *pvParameters) {
       while(1) {
           float temp = read_ds18b20_temperature();
           
           if (temp > TEMP_CRITICAL_THRESHOLD) {
               // CRITICAL: Immediate power cut-off
               relay_turn_off();
               log_critical_event("OVERHEAT", temp);
               send_alert_to_cloud("Device overheating - power cut");
               enter_safe_mode();  // Requires manual reset
           } else if (temp > TEMP_WARNING_THRESHOLD) {
               log_warning_event("HIGH_TEMP", temp);
               send_alert_to_cloud("Temperature warning");
           }
           
           vTaskDelay(TEMP_CHECK_INTERVAL / portTICK_PERIOD_MS);
       }
   }
   ```
3. **Cloud Alert Integration**:
   - Temperature warnings sent via MQTT with high priority
   - User receives push notification on mobile app
   - Dashboard shows temperature trend graph

**Testing Procedure**:
- Heat relay with soldering iron to simulate overheating
- Verify relay trips at 70°C threshold
- Verify recovery after cooling (manual reset required)

**Expected Challenges**:
- **Challenge**: Sensor placement for accurate reading
  - **Solution**: Test multiple positions, select hottest spot
- **Challenge**: False positives in hot environments
  - **Solution**: Calibrate thresholds based on ambient temperature baseline

###### **Physical Security Measures**

**Anti-Tamper Screws**:
- **Type**: Torx T10 security screws (5-point star, harder to remove)
- **Quantity**: 4 screws per enclosure
- **Supplier**: AliExpress (R50 for 100-pack)
- **Alternative**: One-way screws (can tighten but not loosen without drilling)

**Void-If-Removed Seals**:
- **Purpose**: Visual indication of tampering
- **Application**: Over each screw and along enclosure seam
- **Custom Printing**: Include serial number and QR code for verification

**Additional Physical Security**:
- **Epoxy Coating**: Optional epoxy potting over ATECC608A and EEPROM
  - Makes chip-level attacks extremely difficult
  - Only apply after thorough testing (irreversible)
- **PCB Serial Numbers**: Laser-etched on PCB for tracking
- **Anti-Cloning Protection**: ATECC608A stores unique device key (cannot be read out)

##### **Software Team Focus**

###### **OTA Firmware Update System with Code Signing (RSA-3072)**

**Objective**: Enable secure remote firmware updates while preventing malicious firmware injection.

**Architecture**:
1. **Firmware Signing Process** (Developer Workflow):
   ```bash
   # Step 1: Build firmware binary
   cd /firmware
   idf.py build
   
   # Step 2: Sign firmware with private key (kept offline)
   openssl dgst -sha256 -sign private_key_rsa3072.pem \
       -out build/smartplug.bin.sig \
       build/smartplug.bin
   
   # Step 3: Upload to secure storage (Firebase Storage with access controls)
   gsutil cp build/smartplug.bin gs://smartplugai-firmware/v1.2.3/
   gsutil cp build/smartplug.bin.sig gs://smartplugai-firmware/v1.2.3/
   ```

2. **Device-Side OTA Implementation** (ESP32-S3):
   ```c
   #include "esp_ota_ops.h"
   #include "esp_https_ota.h"
   #include "mbedtls/rsa.h"
   #include "mbedtls/sha256.h"
   
   // Embedded public key (RSA-3072)
   extern const uint8_t public_key_pem_start[] asm("_binary_public_key_pem_start");
   extern const uint8_t public_key_pem_end[] asm("_binary_public_key_pem_end");
   
   bool verify_firmware_signature(const uint8_t *firmware, size_t firmware_len,
                                   const uint8_t *signature, size_t sig_len) {
       // Compute SHA-256 hash of firmware
       uint8_t hash[32];
       mbedtls_sha256(firmware, firmware_len, hash, 0);
       
       // Verify signature with RSA public key
       mbedtls_rsa_context rsa;
       mbedtls_rsa_init(&rsa, MBEDTLS_RSA_PKCS_V21, MBEDTLS_MD_SHA256);
       
       // Parse public key PEM
       mbedtls_pk_context pk;
       mbedtls_pk_init(&pk);
       int ret = mbedtls_pk_parse_public_key(&pk, public_key_pem_start,
                                              public_key_pem_end - public_key_pem_start);
       if (ret != 0) {
           ESP_LOGE(TAG, "Failed to parse public key");
           return false;
       }
       
       // Verify signature
       ret = mbedtls_pk_verify(&pk, MBEDTLS_MD_SHA256, hash, 32,
                                signature, sig_len);
       mbedtls_pk_free(&pk);
       
       return (ret == 0);
   }
   
   void ota_update_task(void *pvParameters) {
       // Step 1: Check for updates (query backend API)
       char *latest_version = check_for_firmware_update();
       
       if (strcmp(latest_version, CURRENT_VERSION) > 0) {
           ESP_LOGI(TAG, "New firmware available: %s", latest_version);
           
           // Step 2: Download firmware and signature
           esp_http_client_config_t config = {
               .url = "https://storage.googleapis.com/smartplugai-firmware/v1.2.3/smartplug.bin",
               .cert_pem = (char *)server_cert_pem_start,
               .timeout_ms = 30000,
           };
           
           esp_err_t ret = esp_https_ota(&config);
           if (ret == ESP_OK) {
               // Step 3: Verify signature before reboot
               const esp_partition_t *update_partition = esp_ota_get_next_update_partition(NULL);
               uint8_t *firmware_data = malloc(update_partition->size);
               esp_partition_read(update_partition, 0, firmware_data, update_partition->size);
               
               // Download signature
               uint8_t signature[384];  // RSA-3072 = 384 bytes
               download_file("https://storage.googleapis.com/.../smartplug.bin.sig", signature, 384);
               
               if (verify_firmware_signature(firmware_data, update_partition->size, signature, 384)) {
                   ESP_LOGI(TAG, "Signature valid, rebooting...");
                   esp_restart();
               } else {
                   ESP_LOGE(TAG, "Signature verification failed, aborting OTA");
                   esp_ota_abort();
               }
               
               free(firmware_data);
           }
       }
   }
   ```

3. **Backend Firmware Management**:
   - **Firmware Versioning**: Semantic versioning (v1.2.3)
   - **Rollout Strategy**: Canary deployment (10% of devices first)
   - **Rollback Mechanism**: If >5% devices fail attestation post-update, auto-rollback

**Expected Challenges**:
- **Challenge**: Large firmware binaries (>1MB) over WiFi
  - **Solution**: Delta updates (only changed blocks), compression
- **Challenge**: Device power loss during OTA
  - **Solution**: Use ESP32 OTA partition scheme (A/B partitions)

###### **Device Attestation Endpoints**

**Objective**: Verify device authenticity and firmware integrity before allowing cloud connections.

**Attestation Flow**:
1. **Device Boot** → Generates attestation challenge
2. **Backend** → Sends challenge to device
3. **Device** → Signs challenge with ATECC608A private key
4. **Backend** → Verifies signature with device's public key (stored during provisioning)
5. **Success** → Device granted access token (JWT with 24-hour expiry)

**Backend API Implementation** (Firebase Cloud Functions):

**File**: `/backend/functions/src/deviceAttestation.ts`

```typescript
import * as functions from 'firebase-functions';
import * as admin from 'firebase-admin';
import * as crypto from 'crypto';
import * as elliptic from 'elliptic';

const ec = new elliptic.ec('p256');

export const attestDevice = functions.https.onRequest(async (req, res) => {
  if (req.method !== 'POST') {
    return res.status(405).json({ error: 'Method not allowed' });
  }

  const { deviceId, signature, firmwareVersion, chipId } = req.body;

  try {
    // Step 1: Retrieve device public key from Firestore
    const deviceDoc = await admin.firestore()
      .collection('devices')
      .doc(deviceId)
      .get();

    if (!deviceDoc.exists) {
      return res.status(404).json({ error: 'Device not registered' });
    }

    const deviceData = deviceDoc.data();
    const publicKey = deviceData.publicKey;  // Hex-encoded ECDSA P-256 public key

    // Step 2: Generate challenge (timestamp + random nonce)
    const challenge = `${Date.now()}-${crypto.randomBytes(16).toString('hex')}`;

    // Step 3: Verify signature (device should have signed challenge with ATECC608A)
    const key = ec.keyFromPublic(publicKey, 'hex');
    const isValid = key.verify(
      crypto.createHash('sha256').update(challenge).digest('hex'),
      signature
    );

    if (!isValid) {
      // Log failed attestation attempt
      await admin.firestore().collection('security_events').add({
        type: 'ATTESTATION_FAILED',
        deviceId,
        timestamp: admin.firestore.FieldValue.serverTimestamp(),
        ipAddress: req.ip,
      });

      return res.status(401).json({ error: 'Invalid signature' });
    }

    // Step 4: Verify firmware version (check if it's a known good version)
    const approvedVersions = ['v1.0.0', 'v1.1.0', 'v1.2.3'];
    if (!approvedVersions.includes(firmwareVersion)) {
      return res.status(403).json({ error: 'Unauthorized firmware version' });
    }

    // Step 5: Generate JWT access token
    const token = await admin.auth().createCustomToken(deviceId, {
      deviceId,
      firmwareVersion,
      chipId,
      attestedAt: Date.now(),
    });

    // Step 6: Update device status in Firestore
    await deviceDoc.ref.update({
      lastAttestation: admin.firestore.FieldValue.serverTimestamp(),
      firmwareVersion,
      status: 'ONLINE',
    });

    // Step 7: Log successful attestation
    await admin.firestore().collection('security_events').add({
      type: 'ATTESTATION_SUCCESS',
      deviceId,
      firmwareVersion,
      timestamp: admin.firestore.FieldValue.serverTimestamp(),
    });

    return res.status(200).json({
      success: true,
      token,
      expiresIn: 86400,  // 24 hours
      challenge,  // For next attestation
    });

  } catch (error) {
    console.error('Attestation error:', error);
    return res.status(500).json({ error: 'Internal server error' });
  }
});
```

**Expected Challenges**:
- **Challenge**: Attestation adds latency to device boot
  - **Solution**: Cache tokens locally (with secure storage), only re-attest on expiry
- **Challenge**: Revocation of compromised devices
  - **Solution**: Maintain revocation list in Firestore, check on every attestation

###### **Enhanced Error Handling and Logging**

**Objective**: Implement comprehensive error handling and structured logging for debugging and security monitoring.

**Error Categories**:
1. **Hardware Errors**: Sensor failures, relay malfunctions
2. **Network Errors**: WiFi disconnects, MQTT timeouts
3. **Security Errors**: Attestation failures, invalid signatures
4. **User Errors**: Invalid commands, rate limit exceeded

**Logging Implementation**:

**Firmware Side** (ESP32-S3):
```c
#include "esp_log.h"
#include "nvs_flash.h"

// Log levels: ERROR, WARN, INFO, DEBUG, VERBOSE
static const char *TAG = "SMARTPLUG";

typedef enum {
    LOG_HARDWARE = 0,
    LOG_NETWORK,
    LOG_SECURITY,
    LOG_USER,
    LOG_SYSTEM
} log_category_t;

void log_structured_event(log_category_t category, const char *event,
                          const char *details, esp_log_level_t level) {
    // Format: [TIMESTAMP] [CATEGORY] [LEVEL] Event: Details
    char log_msg[256];
    snprintf(log_msg, sizeof(log_msg), "[%s] [%s] %s: %s",
             get_log_category_name(category),
             get_log_level_name(level),
             event, details);
    
    // Log locally
    ESP_LOG_LEVEL_LOCAL(level, TAG, "%s", log_msg);
    
    // Store in NVS for persistence (circular buffer, last 100 events)
    store_log_in_nvs(log_msg);
    
    // Send to cloud (batch upload every 5 minutes)
    queue_log_for_cloud_upload(category, event, details, level);
}

// Example usage:
log_structured_event(LOG_SECURITY, "ATTESTATION_FAILED",
                     "Invalid signature from backend", ESP_LOG_ERROR);
```

**Backend Side** (Cloud Functions):
```typescript
// Centralized logging with Firebase Functions Logger
import * as functions from 'firebase-functions';

export const logDeviceEvent = functions.https.onRequest(async (req, res) => {
  const { deviceId, category, event, details, level, timestamp } = req.body;
  
  // Store in Firestore for long-term analysis
  await admin.firestore().collection('device_logs').add({
    deviceId,
    category,
    event,
    details,
    level,
    timestamp: new Date(timestamp),
    serverTimestamp: admin.firestore.FieldValue.serverTimestamp(),
  });
  
  // Trigger alerts for critical errors
  if (level === 'ERROR' && category === 'SECURITY') {
    await sendAlertToSecurityTeam(deviceId, event, details);
  }
  
  res.status(200).json({ success: true });
});
```

**Expected Challenges**:
- **Challenge**: Log storage costs on Firebase
  - **Solution**: Implement log rotation (delete logs >30 days), only store critical events
- **Challenge**: Debugging production devices without direct access
  - **Solution**: Remote log retrieval via admin API (requires authentication)

##### **Testing Criteria**

**Hardware Testing**:
- [ ] PCB design passes DRC (Design Rule Check)
- [ ] Insulation resistance >10MΩ between AC and DC zones
- [ ] Enclosure withstands drop test from 1 meter
- [ ] Tamper detection triggers within 1 second of case opening
- [ ] Temperature cut-off triggers at 70°C ±2°C
- [ ] 500W load test for 1 hour (relay temperature <60°C)

**Software Testing**:
- [ ] OTA update successful on all 3 test devices
- [ ] Signature verification rejects tampered firmware
- [ ] Device attestation success rate >99%
- [ ] Failed attestation blocks device access
- [ ] Logs successfully uploaded to cloud
- [ ] Critical errors trigger immediate alerts

##### **Expected Challenges and Solutions**

| Challenge | Solution |
|-----------|----------|
| PCB manufacturing delays (2-3 weeks) | Order 2x quantity, use expedited shipping |
| 3D printer failures mid-print | Print enclosures overnight, inspect in morning |
| RSA-3072 signature verification slow on ESP32 | Use hardware crypto acceleration (ESP32-S3 AES engine) |
| OTA updates fail on slow WiFi | Implement resume-on-failure, retry with exponential backoff |
| Tamper detection false positives | Add 5-second grace period after power-on |

##### **Deliverable Breakdown**

**Hardware Deliverables**:
- 3 enclosed secure prototypes with tamper detection
- PCB Gerber files ready for manufacturing quote
- 3D-printable enclosure STL files (top and bottom)
- Mains wiring documentation with photos

**Software Deliverables**:
- OTA firmware update system (code signing + verification)
- Device attestation API endpoint (deployed to Firebase)
- Enhanced logging framework (device + cloud)
- Security test report (attestation success rates)

**Documentation Deliverables**:
- PCB design documentation with security zones diagram
- Enclosure design files and assembly instructions
- OTA update deployment guide
- Attestation flow diagram

**Success Metric**: All 3 devices successfully perform OTA update, pass attestation, and trigger tamper alerts when case opened.

---

#### **Week 5: Automation, Rule Engine & Advanced Features**

**Full Team Collaboration**: Backend team builds secure automation engine while frontend team creates intuitive automation interfaces.

##### **Backend Team Focus**

###### **Time-Based Schedules with Signed Commands**

**Objective**: Enable users to create schedules (e.g., "Turn on at 7AM daily") with cryptographic verification to prevent unauthorized schedule modifications.

**Architecture**:
1. **Schedule Data Model** (Firestore):
   ```json
   {
     "scheduleId": "sched_abc123",
     "deviceId": "device-001",
     "userId": "user_xyz789",
     "action": "turn_on",
     "cronExpression": "0 7 * * *",
     "timezone": "Africa/Johannesburg",
     "enabled": true,
     "signature": "a1b2c3d4...",
     "createdAt": "2024-12-01T10:00:00Z",
     "updatedAt": "2024-12-01T10:00:00Z"
   }
   ```

2. **Schedule Signing Process**:
   ```typescript
   // Backend: Sign schedule with server private key
   import * as crypto from 'crypto';
   
   function signSchedule(schedule: any, privateKey: string): string {
     const scheduleData = JSON.stringify({
       scheduleId: schedule.scheduleId,
       deviceId: schedule.deviceId,
       action: schedule.action,
       cronExpression: schedule.cronExpression,
       timestamp: Date.now()
     });
     
     const sign = crypto.createSign('RSA-SHA256');
     sign.update(scheduleData);
     const signature = sign.sign(privateKey, 'base64');
     
     return signature;
   }
   
   export const createSchedule = functions.https.onRequest(async (req, res) => {
     const { deviceId, action, cronExpression, timezone } = req.body;
     
     // Step 1: Validate user owns device
     const userId = req.user.uid;
     const device = await verifyDeviceOwnership(userId, deviceId);
     if (!device) {
       return res.status(403).json({ error: 'Unauthorized' });
     }
     
     // Step 2: Validate cron expression
     if (!isValidCron(cronExpression)) {
       return res.status(400).json({ error: 'Invalid cron expression' });
     }
     
     // Step 3: Create schedule
     const scheduleId = `sched_${crypto.randomBytes(8).toString('hex')}`;
     const schedule = {
       scheduleId,
       deviceId,
       userId,
       action,
       cronExpression,
       timezone,
       enabled: true,
       createdAt: admin.firestore.FieldValue.serverTimestamp(),
       updatedAt: admin.firestore.FieldValue.serverTimestamp(),
     };
     
     // Step 4: Sign schedule
     const signature = signSchedule(schedule, SERVER_PRIVATE_KEY);
     schedule.signature = signature;
     
     // Step 5: Store in Firestore
     await admin.firestore().collection('schedules').doc(scheduleId).set(schedule);
     
     return res.status(200).json({ success: true, scheduleId });
   });
   ```

3. **Schedule Execution** (Cloud Functions Cron Job):
   ```typescript
   export const executeSchedules = functions.pubsub
     .schedule('every 1 minutes')
     .onRun(async (context) => {
       const now = new Date();
       
       // Step 1: Query schedules due for execution
       const schedulesSnapshot = await admin.firestore()
         .collection('schedules')
         .where('enabled', '==', true)
         .get();
       
       for (const scheduleDoc of schedulesSnapshot.docs) {
         const schedule = scheduleDoc.data();
         
         // Step 2: Check if schedule should run now
         if (shouldExecuteSchedule(schedule.cronExpression, now, schedule.timezone)) {
           
           // Step 3: Verify schedule signature
           if (!verifyScheduleSignature(schedule, SERVER_PUBLIC_KEY)) {
             console.error(`Invalid signature for schedule ${schedule.scheduleId}`);
             await logSecurityEvent('SCHEDULE_TAMPERING', schedule.scheduleId);
             continue;
           }
           
           // Step 4: Generate signed command
           const command = {
             deviceId: schedule.deviceId,
             action: schedule.action,
             timestamp: Date.now(),
             nonce: crypto.randomBytes(16).toString('hex'),
             scheduledBy: schedule.scheduleId,
           };
           
           const commandSignature = signCommand(command, SERVER_PRIVATE_KEY);
           
           // Step 5: Send command to device via MQTT
           await publishMqttCommand(schedule.deviceId, command, commandSignature);
           
           // Step 6: Log execution
           await admin.firestore().collection('schedule_executions').add({
             scheduleId: schedule.scheduleId,
             deviceId: schedule.deviceId,
             action: schedule.action,
             executedAt: admin.firestore.FieldValue.serverTimestamp(),
             status: 'PENDING',
           });
         }
       }
     });
   ```

4. **Device-Side Verification** (ESP32-S3):
   ```c
   bool verify_scheduled_command(const char *command_json, const char *signature) {
       // Parse command
       cJSON *json = cJSON_Parse(command_json);
       const char *deviceId = cJSON_GetObjectItem(json, "deviceId")->valuestring;
       const char *action = cJSON_GetObjectItem(json, "action")->valuestring;
       uint64_t timestamp = cJSON_GetObjectItem(json, "timestamp")->valueint;
       const char *nonce = cJSON_GetObjectItem(json, "nonce")->valuestring;
       
       // Verify timestamp (reject if >5 minutes old)
       uint64_t current_time = get_current_timestamp();
       if (abs(current_time - timestamp) > 300) {
           ESP_LOGE(TAG, "Command timestamp too old");
           return false;
       }
       
       // Verify signature with server public key
       // (Use mbedTLS RSA verification)
       bool valid = rsa_verify_signature(command_json, signature, SERVER_PUBLIC_KEY);
       
       cJSON_Delete(json);
       return valid;
   }
   ```

**Expected Challenges**:
- **Challenge**: Timezone handling across devices in different regions
  - **Solution**: Store all times in UTC, convert to device timezone on execution
- **Challenge**: Schedule execution delays due to cloud function cold starts
  - **Solution**: Keep functions warm with ping every 5 minutes

###### **Load-Shedding Detection System**

**Objective**: Automatically detect load-shedding schedules (Eskom Stage 1-8) and adjust device behavior accordingly.

**Implementation Strategy**:
1. **Static Schedule Storage**:
   - Store official Eskom load-shedding schedules in Firestore
   - Update schedules via secure admin API (signed with admin keys)
   - Schedules include: stage, area, start time, end time

2. **Real-Time Detection**:
   ```typescript
   export const detectLoadShedding = functions.https.onRequest(async (req, res) => {
     const { deviceId, userArea } = req.body;
     
     // Step 1: Get current load-shedding status from Firestore
     const loadsheddingDoc = await admin.firestore()
       .collection('loadshedding')
       .doc('current')
       .get();
     
     const currentStage = loadsheddingDoc.data().stage;  // e.g., "Stage 4"
     
     if (currentStage === 0) {
       return res.json({ loadshedding: false });
     }
     
     // Step 2: Check if user's area affected now
     const now = new Date();
     const scheduleSnapshot = await admin.firestore()
       .collection('loadshedding_schedules')
       .where('stage', '==', currentStage)
       .where('area', '==', userArea)
       .get();
     
     for (const scheduleDoc of scheduleSnapshot.docs) {
       const schedule = scheduleDoc.data();
       
       if (isWithinTimeRange(now, schedule.startTime, schedule.endTime)) {
         return res.json({
           loadshedding: true,
           stage: currentStage,
           startTime: schedule.startTime,
           endTime: schedule.endTime,
           estimatedDuration: schedule.durationMinutes,
         });
       }
     }
     
     return res.json({ loadshedding: false });
   });
   ```

3. **Schedule Update Mechanism** (Admin Only):
   ```typescript
   export const updateLoadSheddingSchedule = functions.https.onCall(async (data, context) => {
     // Step 1: Verify admin privileges
     if (!context.auth.token.admin) {
       throw new functions.https.HttpsError('permission-denied', 'Admin only');
     }
     
     const { schedules, signature } = data;
     
     // Step 2: Verify signature (schedules signed with admin private key)
     if (!verifyAdminSignature(schedules, signature, ADMIN_PUBLIC_KEY)) {
       throw new functions.https.HttpsError('invalid-argument', 'Invalid signature');
     }
     
     // Step 3: Update schedules in Firestore
     const batch = admin.firestore().batch();
     for (const schedule of schedules) {
       const docRef = admin.firestore()
         .collection('loadshedding_schedules')
         .doc(schedule.id);
       batch.set(docRef, schedule);
     }
     await batch.commit();
     
     // Step 4: Log update
     await admin.firestore().collection('admin_actions').add({
       action: 'UPDATE_LOADSHEDDING',
       adminId: context.auth.uid,
       timestamp: admin.firestore.FieldValue.serverTimestamp(),
       scheduleCount: schedules.length,
     });
     
     return { success: true };
   });
   ```

4. **User Notifications**:
   - Push notification 30 minutes before load-shedding starts
   - Option to pre-schedule device actions (e.g., "Turn off geyser before load-shedding")

**Expected Challenges**:
- **Challenge**: Eskom schedule changes at short notice
  - **Solution**: Admin can push updates instantly, devices poll every 15 minutes
- **Challenge**: Users in areas not covered by official schedules
  - **Solution**: Allow manual schedule entry (with warnings)

###### **Rate Limiting on Control Commands (10 commands/min per device)**

**Objective**: Prevent abuse and potential DDoS attacks by limiting command frequency.

**Implementation** (Firebase Cloud Functions):
```typescript
import * as admin from 'firebase-admin';
import * as functions from 'firebase-functions';

const MAX_COMMANDS_PER_MINUTE = 10;
const RATE_LIMIT_WINDOW = 60 * 1000; // 60 seconds in milliseconds

export const sendDeviceCommand = functions.https.onRequest(async (req, res) => {
  const { deviceId, action } = req.body;
  const userId = req.user.uid;
  
  // Step 1: Check rate limit
  const rateLimitKey = `ratelimit:${userId}:${deviceId}`;
  const rateLimitRef = admin.database().ref(rateLimitKey);
  
  const snapshot = await rateLimitRef.once('value');
  const commandHistory = snapshot.val() || [];
  
  // Step 2: Remove commands older than 1 minute
  const now = Date.now();
  const recentCommands = commandHistory.filter(
    (timestamp: number) => now - timestamp < RATE_LIMIT_WINDOW
  );
  
  // Step 3: Check if limit exceeded
  if (recentCommands.length >= MAX_COMMANDS_PER_MINUTE) {
    // Log rate limit violation
    await admin.firestore().collection('security_events').add({
      type: 'RATE_LIMIT_EXCEEDED',
      userId,
      deviceId,
      timestamp: admin.firestore.FieldValue.serverTimestamp(),
      commandCount: recentCommands.length,
    });
    
    return res.status(429).json({
      error: 'Rate limit exceeded',
      retryAfter: 60,
      limit: MAX_COMMANDS_PER_MINUTE,
    });
  }
  
  // Step 4: Record this command
  recentCommands.push(now);
  await rateLimitRef.set(recentCommands);
  
  // Step 5: Process command
  const command = {
    deviceId,
    action,
    userId,
    timestamp: now,
    nonce: crypto.randomBytes(16).toString('hex'),
  };
  
  const signature = signCommand(command, SERVER_PRIVATE_KEY);
  await publishMqttCommand(deviceId, command, signature);
  
  return res.status(200).json({
    success: true,
    commandId: command.nonce,
    remaining: MAX_COMMANDS_PER_MINUTE - recentCommands.length,
  });
});
```

**Client-Side Handling** (Mobile App):
```dart
// Flutter mobile app
class DeviceCommandService {
  static const int maxRetries = 3;
  
  Future<bool> sendCommand(String deviceId, String action) async {
    for (int attempt = 0; attempt < maxRetries; attempt++) {
      try {
        final response = await http.post(
          Uri.parse('https://api.smartplugai.com/commands'),
          body: jsonEncode({'deviceId': deviceId, 'action': action}),
          headers: {'Authorization': 'Bearer $token'},
        );
        
        if (response.statusCode == 200) {
          return true;
        } else if (response.statusCode == 429) {
          // Rate limit exceeded
          final data = jsonDecode(response.body);
          final retryAfter = data['retryAfter'] as int;
          
          // Show user-friendly message
          showSnackbar('Too many commands. Please wait ${retryAfter}s.');
          
          // Wait and retry
          await Future.delayed(Duration(seconds: retryAfter));
        } else {
          throw Exception('Command failed: ${response.statusCode}');
        }
      } catch (e) {
        print('Command error: $e');
      }
    }
    
    return false;
  }
}
```

**Expected Challenges**:
- **Challenge**: Legitimate burst usage (e.g., user testing device)
  - **Solution**: Allow temporary rate limit increase via UI (requires 2FA confirmation)
- **Challenge**: Rate limit storage in Realtime Database
  - **Solution**: Use Redis Cache if scaling beyond 1000 devices

###### **Command Replay Attack Prevention (Nonce + Timestamp Verification)**

**Objective**: Prevent attackers from replaying captured commands to control devices.

**Implementation**:
1. **Command Structure**:
   ```json
   {
     "deviceId": "device-001",
     "action": "turn_on",
     "timestamp": 1638360000000,
     "nonce": "a1b2c3d4e5f6...",
     "userId": "user_xyz789",
     "signature": "..."
   }
   ```

2. **Backend Command Generation**:
   ```typescript
   function generateSecureCommand(deviceId: string, action: string, userId: string) {
     const command = {
       deviceId,
       action,
       userId,
       timestamp: Date.now(),
       nonce: crypto.randomBytes(16).toString('hex'),  // 128-bit random nonce
     };
     
     const signature = signCommand(command, SERVER_PRIVATE_KEY);
     command.signature = signature;
     
     // Store nonce in Redis (expires in 5 minutes)
     await redis.setex(`nonce:${command.nonce}`, 300, '1');
     
     return command;
   }
   ```

3. **Device-Side Verification** (ESP32-S3):
   ```c
   #include "mbedtls/sha256.h"
   #include "nvs_flash.h"
   
   #define MAX_NONCE_CACHE 100
   #define COMMAND_VALIDITY_WINDOW 300  // 5 minutes in seconds
   
   // Circular buffer for nonce cache (prevent replay)
   static char nonce_cache[MAX_NONCE_CACHE][33];  // 32 hex chars + null terminator
   static int nonce_index = 0;
   
   bool is_nonce_used(const char *nonce) {
       for (int i = 0; i < MAX_NONCE_CACHE; i++) {
           if (strcmp(nonce_cache[i], nonce) == 0) {
               return true;  // Nonce already used
           }
       }
       return false;
   }
   
   void store_nonce(const char *nonce) {
       strncpy(nonce_cache[nonce_index], nonce, 32);
       nonce_cache[nonce_index][32] = '\0';
       nonce_index = (nonce_index + 1) % MAX_NONCE_CACHE;
   }
   
   bool verify_command_freshness(uint64_t command_timestamp) {
       uint64_t current_time = get_ntp_timestamp();  // Synced with NTP
       uint64_t time_diff = abs(current_time - command_timestamp);
       
       if (time_diff > COMMAND_VALIDITY_WINDOW) {
           ESP_LOGW(TAG, "Command timestamp too old: %llu seconds", time_diff);
           return false;
       }
       
       return true;
   }
   
   bool process_device_command(const char *command_json, const char *signature) {
       // Step 1: Parse command
       cJSON *json = cJSON_Parse(command_json);
       const char *deviceId = cJSON_GetObjectItem(json, "deviceId")->valuestring;
       const char *action = cJSON_GetObjectItem(json, "action")->valuestring;
       uint64_t timestamp = cJSON_GetObjectItem(json, "timestamp")->valueint;
       const char *nonce = cJSON_GetObjectItem(json, "nonce")->valuestring;
       
       // Step 2: Verify device ID matches
       if (strcmp(deviceId, DEVICE_ID) != 0) {
           ESP_LOGE(TAG, "Device ID mismatch");
           cJSON_Delete(json);
           return false;
       }
       
       // Step 3: Verify timestamp (freshness check)
       if (!verify_command_freshness(timestamp)) {
           log_security_event("STALE_COMMAND", timestamp);
           cJSON_Delete(json);
           return false;
       }
       
       // Step 4: Check for replay attack (nonce reuse)
       if (is_nonce_used(nonce)) {
           log_security_event("REPLAY_ATTACK", nonce);
           cJSON_Delete(json);
           return false;
       }
       
       // Step 5: Verify signature
       bool signature_valid = verify_rsa_signature(command_json, signature);
       if (!signature_valid) {
           log_security_event("INVALID_SIGNATURE", "");
           cJSON_Delete(json);
           return false;
       }
       
       // Step 6: Store nonce to prevent replay
       store_nonce(nonce);
       
       // Step 7: Execute command
       if (strcmp(action, "turn_on") == 0) {
           relay_turn_on();
       } else if (strcmp(action, "turn_off") == 0) {
           relay_turn_off();
       }
       
       cJSON_Delete(json);
       return true;
   }
   ```

**Expected Challenges**:
- **Challenge**: Device clock drift causing timestamp mismatches
  - **Solution**: Sync with NTP server every hour, allow ±5 minute tolerance
- **Challenge**: Limited memory for nonce cache on ESP32
  - **Solution**: Use circular buffer (100 nonces = ~3.2KB), persist to NVS

##### **Frontend Team Focus**

###### **Automation UI Design in Mobile App and Web Dashboard**

**Objective**: Create intuitive interfaces for users to create, edit, and manage automation rules.

**Mobile App UI (Flutter)**:

**File**: `/app/lib/screens/automation_screen.dart`

```dart
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

class AutomationScreen extends StatefulWidget {
  @override
  _AutomationScreenState createState() => _AutomationScreenState();
}

class _AutomationScreenState extends State<AutomationScreen> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Automation'),
        actions: [
          IconButton(
            icon: Icon(Icons.add),
            onPressed: () => _showCreateAutomationDialog(),
          ),
        ],
      ),
      body: StreamBuilder<List<Schedule>>(
        stream: _automationService.getSchedules(),
        builder: (context, snapshot) {
          if (snapshot.connectionState == ConnectionState.waiting) {
            return Center(child: CircularProgressIndicator());
          }
          
          if (!snapshot.hasData || snapshot.data.isEmpty) {
            return _buildEmptyState();
          }
          
          return ListView.builder(
            itemCount: snapshot.data.length,
            itemBuilder: (context, index) {
              final schedule = snapshot.data[index];
              return _buildScheduleCard(schedule);
            },
          );
        },
      ),
    );
  }
  
  Widget _buildScheduleCard(Schedule schedule) {
    return Card(
      margin: EdgeInsets.symmetric(horizontal: 16, vertical: 8),
      child: ListTile(
        leading: Icon(
          schedule.enabled ? Icons.schedule : Icons.schedule_outlined,
          color: schedule.enabled ? Colors.green : Colors.grey,
        ),
        title: Text('${schedule.action} at ${schedule.displayTime}'),
        subtitle: Text('Device: ${schedule.deviceName}'),
        trailing: Switch(
          value: schedule.enabled,
          onChanged: (value) => _toggleSchedule(schedule, value),
        ),
        onTap: () => _editSchedule(schedule),
      ),
    );
  }
  
  void _showCreateAutomationDialog() {
    // Time picker, device selector, action selector
    showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text('Create Schedule'),
        content: Column(
          mainAxisSize: MainAxisSize.min,
          children: [
            DropdownButton<String>(
              items: devices.map((device) =>
                DropdownMenuItem(value: device.id, child: Text(device.name))
              ).toList(),
              onChanged: (value) => setState(() => selectedDevice = value),
            ),
            // Time picker, repeat options, etc.
          ],
        ),
        actions: [
          TextButton(
            onPressed: () => Navigator.pop(context),
            child: Text('Cancel'),
          ),
          ElevatedButton(
            onPressed: () => _createSchedule(),
            child: Text('Create'),
          ),
        ],
      ),
    );
  }
}
```

**Web Dashboard UI (React)**:

**File**: `/web/src/components/AutomationManager.tsx`

```typescript
import React, { useState, useEffect } from 'react';
import { Card, Button, Switch, Modal, TimePicker, Select } from 'antd';
import { PlusOutlined, ClockCircleOutlined } from '@ant-design/icons';

interface Schedule {
  scheduleId: string;
  deviceId: string;
  deviceName: string;
  action: string;
  cronExpression: string;
  displayTime: string;
  enabled: boolean;
}

export const AutomationManager: React.FC = () => {
  const [schedules, setSchedules] = useState<Schedule[]>([]);
  const [isModalVisible, setIsModalVisible] = useState(false);
  const [loading, setLoading] = useState(false);
  
  useEffect(() => {
    fetchSchedules();
  }, []);
  
  const fetchSchedules = async () => {
    setLoading(true);
    try {
      const response = await fetch('/api/schedules', {
        headers: { Authorization: `Bearer ${getAuthToken()}` },
      });
      const data = await response.json();
      setSchedules(data.schedules);
    } catch (error) {
      console.error('Error fetching schedules:', error);
    } finally {
      setLoading(false);
    }
  };
  
  const toggleSchedule = async (scheduleId: string, enabled: boolean) => {
    try {
      await fetch(`/api/schedules/${scheduleId}`, {
        method: 'PATCH',
        headers: {
          'Content-Type': 'application/json',
          Authorization: `Bearer ${getAuthToken()}`,
        },
        body: JSON.stringify({ enabled }),
      });
      fetchSchedules();
    } catch (error) {
      console.error('Error toggling schedule:', error);
    }
  };
  
  return (
    <div className="automation-manager">
      <div className="header">
        <h2>Automation Rules</h2>
        <Button
          type="primary"
          icon={<PlusOutlined />}
          onClick={() => setIsModalVisible(true)}
        >
          Create Schedule
        </Button>
      </div>
      
      <div className="schedules-grid">
        {schedules.map((schedule) => (
          <Card
            key={schedule.scheduleId}
            title={`${schedule.action} at ${schedule.displayTime}`}
            extra={
              <Switch
                checked={schedule.enabled}
                onChange={(checked) => toggleSchedule(schedule.scheduleId, checked)}
              />
            }
          >
            <p>Device: {schedule.deviceName}</p>
            <p>Schedule: {schedule.displayTime}</p>
            <ClockCircleOutlined style={{ marginRight: 8 }} />
          </Card>
        ))}
      </div>
      
      <Modal
        title="Create Automation"
        visible={isModalVisible}
        onCancel={() => setIsModalVisible(false)}
        onOk={handleCreateSchedule}
      >
        {/* Form fields for creating schedule */}
      </Modal>
    </div>
  );
};
```

**Expected Challenges**:
- **Challenge**: Complex cron expressions confusing for users
  - **Solution**: Use natural language (e.g., "Every weekday at 7AM") with visual picker
- **Challenge**: Multiple schedules conflicting
  - **Solution**: Show warnings when schedules overlap, allow priority setting

###### **Energy Usage Alerts System**

**Objective**: Notify users when energy consumption exceeds thresholds or anomalies detected.

**Implementation**:
1. **Alert Rules Configuration**:
   ```typescript
   interface AlertRule {
     alertId: string;
     userId: string;
     deviceId: string;
     condition: 'ABOVE' | 'BELOW' | 'ANOMALY';
     threshold: number;  // kWh or Watts
     enabled: boolean;
     notificationChannels: ('push' | 'email' | 'sms')[];
   }
   ```

2. **Alert Monitoring** (Cloud Functions):
   ```typescript
   export const monitorEnergyUsage = functions.firestore
     .document('telemetry/{deviceId}/{timestamp}')
     .onCreate(async (snapshot, context) => {
       const telemetry = snapshot.data();
       const deviceId = context.params.deviceId;
       
       // Get alert rules for this device
       const alertsSnapshot = await admin.firestore()
         .collection('alerts')
         .where('deviceId', '==', deviceId)
         .where('enabled', '==', true)
         .get();
       
       for (const alertDoc of alertsSnapshot.docs) {
         const alert = alertDoc.data() as AlertRule;
         
         if (alert.condition === 'ABOVE' && telemetry.power > alert.threshold) {
           await sendAlert(alert, telemetry);
         } else if (alert.condition === 'BELOW' && telemetry.power < alert.threshold) {
           await sendAlert(alert, telemetry);
         }
       }
     });
   
   async function sendAlert(alert: AlertRule, telemetry: any) {
     const notification = {
       title: 'Energy Usage Alert',
       body: `Device ${alert.deviceId} is using ${telemetry.power}W (threshold: ${alert.threshold}W)`,
       data: {
         deviceId: alert.deviceId,
         power: telemetry.power.toString(),
       },
     };
     
     if (alert.notificationChannels.includes('push')) {
       await sendPushNotification(alert.userId, notification);
     }
     
     if (alert.notificationChannels.includes('email')) {
       await sendEmailNotification(alert.userId, notification);
     }
     
     // Log alert
     await admin.firestore().collection('alert_history').add({
       alertId: alert.alertId,
       deviceId: alert.deviceId,
       userId: alert.userId,
       condition: alert.condition,
       threshold: alert.threshold,
       actualValue: telemetry.power,
       timestamp: admin.firestore.FieldValue.serverTimestamp(),
     });
   }
   ```

###### **Historical Data Visualization Components**

**Objective**: Display energy usage trends over time with interactive charts.

**Mobile App** (Using fl_chart):
```dart
import 'package:fl_chart/fl_chart.dart';

class EnergyUsageChart extends StatelessWidget {
  final List<EnergyDataPoint> dataPoints;
  
  @override
  Widget build(BuildContext context) {
    return LineChart(
      LineChartData(
        lineBarsData: [
          LineChartBarData(
            spots: dataPoints.map((point) =>
              FlSpot(point.timestamp.toDouble(), point.power)
            ).toList(),
            isCurved: true,
            colors: [Colors.blue],
            barWidth: 3,
            dotData: FlDotData(show: false),
          ),
        ],
        titlesData: FlTitlesData(
          bottomTitles: SideTitles(
            showTitles: true,
            getTitles: (value) => formatTimestamp(value),
          ),
          leftTitles: SideTitles(
            showTitles: true,
            getTitles: (value) => '${value.toInt()}W',
          ),
        ),
      ),
    );
  }
}
```

**Web Dashboard** (Using Chart.js):
```typescript
import { Line } from 'react-chartjs-2';

export const EnergyUsageChart: React.FC<{ deviceId: string }> = ({ deviceId }) => {
  const [chartData, setChartData] = useState(null);
  
  useEffect(() => {
    fetchEnergyData(deviceId).then(data => {
      setChartData({
        labels: data.map(point => new Date(point.timestamp).toLocaleTimeString()),
        datasets: [
          {
            label: 'Power Usage (W)',
            data: data.map(point => point.power),
            borderColor: 'rgb(75, 192, 192)',
            tension: 0.4,
          },
        ],
      });
    });
  }, [deviceId]);
  
  return chartData ? <Line data={chartData} options={chartOptions} /> : <Loading />;
};
```

###### **Cost Tracking Integration Details**

**Objective**: Calculate electricity costs based on usage and user's tariff.

**Implementation**:
```typescript
interface TariffStructure {
  userId: string;
  provider: 'ESKOM' | 'CITY_POWER' | 'CUSTOM';
  rate: number;  // R per kWh
  peakRate?: number;
  offPeakRate?: number;
  peakHours?: { start: string; end: string };
}

function calculateCost(energyData: EnergyDataPoint[], tariff: TariffStructure): number {
  let totalCost = 0;
  
  for (const point of energyData) {
    const energyKwh = (point.power / 1000) * (point.duration / 3600);  // Convert W·s to kWh
    
    if (tariff.peakRate && isPeakHour(point.timestamp, tariff.peakHours)) {
      totalCost += energyKwh * tariff.peakRate;
    } else {
      totalCost += energyKwh * (tariff.offPeakRate || tariff.rate);
    }
  }
  
  return totalCost;
}
```

###### **Backend Rule Engine Architecture**

**Objective**: Centralized rule evaluation engine for complex automation scenarios.

**Architecture**:
```
Rule Engine Flow:
1. Event Trigger (device state change, time, sensor reading)
2. Rule Evaluation (check conditions)
3. Action Execution (send commands, notifications)
4. Logging and Audit
```

**Implementation**:
```typescript
interface Rule {
  ruleId: string;
  name: string;
  trigger: Trigger;
  conditions: Condition[];
  actions: Action[];
  enabled: boolean;
}

interface Trigger {
  type: 'TIME' | 'DEVICE_STATE' | 'SENSOR_VALUE';
  config: any;
}

interface Condition {
  type: 'IF' | 'AND' | 'OR';
  left: string;
  operator: '==' | '>' | '<' | '!=';
  right: any;
}

interface Action {
  type: 'DEVICE_CONTROL' | 'NOTIFICATION' | 'WEBHOOK';
  config: any;
}

export const ruleEngine = {
  async evaluateRule(rule: Rule, context: any): Promise<boolean> {
    // Check if trigger matches
    if (!this.evaluateTrigger(rule.trigger, context)) {
      return false;
    }
    
    // Check all conditions
    for (const condition of rule.conditions) {
      if (!this.evaluateCondition(condition, context)) {
        return false;
      }
    }
    
    return true;
  },
  
  async executeActions(rule: Rule, context: any) {
    for (const action of rule.actions) {
      try {
        await this.executeAction(action, context);
      } catch (error) {
        console.error(`Failed to execute action:`, error);
      }
    }
  },
};
```

##### **Testing Criteria**

**Backend Testing**:
- [ ] Schedule executes within 1 second of scheduled time
- [ ] Signed commands verified successfully on device
- [ ] Load-shedding detection accuracy >95%
- [ ] Rate limiting blocks commands after 10/minute
- [ ] Replay attacks detected and blocked (100% success rate)
- [ ] Nonce cache prevents all replay attempts

**Frontend Testing**:
- [ ] Automation UI loads in <2 seconds
- [ ] Schedule creation completes in <1 second
- [ ] Energy alerts delivered within 5 seconds
- [ ] Charts render with 1000+ data points smoothly
- [ ] Cost calculations accurate within 1 cent

##### **Expected Challenges and Solutions**

| Challenge | Solution |
|-----------|----------|
| Schedule execution delays (cloud functions cold start) | Keep functions warm with periodic pings |
| Complex cron expressions confusing users | Provide visual schedule picker with previews |
| Rate limiting too restrictive for legitimate use | Allow temporary increase with 2FA confirmation |
| Nonce storage limited on ESP32 (100 nonces) | Circular buffer + NVS persistence |
| Time zone handling across multiple regions | Store all times in UTC, convert on display |
| Cost calculations inaccurate due to tariff changes | Allow users to update tariff, notify on changes |

##### **Deliverable Breakdown**

**Backend Deliverables**:
- Time-based scheduling API with signature verification
- Load-shedding detection system with schedule database
- Rate limiting middleware (10 commands/min)
- Replay attack prevention (nonce + timestamp)
- Rule engine with condition evaluation

**Frontend Deliverables**:
- Automation UI (mobile app + web dashboard)
- Energy usage alerts configuration
- Historical data visualization charts
- Cost tracking with tariff management

**Documentation Deliverables**:
- Automation API documentation
- Schedule creation guide for users
- Rule engine architecture diagram
- Security documentation (signatures, nonces)

**Success Metric**: Users can create schedules, receive energy alerts, and view historical data with zero security incidents.

---

#### **Week 6: Testing, Security Audit & Bug Fixes**

**Full Team Focus**: Comprehensive testing, security validation, and bug resolution to ensure production-readiness.

##### **Testing Strategy Overview**

**Objective**: Validate all system components through rigorous testing at multiple levels: unit, integration, end-to-end, security, and performance.

**Testing Pyramid**:
```
        /\
       /E2E\          10% - End-to-End Tests (Critical user flows)
      /______\
     /        \
    /Integration\     30% - Integration Tests (API + Device communication)
   /__________  \
  /              \
 /  Unit Tests    \   60% - Unit Tests (Individual functions)
/__________________\
```

##### **Comprehensive Testing Suite Setup**

###### **Unit Testing**

**Firmware Unit Tests** (ESP32-S3 with Unity test framework):

**Setup**:
```bash
cd /firmware
# Install PlatformIO for testing
pip install platformio

# Configure test environment
cat > platformio.ini <<EOF
[env:test]
platform = espressif32
framework = espidf
board = esp32-s3-devkitc-1
test_framework = unity
test_build_src = yes
EOF
```

**Example Test** - File: `/firmware/test/test_security.c`

```c
#include <unity.h>
#include "security/attestation.h"
#include "security/signature.h"

void setUp(void) {
    // Setup runs before each test
    init_security_module();
}

void tearDown(void) {
    // Cleanup runs after each test
    cleanup_security_module();
}

void test_signature_verification_success(void) {
    // Arrange
    const char *message = "test message";
    uint8_t signature[64];
    generate_test_signature(message, signature);
    
    // Act
    bool result = verify_ecdsa_signature(message, strlen(message), signature, 64);
    
    // Assert
    TEST_ASSERT_TRUE(result);
}

void test_signature_verification_failure_tampered_message(void) {
    // Arrange
    const char *original = "test message";
    const char *tampered = "test message modified";
    uint8_t signature[64];
    generate_test_signature(original, signature);
    
    // Act
    bool result = verify_ecdsa_signature(tampered, strlen(tampered), signature, 64);
    
    // Assert
    TEST_ASSERT_FALSE(result);
}

void test_nonce_replay_detection(void) {
    // Arrange
    const char *nonce = "a1b2c3d4e5f6g7h8";
    
    // Act - First use should succeed
    bool first_use = check_and_store_nonce(nonce);
    TEST_ASSERT_TRUE(first_use);
    
    // Act - Second use should fail (replay)
    bool replay = check_and_store_nonce(nonce);
    TEST_ASSERT_FALSE(replay);
}

void test_temperature_safety_cutoff(void) {
    // Arrange
    set_mock_temperature(75.0);  // Above 70°C threshold
    
    // Act
    temperature_safety_check();
    
    // Assert
    TEST_ASSERT_FALSE(relay_is_on());
    TEST_ASSERT_TRUE(is_in_safe_mode());
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_signature_verification_success);
    RUN_TEST(test_signature_verification_failure_tampered_message);
    RUN_TEST(test_nonce_replay_detection);
    RUN_TEST(test_temperature_safety_cutoff);
    
    return UNITY_END();
}
```

**Run Tests**:
```bash
pio test -e test
# Expected: 4 tests passed, 0 failed
```

**Backend Unit Tests** (Firebase Cloud Functions with Jest):

**File**: `/backend/functions/test/deviceAttestation.test.ts`

```typescript
import { attestDevice } from '../src/deviceAttestation';
import * as admin from 'firebase-admin';
import * as httpMocks from 'node-mocks-http';

describe('Device Attestation', () => {
  beforeEach(() => {
    jest.clearAllMocks();
  });
  
  test('should successfully attest valid device', async () => {
    // Arrange
    const req = httpMocks.createRequest({
      method: 'POST',
      body: {
        deviceId: 'device-001',
        signature: 'valid_signature_hex',
        firmwareVersion: 'v1.0.0',
        chipId: 'ESP32-S3-12345',
      },
    });
    const res = httpMocks.createResponse();
    
    // Mock Firestore
    jest.spyOn(admin.firestore(), 'collection').mockReturnValue({
      doc: jest.fn().mockReturnValue({
        get: jest.fn().mockResolvedValue({
          exists: true,
          data: () => ({ publicKey: 'mock_public_key' }),
        }),
      }),
    } as any);
    
    // Act
    await attestDevice(req, res);
    
    // Assert
    expect(res.statusCode).toBe(200);
    const data = res._getJSONData();
    expect(data.success).toBe(true);
    expect(data.token).toBeDefined();
  });
  
  test('should reject unregistered device', async () => {
    // Arrange
    const req = httpMocks.createRequest({
      method: 'POST',
      body: { deviceId: 'device-unknown' },
    });
    const res = httpMocks.createResponse();
    
    // Mock Firestore - device not found
    jest.spyOn(admin.firestore(), 'collection').mockReturnValue({
      doc: jest.fn().mockReturnValue({
        get: jest.fn().mockResolvedValue({ exists: false }),
      }),
    } as any);
    
    // Act
    await attestDevice(req, res);
    
    // Assert
    expect(res.statusCode).toBe(404);
    expect(res._getJSONData().error).toBe('Device not registered');
  });
  
  test('should enforce rate limiting', async () => {
    // Arrange
    const req = httpMocks.createRequest({
      method: 'POST',
      body: { deviceId: 'device-001', action: 'turn_on' },
    });
    const res = httpMocks.createResponse();
    
    // Mock rate limit exceeded
    jest.spyOn(rateLimit, 'check').mockResolvedValue(false);
    
    // Act
    await sendDeviceCommand(req, res);
    
    // Assert
    expect(res.statusCode).toBe(429);
    expect(res._getJSONData().error).toBe('Rate limit exceeded');
  });
});
```

**Run Tests**:
```bash
cd /backend/functions
npm test
# Expected: All tests passing, coverage >85%
```

**Mobile App Unit Tests** (Flutter with flutter_test):

**File**: `/app/test/device_command_test.dart`

```dart
import 'package:flutter_test/flutter_test.dart';
import 'package:mockito/mockito.dart';
import 'package:smartplugai/services/device_command_service.dart';

void main() {
  group('DeviceCommandService', () {
    late DeviceCommandService service;
    late MockHttpClient mockHttpClient;
    
    setUp(() {
      mockHttpClient = MockHttpClient();
      service = DeviceCommandService(httpClient: mockHttpClient);
    });
    
    test('should send command successfully', () async {
      // Arrange
      when(mockHttpClient.post(any, body: anyNamed('body')))
          .thenAnswer((_) async => MockResponse(200, '{"success": true}'));
      
      // Act
      final result = await service.sendCommand('device-001', 'turn_on');
      
      // Assert
      expect(result, true);
      verify(mockHttpClient.post(any, body: anyNamed('body'))).called(1);
    });
    
    test('should handle rate limit gracefully', () async {
      // Arrange
      when(mockHttpClient.post(any, body: anyNamed('body')))
          .thenAnswer((_) async => MockResponse(429, '{"error": "Rate limit exceeded"}'));
      
      // Act
      final result = await service.sendCommand('device-001', 'turn_on');
      
      // Assert
      expect(result, false);
    });
  });
}
```

###### **Integration Testing**

**Objective**: Test interactions between components (device ↔ backend ↔ frontend).

**MQTT Integration Test**:

**File**: `/backend/functions/test/integration/mqtt.test.ts`

```typescript
import * as mqtt from 'mqtt';
import { publishMqttCommand } from '../src/mqttClient';

describe('MQTT Integration', () => {
  let mqttClient: mqtt.MqttClient;
  
  beforeAll(async () => {
    mqttClient = mqtt.connect('mqtt://localhost:1883', {
      username: 'test_user',
      password: 'test_password',
    });
    
    await new Promise((resolve) => {
      mqttClient.on('connect', resolve);
    });
  });
  
  afterAll(() => {
    mqttClient.end();
  });
  
  test('should publish command to device topic', (done) => {
    // Arrange
    const deviceId = 'device-test-001';
    const command = { action: 'turn_on', timestamp: Date.now() };
    
    // Subscribe to topic
    mqttClient.subscribe(`devices/${deviceId}/commands`, (err) => {
      expect(err).toBeNull();
      
      // Listen for message
      mqttClient.on('message', (topic, message) => {
        const payload = JSON.parse(message.toString());
        expect(payload.action).toBe('turn_on');
        done();
      });
      
      // Act - Publish command
      publishMqttCommand(deviceId, command, 'mock_signature');
    });
  });
});
```

**API Integration Test**:

**File**: `/backend/functions/test/integration/api.test.ts`

```typescript
import * as request from 'supertest';
import { app } from '../src/app';

describe('API Integration', () => {
  test('should create schedule and execute command', async () => {
    // Step 1: Create schedule
    const scheduleResponse = await request(app)
      .post('/api/schedules')
      .set('Authorization', 'Bearer valid_token')
      .send({
        deviceId: 'device-001',
        action: 'turn_on',
        cronExpression: '0 7 * * *',
        timezone: 'Africa/Johannesburg',
      })
      .expect(200);
    
    expect(scheduleResponse.body.scheduleId).toBeDefined();
    
    // Step 2: Trigger schedule execution
    const scheduleId = scheduleResponse.body.scheduleId;
    const executeResponse = await request(app)
      .post(`/api/schedules/${scheduleId}/execute`)
      .set('Authorization', 'Bearer valid_token')
      .expect(200);
    
    expect(executeResponse.body.success).toBe(true);
  });
});
```

###### **End-to-End (E2E) Testing**

**Objective**: Test complete user journeys from mobile app to device control.

**E2E Test Scenarios**:
1. **User Registration → Device Pairing → Control Device**
2. **Create Schedule → Schedule Executes → Device Responds**
3. **Energy Alert Threshold → Device Exceeds → User Receives Notification**

**Implementation** (Using Appium for mobile E2E):

**File**: `/app/test/e2e/device_control.test.dart`

```dart
import 'package:flutter_driver/flutter_driver.dart';
import 'package:test/test.dart';

void main() {
  group('Device Control E2E', () {
    late FlutterDriver driver;
    
    setUpAll(() async {
      driver = await FlutterDriver.connect();
    });
    
    tearDownAll(() async {
      await driver.close();
    });
    
    test('complete device control flow', () async {
      // Step 1: Login
      await driver.tap(find.byValueKey('email_field'));
      await driver.enterText('test@smartplugai.com');
      await driver.tap(find.byValueKey('password_field'));
      await driver.enterText('SecurePassword123!');
      await driver.tap(find.byValueKey('login_button'));
      
      // Wait for home screen
      await driver.waitFor(find.text('My Devices'));
      
      // Step 2: Select device
      await driver.tap(find.text('Living Room Plug'));
      
      // Step 3: Turn on device
      await driver.tap(find.byValueKey('turn_on_button'));
      
      // Wait for confirmation
      await driver.waitFor(find.text('Device turned on'));
      
      // Step 4: Verify device state updated
      final stateText = await driver.getText(find.byValueKey('device_state'));
      expect(stateText, 'ON');
    });
  });
}
```

##### **Security Penetration Testing Procedures**

**Objective**: Identify vulnerabilities through simulated attacks (internal team + external auditors if budget allows).

###### **Internal Security Testing**

**Test Categories**:
1. **Authentication & Authorization**
2. **API Security**
3. **Device Security**
4. **Network Security**

**Testing Methodology**:

**1. Authentication Testing**:
```bash
# Test 1: Brute force protection
for i in {1..100}; do
  curl -X POST https://api.smartplugai.com/auth/login \
    -H "Content-Type: application/json" \
    -d '{"email":"test@test.com","password":"wrong"}' \
    -w "%{http_code}\n"
done
# Expected: Should block after 5 attempts (429 status code)

# Test 2: Token expiration
OLD_TOKEN="expired_jwt_token"
curl -X GET https://api.smartplugai.com/api/devices \
  -H "Authorization: Bearer $OLD_TOKEN"
# Expected: 401 Unauthorized

# Test 3: 2FA bypass attempt
curl -X POST https://api.smartplugai.com/auth/login \
  -H "Content-Type: application/json" \
  -d '{"email":"test@test.com","password":"correct_password","skip2fa":true}'
# Expected: 400 Bad Request or 2FA still required
```

**2. API Security Testing**:
```bash
# Test 1: SQL injection (should be N/A with Firestore, but test anyway)
curl -X GET "https://api.smartplugai.com/api/devices?deviceId=1' OR '1'='1"
# Expected: 400 Bad Request or no devices returned

# Test 2: Command injection
curl -X POST https://api.smartplugai.com/api/devices/device-001/command \
  -H "Authorization: Bearer $TOKEN" \
  -d '{"action":"turn_on; rm -rf /"}'
# Expected: 400 Bad Request, command rejected

# Test 3: Rate limiting
for i in {1..15}; do
  curl -X POST https://api.smartplugai.com/api/devices/device-001/command \
    -H "Authorization: Bearer $TOKEN" \
    -d '{"action":"turn_on"}' &
done
wait
# Expected: First 10 succeed, next 5 get 429 Rate Limit Exceeded

# Test 4: CORS policy
curl -X POST https://api.smartplugai.com/api/devices \
  -H "Origin: https://malicious-site.com" \
  -H "Authorization: Bearer $TOKEN"
# Expected: CORS error or blocked origin
```

**3. Device Security Testing**:

**Hardware Security**:
- [ ] Attempt to extract private key from ATECC608A (should fail - locked chip)
- [ ] Try to read flash encryption keys (should fail - secure boot enabled)
- [ ] Open enclosure and verify tamper detection triggers
- [ ] Test physical attack scenarios:
  - Remove ATECC608A from board → Device should refuse to boot
  - Replace firmware chip → Secure boot should reject unsigned firmware
  - Inject voltage spike → MOV protection should clamp

**Firmware Security**:
```bash
# Test 1: Upload unsigned firmware via OTA
python3 ota_upload.py --file unsigned_firmware.bin --device device-001
# Expected: Device rejects firmware (signature verification fails)

# Test 2: Replay attack (resend old command)
# Capture command from MQTT broker
mosquitto_sub -h mqtt.smartplugai.com -p 8883 -t "devices/device-001/commands" \
  --cafile ca.crt --cert client.crt --key client.key > captured_command.txt

# Wait 10 minutes, then replay
cat captured_command.txt | mosquitto_pub -h mqtt.smartplugai.com -p 8883 \
  -t "devices/device-001/commands" --cafile ca.crt --cert client.crt --key client.key
# Expected: Device rejects command (timestamp too old)

# Test 3: Man-in-the-middle attack
# Try to intercept MQTT traffic
tshark -i wlan0 -Y "mqtt" -T fields -e mqtt.msg
# Expected: All traffic encrypted (TLS 1.3), cannot read payload
```

**4. Network Security Testing**:
```bash
# Test 1: TLS version downgrade
openssl s_client -connect mqtt.smartplugai.com:8883 -tls1_1
# Expected: Connection refused (only TLS 1.2+ allowed)

# Test 2: Certificate validation
openssl s_client -connect mqtt.smartplugai.com:8883 -showcerts
# Expected: Valid certificate chain, no self-signed certs in production

# Test 3: DDoS protection
ab -n 10000 -c 100 https://api.smartplugai.com/api/devices
# Expected: Rate limiting kicks in, no service degradation
```

###### **External Security Audit (If Budget Allows)**

**Budget**: R10,000 - R50,000 for professional penetration testing

**Recommended Firms** (South Africa):
- **SecureData** (Cape Town) - R25,000 for IoT security audit
- **Performanta** (Johannesburg) - R30,000 for comprehensive audit
- **DIY Alternative**: Bug bounty program (R5,000 - R10,000 rewards)

**Audit Scope**:
- [ ] Full application penetration test (web + mobile + API)
- [ ] Hardware security assessment (ATECC608A, secure boot)
- [ ] Network security review (TLS, MQTT, certificate management)
- [ ] Code review (static analysis with Snyk or SonarQube)
- [ ] Compliance check (POPIA, PCI DSS readiness)

##### **Load Testing Methodology (100+ Concurrent Devices)**

**Objective**: Ensure backend can handle multiple devices simultaneously without performance degradation.

**Load Testing Tool**: Apache JMeter or Artillery.io

**Test Scenarios**:

**Scenario 1: Device Telemetry Flood**
```yaml
# artillery.yml
config:
  target: 'https://api.smartplugai.com'
  phases:
    - duration: 300  # 5 minutes
      arrivalRate: 20  # 20 devices/second
      name: "Ramp up"
scenarios:
  - name: "Device Telemetry"
    flow:
      - post:
          url: "/api/telemetry"
          headers:
            Authorization: "Bearer {{ $randomToken }}"
          json:
            deviceId: "device-{{ $randomNumber }}"
            power: "{{ $randomNumber(0, 3000) }}"
            voltage: 230
            current: "{{ $randomNumber(0, 13) }}"
            timestamp: "{{ $timestamp }}"
```

**Run Test**:
```bash
artillery run artillery.yml --output report.json
artillery report report.json
# Expected: 
# - 99th percentile latency <500ms
# - Error rate <1%
# - Backend auto-scales to handle load
```

**Scenario 2: Concurrent Device Commands**
```javascript
// JMeter test plan
ThreadGroup {
  threads: 100,  // 100 concurrent users
  rampUp: 60,    // Ramp up over 60 seconds
  duration: 300, // Run for 5 minutes
  
  HTTPRequest {
    method: 'POST',
    url: '/api/devices/${deviceId}/command',
    body: '{"action": "turn_${onOrOff}"}',
    headers: {
      'Authorization': 'Bearer ${token}'
    }
  },
  
  Assertions {
    responseCode: [200, 429],  // Allow rate limiting
    responseTime: < 1000ms
  }
}
```

**Performance Targets**:
- **API Response Time**: p50 <200ms, p95 <500ms, p99 <1000ms
- **Device Command Latency**: <2 seconds from API call to device execution
- **Throughput**: 1000 requests/second sustained
- **Error Rate**: <0.1% under normal load, <1% under stress
- **Database**: Firestore reads <100ms, writes <200ms

##### **User Acceptance Testing (UAT) Protocol with Friends/Family (10 Devices)**

**Objective**: Validate real-world usability and identify edge cases through beta testing.

**UAT Plan**:

**Participant Selection**:
- 10 beta testers (friends/family)
- Mix of technical and non-technical users
- Different home setups (apartments, houses, townhouses)
- Geographic spread (urban, suburban)

**Test Devices**:
- Deploy 10 fully assembled smart plugs with production firmware
- Pre-provisioned with unique device IDs and ATECC608A keys
- Labeled with device ID and serial number

**Testing Protocol**:

**Week 1: Installation & Onboarding**
- [ ] Tester receives device with user manual
- [ ] Tester installs device (plug into wall outlet)
- [ ] Tester downloads mobile app and creates account
- [ ] Tester pairs device using QR code
- [ ] Tester controls device (on/off) successfully

**Week 2: Daily Usage & Automation**
- [ ] Tester creates 2-3 automation schedules
- [ ] Tester monitors energy usage daily
- [ ] Tester receives energy alert (if threshold exceeded)
- [ ] Tester tests load-shedding detection (if applicable)

**Feedback Collection**:
- **Daily Survey**: 2-minute quick survey (1-10 ratings)
  - "How easy was it to use the app today?"
  - "Did you encounter any errors?"
  - "Would you recommend this product?"
- **Weekly Interview**: 15-minute video call
  - Discuss pain points
  - Feature requests
  - Usability issues
- **Bug Reporting**: In-app feedback button
  - Automatically includes logs and device state
  - Users can attach screenshots

**Success Criteria**:
- [ ] 90%+ successful device pairings
- [ ] 80%+ daily active usage
- [ ] <5 critical bugs reported
- [ ] Average user satisfaction >4.5/5

##### **Bug Triage Process and Priority System**

**Objective**: Systematically categorize and prioritize bugs for efficient resolution.

**Bug Priority Levels**:

| Priority | Description | SLA | Examples |
|----------|-------------|-----|----------|
| **P0 - Critical** | Service down, data loss, security breach | Fix within 4 hours | Device attestation failing globally, data breach detected |
| **P1 - High** | Major feature broken, affects many users | Fix within 24 hours | OTA updates failing, cannot control devices |
| **P2 - Medium** | Feature partially broken, workaround exists | Fix within 1 week | Chart not loading, schedule creation slow |
| **P3 - Low** | Minor UI issue, cosmetic bug | Fix when time permits | Button color incorrect, typo in text |

**Triage Process**:
1. **Bug Report Received** (via GitHub Issues, Sentry, user feedback)
2. **Auto-Categorization**: Sentry auto-labels crashes as P1
3. **Manual Review**: Team lead reviews daily, assigns priority
4. **Assignment**: Bugs assigned to developer based on expertise
5. **Resolution**: Developer fixes, writes regression test
6. **Verification**: QA or team lead verifies fix
7. **Deployment**: Fix deployed via CI/CD pipeline

**Tools**:
- **Bug Tracking**: GitHub Issues with labels (P0, P1, P2, P3, security, frontend, backend)
- **Error Monitoring**: Sentry for backend + Firebase Crashlytics for mobile
- **Alerting**: PagerDuty for P0/P1 bugs (SMS + phone call)

##### **Performance Optimization Strategies**

**Objective**: Improve application speed and responsiveness.

**Backend Optimizations**:
1. **Database Indexing**: Index Firestore queries
   ```javascript
   // Before: Full collection scan
   firestore.collection('devices').where('userId', '==', userId).get();
   
   // After: Create composite index
   // Index: (userId, lastSeen DESC)
   ```
2. **Caching**: Redis cache for frequently accessed data
   ```typescript
   // Cache device list for 5 minutes
   const devices = await redis.get(`devices:${userId}`);
   if (!devices) {
     const devicesFromDb = await firestore.collection('devices')
       .where('userId', '==', userId).get();
     await redis.setex(`devices:${userId}`, 300, JSON.stringify(devicesFromDb));
     return devicesFromDb;
   }
   return JSON.parse(devices);
   ```
3. **Function Optimization**: Keep Cloud Functions warm
   ```typescript
   // Ping function every 5 minutes to prevent cold start
   export const keepWarm = functions.pubsub.schedule('every 5 minutes').onRun(() => {
     console.log('Keep warm ping');
   });
   ```

**Frontend Optimizations**:
1. **Lazy Loading**: Load screens on-demand
   ```dart
   // Flutter lazy loading
   MaterialPageRoute(builder: (_) => AutomationScreen())
   ```
2. **Image Optimization**: Compress images, use cached_network_image
3. **State Management**: Use efficient state management (Provider, Riverpod)

**Device Optimizations**:
1. **Batch Telemetry**: Send data every 5 seconds instead of real-time
2. **Power Saving**: ESP32 light sleep between measurements
3. **MQTT QoS**: Use QoS 1 (at least once delivery) instead of QoS 2

##### **Security Audit Checklist**

**Pre-Deployment Security Review**:
- [ ] All secrets removed from source code (use environment variables)
- [ ] API endpoints require authentication
- [ ] Rate limiting enabled on all public endpoints
- [ ] CORS configured correctly (whitelist only)
- [ ] SQL injection tests passed (N/A for Firestore)
- [ ] XSS tests passed (input sanitization)
- [ ] CSRF tokens implemented for web dashboard
- [ ] Secure boot enabled on all devices
- [ ] Flash encryption enabled on all devices
- [ ] ATECC608A locked on all devices
- [ ] OTA updates require valid signatures
- [ ] Device attestation enforced
- [ ] TLS 1.3 enforced (no downgrades)
- [ ] Certificate pinning in mobile app
- [ ] Firestore security rules validated
- [ ] Sensitive data encrypted at rest (AES-256-GCM)
- [ ] Logging does not expose PII or secrets
- [ ] Error messages do not reveal system internals

##### **Test Coverage Requirements**

**Code Coverage Targets**:
- **Backend**: 85%+ line coverage, 90%+ branch coverage
- **Mobile App**: 75%+ line coverage (UI testing challenging)
- **Firmware**: 80%+ line coverage for security-critical functions

**Measure Coverage**:
```bash
# Backend (Jest)
cd /backend/functions
npm run test:coverage
# Output: Istanbul coverage report

# Mobile App (Flutter)
cd /app
flutter test --coverage
genhtml coverage/lcov.info -o coverage/html
open coverage/html/index.html

# Firmware (PlatformIO)
cd /firmware
pio test --verbose --coverage
```

**Critical Paths Requiring 100% Coverage**:
- Device attestation
- Signature verification
- Nonce replay prevention
- Rate limiting
- Temperature safety cutoff
- OTA signature verification

##### **Critical Bug Fixing Workflow**

**P0 Critical Bug Response**:
1. **Alert Triggered**: PagerDuty notifies on-call engineer
2. **Incident Created**: Create incident in GitHub Issues
3. **War Room**: Team gathers (Slack call or in-person)
4. **Root Cause Analysis**: Investigate logs, traces, metrics
5. **Hotfix**: Develop fix in separate branch
6. **Testing**: Rapid testing (automated + manual)
7. **Deployment**: Deploy via CI/CD (with rollback plan)
8. **Monitoring**: Watch metrics for 1 hour post-deployment
9. **Postmortem**: Document incident, root cause, prevention

**Example P0 Scenario**:
> "Device attestation failing for all devices - no devices can connect"

**Response**:
1. Check backend logs → Certificate expired
2. Renew certificate with Let's Encrypt
3. Deploy updated certificate to production
4. Verify devices reconnecting
5. Total downtime: 22 minutes
6. Postmortem: Implement automated certificate renewal

##### **Testing Criteria Summary**

**Pass Criteria**:
- [ ] All unit tests passing (100%)
- [ ] Integration tests passing (100%)
- [ ] E2E tests passing for critical flows (100%)
- [ ] Security penetration tests: 0 critical vulnerabilities
- [ ] Load test: 100 concurrent devices with <1% error rate
- [ ] UAT: 8/10 testers rate app >4/5
- [ ] Code coverage: Backend 85%+, Mobile 75%+, Firmware 80%+
- [ ] Performance: API p95 latency <500ms

##### **Expected Challenges and Solutions**

| Challenge | Solution |
|-----------|----------|
| Intermittent WiFi disconnects during testing | Implement exponential backoff reconnection, test in poor network conditions |
| False positives in security scans | Manually verify and whitelist known safe patterns |
| Load testing overwhelms free-tier Firebase | Upgrade to Blaze plan temporarily, use load testing budget |
| UAT testers not providing feedback | Gamify feedback (R50 voucher for most helpful tester) |
| Critical bug found day before launch | Delay launch, fix properly (do not rush) |
| External audit finds vulnerabilities | Prioritize fixes, document remediation plan |

##### **Deliverable Breakdown**

**Testing Deliverables**:
- Comprehensive test suite (unit + integration + E2E)
- Security penetration test report (internal)
- Load test results and performance benchmarks
- UAT feedback summary and action items
- Bug tracker with all identified issues

**Security Deliverables**:
- Security audit checklist (completed)
- Vulnerability assessment report
- Remediation plan for identified issues
- Code coverage reports (>85% backend)

**Documentation Deliverables**:
- Testing strategy document
- Security testing procedures
- Performance optimization guide
- Bug triage workflow documentation

**Success Metric**: System passes all tests, security audit reveals 0 critical vulnerabilities, ready for production deployment with confidence.

---

#### **Week 7: Pilot Deployment & Documentation**

**Full Team Effort**: Deploy pilot devices, establish monitoring, and create comprehensive documentation for users and developers.

##### **Deployment Strategy Overview**

**Objective**: Deploy 10-20 smart plugs to pilot users (friends/family) with 24/7 monitoring and support infrastructure in place.

**Pilot Program Goals**:
1. Validate system reliability in real-world conditions
2. Gather user feedback for Phase 2 improvements
3. Test incident response procedures
4. Build case studies for marketing
5. Establish operational excellence baseline

##### **Deployment Process for 10-20 Devices to Pilot Users**

###### **Device Preparation Workflow**

**Pre-Deployment Checklist (Per Device)**:

**Day 1: Hardware Preparation**
```text
Device ID: device-001
Serial Number: SPA-2024-001
MAC Address: [Record from ESP32]

Hardware Checklist:
[√] PCB assembled and soldered (all connections verified)
[√] ATECC608A provisioned with unique private key
[√] ATECC608A configuration locked (irreversible)
[√] ESP32-S3 secure boot enabled
[√] ESP32-S3 flash encryption enabled
[√] Temperature sensor (DS18B20) calibrated
[√] Current sensor (SCT-013) calibrated with known load
[√] Relay tested (1000 on/off cycles)
[√] Enclosure assembled with tamper detection
[√] Anti-tamper screws installed (Torx T10)
[√] Void-if-removed seals applied
[√] Mains wiring inspected (continuity + insulation test)
[√] Ground connection verified (<0.1Ω)
[√] 500W load test passed (60 minutes, temp <60°C)
[√] Safety cut-off tested (verified at 70°C)
```

**Day 2: Firmware & Software Configuration**
```bash
# Step 1: Flash production firmware
cd /firmware
idf.py build
esptool.py --chip esp32s3 --port /dev/ttyUSB0 \
  write_flash 0x0 build/bootloader.bin \
  0x8000 build/partition-table.bin \
  0x10000 build/smartplug.bin

# Step 2: Provision device in backend
curl -X POST https://api.smartplugai.com/admin/devices/provision \
  -H "Authorization: Bearer $ADMIN_TOKEN" \
  -H "Content-Type: application/json" \
  -d '{
    "deviceId": "device-001",
    "serialNumber": "SPA-2024-001",
    "hardwareVersion": "v1.0",
    "firmwareVersion": "v1.0.0",
    "publicKey": "'"$(cat device-001-public-key.pem)"'",
    "macAddress": "AA:BB:CC:DD:EE:01",
    "productionDate": "2024-12-01",
    "calibrationData": {
      "currentSensorOffset": 0.02,
      "voltageRatio": 1.003,
      "powerFactor": 0.98
    }
  }'

# Step 3: Register device attestation
curl -X POST https://api.smartplugai.com/admin/devices/device-001/attest \
  -H "Authorization: Bearer $ADMIN_TOKEN"

# Expected Response:
# {
#   "success": true,
#   "deviceId": "device-001",
#   "attestationStatus": "VERIFIED",
#   "message": "Device ready for deployment"
# }
```

**Day 3: Quality Assurance Testing**
```text
QA Test Suite (30 minutes per device):

Functional Tests:
[√] Power on/off via mobile app (5 cycles)
[√] Schedule creation and execution
[√] Real-time power monitoring (±1% accuracy)
[√] Energy usage calculation (compare with kill-a-watt meter)
[√] WiFi reconnection after network loss
[√] OTA firmware update (from v1.0.0 to v1.0.1 test build)
[√] Tamper detection (open enclosure, verify alert)
[√] Temperature monitoring (verify readings)

Security Tests:
[√] Device attestation successful
[√] Command signature verification
[√] Replay attack blocked
[√] Rate limiting enforced (10 commands/min)
[√] TLS connection established (no downgrades)

Performance Tests:
[√] Command latency <2 seconds
[√] Telemetry update frequency: 5 seconds
[√] Mobile app responsive (<300ms UI updates)

Safety Tests:
[√] Over-temperature cut-off triggers at 70°C
[√] Fuse integrity check (continuity test)
[√] Ground fault protection verified
[√] Load test at rated capacity (500W for 1 hour)
```

###### **User Assignment and Distribution**

**Pilot User Selection Criteria**:
- [ ] Lives within 50km radius (for in-person support if needed)
- [ ] Has stable WiFi connection (>=10 Mbps)
- [ ] Willing to provide weekly feedback
- [ ] Mix of user profiles:
  - 4 tech-savvy users (developers, IT professionals)
  - 4 average users (non-technical)
  - 2 elderly users (accessibility testing)

**Distribution Process**:
```text
Per-User Package Contents:
1. Smart Plug device (assembled and tested)
2. Quick Start Guide (2-page color printout)
3. User Manual (full documentation, 20 pages)
4. Calibration Certificate (signed by QA lead)
5. Safety Instructions (warning labels, electrical safety)
6. Support Contact Card (phone number, email, WhatsApp)
7. Feedback Form (weekly survey QR code)
8. Return Shipping Label (prepaid, in case of issues)

Hand-Off Checklist:
[√] Device physically handed to user (or shipped with tracking)
[√] User scans QR code to download mobile app
[√] Walkthrough device pairing process together
[√] Demonstrate basic on/off control
[√] Show energy monitoring dashboard
[√] Explain how to create schedules
[√] Provide support contact information
[√] Capture user signature on deployment form
[√] Record device ID and user mapping in database
```

**Database Record** (Firestore):
```json
{
  "deployments": {
    "deploy-001": {
      "deviceId": "device-001",
      "userId": "user_pilot_001",
      "userName": "John Doe",
      "userEmail": "john@example.com",
      "userPhone": "+27821234567",
      "deploymentDate": "2024-12-15T10:00:00Z",
      "location": "Cape Town, Western Cape",
      "devicePlacement": "Kitchen outlet, near refrigerator",
      "expectedLoad": "Kettle, microwave",
      "status": "ACTIVE",
      "feedbackCompleted": 0,
      "incidentsReported": 0
    }
  }
}
```

##### **24/7 Monitoring Setup Details**

**Objective**: Proactively detect and respond to issues before users notice.

###### **Monitoring Infrastructure**

**1. Device Health Monitoring**

**Metrics to Track**:
- Device online/offline status
- Last telemetry timestamp (detect silent failures)
- Firmware version
- WiFi signal strength (RSSI)
- Temperature readings
- Power consumption
- Command success/failure rate
- Attestation status

**Implementation** (Firebase + Grafana):

**Dashboard Widgets**:
```yaml
Monitoring Dashboard (Grafana):

Panel 1: Device Status Map
- Green: Online and healthy (last seen <5 min)
- Yellow: Degraded (last seen 5-15 min)
- Red: Offline (last seen >15 min)
- Display: 10 device cards with status icons

Panel 2: System Health Metrics
- Total devices online: 10/10
- Average response time: 1.2s
- Error rate: 0.01%
- Attestation success rate: 100%

Panel 3: Temperature Trends
- Line chart: Device temps over last 24 hours
- Alert threshold: 60°C (warning), 70°C (critical)
- Current max temp: 42°C (device-007)

Panel 4: Energy Usage
- Bar chart: kWh consumed per device (today)
- Total energy: 12.5 kWh
- Cost estimate: R15.00 (at R1.20/kWh)

Panel 5: Recent Alerts
- [14:23] device-003: Temperature warning (62°C)
- [12:15] device-008: WiFi reconnected after 2 min outage
- [09:00] device-005: Schedule executed successfully
```

**Alert Configuration** (Firebase Cloud Functions):
```typescript
export const monitorDeviceHealth = functions.pubsub
  .schedule('every 5 minutes')
  .onRun(async (context) => {
    const now = Date.now();
    const fiveMinutesAgo = now - (5 * 60 * 1000);
    
    // Query all pilot devices
    const devicesSnapshot = await admin.firestore()
      .collection('devices')
      .where('deploymentStatus', '==', 'PILOT')
      .get();
    
    for (const deviceDoc of devicesSnapshot.docs) {
      const device = deviceDoc.data();
      const lastSeen = device.lastTelemetry?.toMillis() || 0;
      
      // Alert: Device offline
      if (now - lastSeen > fiveMinutesAgo) {
        await sendAlert({
          severity: 'HIGH',
          title: `Device Offline: ${device.deviceId}`,
          message: `Last seen: ${new Date(lastSeen).toLocaleString()}`,
          actionRequired: 'Contact user to check device',
          affectedUser: device.userId,
        });
      }
      
      // Alert: High temperature
      if (device.temperature > 60) {
        await sendAlert({
          severity: 'MEDIUM',
          title: `High Temperature: ${device.deviceId}`,
          message: `Current temp: ${device.temperature}°C`,
          actionRequired: 'Monitor, may trigger safety cut-off at 70°C',
        });
      }
      
      // Alert: Attestation failure
      if (device.lastAttestationStatus === 'FAILED') {
        await sendAlert({
          severity: 'CRITICAL',
          title: `Attestation Failed: ${device.deviceId}`,
          message: 'Device may be compromised',
          actionRequired: 'Immediate investigation required',
          affectedUser: device.userId,
        });
      }
    }
  });
```

**2. Backend Service Monitoring**

**Uptime Monitoring**:
- **Tool**: UptimeRobot (free tier: 50 monitors, 5-min intervals)
- **Endpoints to Monitor**:
  - API Health: `https://api.smartplugai.com/health` (expect 200 OK)
  - MQTT Broker: `mqtt://mqtt.smartplugai.com:8883` (expect connection)
  - Web Dashboard: `https://dashboard.smartplugai.com` (expect 200 OK)

**Performance Monitoring**:
- **Tool**: Firebase Performance Monitoring
- **Metrics**:
  - API response time (p50, p95, p99)
  - Function execution time
  - Database query latency
  - Error rate (4xx, 5xx responses)

**3. Security Monitoring**

**Security Event Log**:
```typescript
// Log all security-relevant events
export const logSecurityEvent = functions.https.onCall(async (data, context) => {
  await admin.firestore().collection('security_events').add({
    eventType: data.eventType,  // ATTESTATION_FAILED, INVALID_SIGNATURE, etc.
    deviceId: data.deviceId,
    userId: context.auth?.uid,
    ipAddress: context.rawRequest.ip,
    userAgent: context.rawRequest.headers['user-agent'],
    timestamp: admin.firestore.FieldValue.serverTimestamp(),
    details: data.details,
  });
});
```

**Security Dashboard** (Grafana):
- Failed login attempts (last 24 hours)
- Attestation failures by device
- Rate limit violations
- Suspicious activity (e.g., commands from unusual locations)

##### **Incident Response Procedures and Escalation Paths**

**Objective**: Rapid response to production issues to minimize user impact.

###### **Incident Severity Levels**

| Severity | Definition | Response Time | Escalation |
|----------|------------|---------------|------------|
| **SEV-1** | Service down, data breach, safety hazard | Immediate (15 min) | On-call engineer + Team lead |
| **SEV-2** | Major feature broken, multiple users affected | 1 hour | On-call engineer |
| **SEV-3** | Minor feature issue, single user affected | 4 hours | Next business day |
| **SEV-4** | Cosmetic bug, no user impact | 1 week | Backlog |

###### **Incident Response Workflow**

**SEV-1 Incident Example**: "Device not turning off, potential fire hazard"

**Response Steps**:
```text
T+0 min: Alert received (user calls support hotline)
  - Support logs incident in GitHub Issues
  - Tags: [SEV-1] [SAFETY] [device-005]
  - PagerDuty pages on-call engineer (SMS + phone call)

T+5 min: On-call engineer acknowledges
  - Creates Slack war room: #incident-device-005
  - Invites team lead and hardware engineer
  - Remote diagnosis begins

T+10 min: Root cause identified
  - Relay stuck in "on" position (mechanical failure)
  - Device not responding to off commands
  - Temperature: 45°C (below safety threshold, but rising)

T+12 min: Immediate mitigation
  - Engineer sends emergency shutdown command (bypass normal relay)
  - Command: Cut power by entering safe mode
  - Device successfully enters safe mode, relay forced off

T+15 min: User notified
  - Call user: "We've remotely disabled the device for safety"
  - Instruct user to unplug device from wall outlet
  - Arrange immediate replacement device delivery

T+30 min: Post-incident analysis
  - Review device logs
  - Identify relay model quality issue
  - Decision: Recall all devices with same relay batch

T+60 min: Incident closed
  - User receives replacement device (next-day delivery)
  - Root cause: Relay model SRD-05VDC-SL-C has 0.5% failure rate
  - Action: Source new relay model (Omron G5LE-1-12VDC)
  - Prevention: Add relay health monitoring (cycle count)

T+24 hours: Postmortem published
  - Document incident timeline
  - Share with team for learning
  - Update runbooks with new procedures
```

**Communication Templates**:

**User Notification (SEV-1)**:
```text
Subject: Urgent: Your Smart Plug has been remotely disabled for safety

Dear [User Name],

We detected an issue with your Smart Plug (Device ID: device-005) and 
have remotely disabled it for your safety. 

What happened: The relay component experienced a mechanical failure and 
was unable to turn off on command.

What we did: Our monitoring system detected the issue and remotely shut 
down the device within 15 minutes.

What you should do:
1. Unplug the device from the wall outlet immediately
2. Do not attempt to use the device
3. We will deliver a replacement device tomorrow at no cost

Your safety is our top priority. We sincerely apologize for the inconvenience.

Support: Call us 24/7 at +27-82-SMARTPLUG or email support@smartplugai.com

Regards,
Smart Plug AI Support Team
```

###### **Escalation Paths**

**On-Call Rotation**:
```yaml
Week 1: Engineer A (primary), Engineer B (backup)
Week 2: Engineer B (primary), Engineer C (backup)
Week 3: Engineer C (primary), Engineer A (backup)

On-Call Responsibilities:
- Respond to PagerDuty alerts within 15 minutes
- Triage incidents and assign severity
- Coordinate response with team members
- Document incident in GitHub Issues
- Update status page (status.smartplugai.com)
- Communicate with affected users
```

**Escalation Matrix**:
```
SEV-1 Incident:
  Alert → On-Call Engineer (immediate)
      ↓ (if no response in 15 min)
  Escalate → Backup Engineer + Team Lead
      ↓ (if not resolved in 1 hour)
  Escalate → CTO + All team members

SEV-2 Incident:
  Alert → On-Call Engineer (1 hour SLA)
      ↓ (if not resolved in 4 hours)
  Escalate → Team Lead

SEV-3/4 Incidents:
  No escalation, handled during business hours
```

##### **User Onboarding Materials Creation**

**Objective**: Empower users to successfully set up and use their smart plugs with minimal support.

###### **Quick Start Guide (2-Page Printout)**

**Page 1: Setup in 3 Easy Steps**

```text
┌────────────────────────────────────────────────────┐
│  SMART PLUG AI - QUICK START GUIDE                 │
├────────────────────────────────────────────────────┤
│                                                     │
│  STEP 1: PLUG IT IN                                │
│  ┌──────────┐                                      │
│  │  [Plug]  │  → Insert device into wall outlet    │
│  │   ▼▼▼    │     (230V AC, standard 3-pin)       │
│  └──────────┘                                      │
│  LED should start blinking blue (pairing mode)     │
│                                                     │
│  STEP 2: DOWNLOAD APP                              │
│  [QR Code]  ← Scan to download app                 │
│                (Android / iOS)                     │
│  Or search "Smart Plug AI" in app store            │
│                                                     │
│  STEP 3: PAIR DEVICE                               │
│  1. Open app, create account (email + password)    │
│  2. Tap "Add Device" button                        │
│  3. Scan QR code on device label                   │
│  4. Enter WiFi password                            │
│  5. Wait 30 seconds... Done! ✓                     │
│                                                     │
│  SUPPORT: +27-82-123-4567 | support@smartplugai.com│
└────────────────────────────────────────────────────┘
```

**Page 2: Basic Features & Safety**

```text
┌────────────────────────────────────────────────────┐
│  BASIC FEATURES                                    │
├────────────────────────────────────────────────────┤
│  ⚡ POWER CONTROL                                  │
│     Tap power button in app to turn on/off         │
│     Status LED: Green = On, Red = Off              │
│                                                     │
│  📊 ENERGY MONITORING                              │
│     View real-time power usage (Watts)             │
│     Track daily/weekly/monthly consumption         │
│     Cost estimates based on your tariff            │
│                                                     │
│  ⏰ SCHEDULES                                      │
│     Automate your devices                          │
│     Example: "Turn on at 7AM every weekday"        │
│                                                     │
│  🔔 ALERTS                                         │
│     Get notified when energy usage is high         │
│     Load-shedding detection (if available)         │
│                                                     │
├────────────────────────────────────────────────────┤
│  SAFETY WARNINGS ⚠️                                │
├────────────────────────────────────────────────────┤
│  ✓ Do not exceed 500W (2.2A) load                 │
│  ✓ Do not use with heating appliances (heaters,   │
│    irons) - fire hazard                            │
│  ✓ Do not use in wet locations (bathroom, outdoor)│
│  ✓ Do not open enclosure (voids warranty, safety) │
│  ✓ Unplug device during thunderstorms              │
│                                                     │
│  TROUBLESHOOTING                                   │
│  Problem: Device not pairing                       │
│  Solution: Hold reset button 10 seconds, try again │
│                                                     │
│  Problem: Device offline                           │
│  Solution: Check WiFi connection, restart device   │
│                                                     │
│  For more help: www.smartplugai.com/support        │
└────────────────────────────────────────────────────┘
```

###### **Complete User Manual Structure**

**User Manual** (20-page PDF):

**Table of Contents**:
```text
1. Introduction (1 page)
   - Welcome
   - Package contents
   - Safety certifications

2. Getting Started (3 pages)
   - Hardware overview
   - LED indicator meanings
   - Initial setup walkthrough
   - WiFi configuration

3. Mobile App Guide (5 pages)
   - App interface tour
   - Device control
   - Energy monitoring
   - Creating schedules
   - Setting up alerts
   - Viewing history

4. Advanced Features (4 pages)
   - Automation rules
   - Load-shedding detection
   - Energy cost tracking
   - Multi-device management
   - Sharing devices with family

5. Safety & Maintenance (3 pages)
   - Electrical safety guidelines
   - Load capacity warnings
   - Temperature monitoring
   - Firmware updates (OTA)
   - Cleaning and care

6. Troubleshooting (2 pages)
   - Common issues and solutions
   - Error code reference
   - Factory reset procedure
   - Diagnostic logs

7. Technical Specifications (1 page)
   - Electrical ratings
   - WiFi specifications
   - Dimensions and weight
   - Compliance certifications

8. Warranty & Support (1 page)
   - Warranty terms (12 months)
   - Return policy
   - Contact information
   - FAQ
```

**Sample Section** - File: `/docs/user_manual.md` (excerpt)

```markdown
## 3. Mobile App Guide

### 3.1 Device Control

#### Turning Devices On/Off

1. Open the Smart Plug AI app
2. Select your device from the home screen
3. Tap the large power button
4. Device will respond within 2 seconds
5. LED will turn green (on) or red (off)

**Pro Tip**: Use 3D Touch (iOS) or long press (Android) on the app icon for 
quick device control without opening the app.

#### Viewing Real-Time Power Usage

The main device screen shows:
- **Current Power**: 150W (updates every 5 seconds)
- **Voltage**: 230V AC
- **Current**: 0.65A
- **Daily Energy**: 2.3 kWh
- **Estimated Cost**: R2.76 (based on your tariff)

[Screenshot of device control screen]

### 3.2 Creating Schedules

Automate your devices to turn on/off at specific times.

**Example: Turn on coffee maker at 7AM weekdays**

1. Tap the "Automation" tab
2. Tap the "+" button (top right)
3. Select device: "Kitchen Coffee Maker"
4. Select action: "Turn On"
5. Select time: 07:00
6. Select days: Monday, Tuesday, Wednesday, Thursday, Friday
7. Tap "Create Schedule"
8. Done! ✓

Your coffee will be ready when you wake up.

**Important**: Device must be online for schedules to work. If device is offline, 
the schedule will execute once device reconnects.
```

##### **Developer Documentation Requirements**

**Objective**: Enable future developers to understand, maintain, and extend the system.

###### **API Documentation**

**File**: `/docs/API.md` (expand existing)

**Additional Sections**:
```markdown
## Device Control API

### POST /api/devices/{deviceId}/command

Send a control command to a device.

**Authentication**: Required (Bearer token)

**Rate Limit**: 10 requests/minute per device

**Request Body**:
```json
{
  "action": "turn_on | turn_off",
  "source": "manual | schedule | automation"
}
```

**Response** (200 OK):
```json
{
  "success": true,
  "commandId": "cmd_abc123",
  "timestamp": 1638360000000,
  "estimatedExecutionTime": 1500,
  "remaining": 9
}
```

**Response** (429 Rate Limit Exceeded):
```json
{
  "error": "Rate limit exceeded",
  "retryAfter": 60,
  "limit": 10
}
```

**Security**: All commands are signed with server private key and verified by 
device using embedded public key. Commands include timestamp and nonce to 
prevent replay attacks.

**Example** (cURL):
```bash
curl -X POST https://api.smartplugai.com/api/devices/device-001/command \
  -H "Authorization: Bearer YOUR_TOKEN" \
  -H "Content-Type: application/json" \
  -d '{"action": "turn_on", "source": "manual"}'
```

**Example** (Python):
```python
import requests

headers = {
    "Authorization": f"Bearer {token}",
    "Content-Type": "application/json"
}

data = {
    "action": "turn_on",
    "source": "manual"
}

response = requests.post(
    "https://api.smartplugai.com/api/devices/device-001/command",
    headers=headers,
    json=data
)

print(response.json())
```
```

###### **Architecture Documentation**

**File**: `/docs/ARCHITECTURE.md` (expand existing)

**Add Deployment Architecture**:
```markdown
## Production Deployment Architecture

### Cloud Infrastructure (Google Cloud Platform)

```
┌─────────────────────────────────────────────────────┐
│                   CLIENT LAYER                       │
├─────────────────────────────────────────────────────┤
│  [Mobile App]  [Web Dashboard]  [IoT Devices]      │
│   (Flutter)     (React)          (ESP32-S3)         │
└──────┬────────────────┬────────────────┬────────────┘
       │                │                │
       ▼                ▼                ▼
┌─────────────────────────────────────────────────────┐
│               API GATEWAY LAYER                      │
├─────────────────────────────────────────────────────┤
│  [Firebase Hosting]  [Cloud Load Balancer]          │
│  TLS 1.3 Termination                                │
└──────┬──────────────────────┬──────────────────────┘
       │                      │
       ▼                      ▼
┌──────────────────┐   ┌─────────────────────────┐
│ BACKEND SERVICES │   │   MQTT BROKER           │
├──────────────────┤   ├─────────────────────────┤
│ Cloud Functions  │   │ Cloud IoT Core          │
│ - Device Control │   │ - Device Pub/Sub        │
│ - Attestation    │   │ - TLS 1.3 + mTLS        │
│ - Schedules      │   │ - 100 devices capacity  │
│ - Alerts         │   └─────────────────────────┘
└────────┬─────────┘
         │
         ▼
┌─────────────────────────────────────────────────────┐
│                 DATA LAYER                           │
├─────────────────────────────────────────────────────┤
│  [Firestore]         [Realtime DB]    [Storage]     │
│  - Device metadata   - Rate limits    - Firmware    │
│  - User data         - Nonce cache    - Logs        │
│  - Telemetry         - Real-time      - Backups     │
│  - Schedules         state                           │
└─────────────────────────────────────────────────────┘
```

### Security Zones

**Zone 1: Public (Untrusted)**
- Mobile app (user devices)
- Web dashboard (user browsers)

**Zone 2: DMZ (Semi-Trusted)**
- API Gateway (Firebase Hosting + Cloud Load Balancer)
- Rate limiting, DDoS protection

**Zone 3: Application (Trusted)**
- Cloud Functions (business logic)
- MQTT Broker (device communication)

**Zone 4: Data (Highly Trusted)**
- Firestore (persistent storage)
- Secrets Manager (encryption keys)

**Security Controls**:
- All zones communicate over TLS 1.3
- Zone 3 & 4 only accessible via private IPs
- Firestore rules enforce server-side authentication
- Secrets never stored in code or environment variables
```

##### **Security Audit Report (Appendix F) Compilation**

**Objective**: Document all security measures, testing results, and compliance status.

**File**: `/docs/APPENDIX_F_SECURITY_AUDIT.md`

```markdown
# Appendix F: Security Audit Report

**Project**: Smart Plug AI - Phase 1 MVP  
**Audit Date**: December 2024  
**Version**: 1.0  
**Status**: PASSED

## Executive Summary

The Smart Plug AI system has undergone comprehensive security testing including 
internal penetration tests, code reviews, and security architecture reviews. 

**Overall Security Score**: 94/100 (A-grade)

**Key Findings**:
- ✅ 0 Critical vulnerabilities
- ⚠️ 2 Medium-severity issues (addressed)
- ✅ All devices pass attestation
- ✅ TLS 1.3 enforced across all communications
- ✅ Secure boot and flash encryption enabled on all devices

## 1. Device Security

### 1.1 Hardware Security
- **ATECC608A Secure Element**: ✅ PASSED
  - Unique private keys provisioned per device
  - Configuration locked (irreversible)
  - Keys cannot be extracted (tested with SPA attack)
  
- **Secure Boot**: ✅ PASSED
  - ESP32-S3 secure boot V2 enabled
  - Only signed firmware can execute
  - Attempted unsigned firmware flash: REJECTED
  
- **Flash Encryption**: ✅ PASSED
  - AES-256 flash encryption enabled
  - Firmware dump unreadable
  - Decryption keys stored in eFuse (OTP)

- **Physical Tamper Detection**: ✅ PASSED
  - MAX6316 watchdog with tamper input
  - Opening enclosure triggers alert within 1 second
  - Device enters safe mode (requires re-attestation)

### 1.2 Firmware Security
- **Code Signing**: ✅ PASSED
  - OTA updates signed with RSA-3072
  - Device verifies signature before flashing
  - Attempted unsigned OTA: REJECTED
  
- **Replay Attack Prevention**: ✅ PASSED
  - Nonce + timestamp verification
  - Commands >5 minutes old: REJECTED
  - Replayed command: REJECTED (nonce cache hit)
  
- **Rate Limiting**: ✅ PASSED
  - 10 commands/minute per device enforced
  - 11th command within 1 minute: REJECTED (429 error)

## 2. Backend Security

### 2.1 API Security
- **Authentication**: ✅ PASSED
  - Firebase Auth with JWT tokens
  - Token expiration: 1 hour (enforced)
  - Refresh token rotation: enabled
  
- **Authorization**: ✅ PASSED
  - Users can only access their own devices
  - Attempted unauthorized access: BLOCKED (403 error)
  
- **Input Validation**: ✅ PASSED
  - All inputs sanitized
  - Attempted SQL injection: N/A (Firestore NoSQL)
  - Attempted XSS: BLOCKED (input escaping)
  
- **CORS**: ✅ PASSED
  - Whitelist only: app.smartplugai.com, dashboard.smartplugai.com
  - Attempted cross-origin request from malicious-site.com: BLOCKED

### 2.2 Data Security
- **Encryption at Rest**: ✅ PASSED
  - Firestore encryption enabled (AES-256)
  - Sensitive fields (passwords, keys) hashed with bcrypt
  
- **Encryption in Transit**: ✅ PASSED
  - TLS 1.3 enforced on all endpoints
  - TLS 1.0, 1.1, 1.2 downgrade: REJECTED
  - Certificate pinning in mobile app: ENABLED

### 2.3 Secrets Management
- **Secret Storage**: ✅ PASSED
  - No secrets in source code (verified with truffleHog)
  - Secrets stored in Google Secret Manager
  - Access controlled by IAM roles

## 3. Network Security

### 3.1 MQTT Broker
- **TLS**: ✅ PASSED
  - TLS 1.3 enforced
  - Mutual TLS (mTLS) enabled
  - Client certificate verification: REQUIRED
  
- **Access Control**: ✅ PASSED
  - Per-device ACLs configured
  - Device can only pub/sub to own topics
  - Attempted cross-device access: BLOCKED

### 3.2 Firewall Rules
- **Ingress Rules**: ✅ PASSED
  - Only ports 443 (HTTPS) and 8883 (MQTTS) open
  - All other ports: CLOSED
  
- **Egress Rules**: ✅ PASSED
  - Firestore, Cloud Functions can only access GCP services
  - No outbound internet access (except NTP)

## 4. Identified Issues & Remediation

### Issue 1: Missing HSTS Header (MEDIUM)
**Description**: Web dashboard did not set Strict-Transport-Security header.  
**Impact**: Potential SSL stripping attack.  
**Remediation**: Added HSTS header with max-age=31536000 (1 year).  
**Status**: ✅ FIXED

### Issue 2: Verbose Error Messages (MEDIUM)
**Description**: API returned stack traces in error responses.  
**Impact**: Information disclosure (internal paths, library versions).  
**Remediation**: Sanitized error messages, log details server-side only.  
**Status**: ✅ FIXED

## 5. Compliance Status

- **POPIA (South Africa)**: ⚠️ PARTIAL
  - User data minimization: ✅
  - Consent collection: ✅
  - Data breach notification procedures: ✅
  - Data portability: ⏳ TODO (Phase 2)
  
- **SANS 10142-1 (Electrical Safety)**: ⏳ IN PROGRESS
  - Compliance testing scheduled for Phase 2
  - Pre-audit review: PASSED
  
- **ICASA Type Approval**: ⏳ NOT STARTED
  - Scheduled for Phase 2 (Month 7-8)

## 6. Recommendations

### Short-Term (Phase 1)
1. ✅ Implement web application firewall (WAF) - COMPLETED
2. ✅ Enable DDoS protection (Cloud Armor) - COMPLETED
3. ⏳ Conduct external penetration test - SCHEDULED (Phase 2)

### Medium-Term (Phase 2)
1. Achieve SABS certification
2. Complete POPIA compliance (data portability)
3. Implement SIEM for security event correlation

### Long-Term (Phase 3)
1. SOC2 Type II certification
2. ISO 27001 certification
3. Quantum-resistant cryptography research

## 7. Testing Evidence

All test results, scripts, and logs are available in:
- `/docs/security_tests/` (internal repository)
- Penetration test report: `pentest_report_2024-12.pdf`
- Code review findings: `code_review_2024-12.xlsx`

## 8. Sign-Off

**Reviewed By**:
- Security Lead: [Name], [Date]
- CTO: [Name], [Date]
- External Auditor: [Name], [Date] (if applicable)

**Approved For Production**: ✅ YES

**Next Review Date**: March 2025 (Quarterly)
```

##### **Calibration Certificates Generation**

**Objective**: Provide proof of accuracy for energy measurements (required for commercial products).

**Calibration Process**:
```text
Device ID: device-001
Serial Number: SPA-2024-001
Calibration Date: 2024-12-01

CALIBRATION PROCEDURE:

Equipment Used:
- Reference Power Meter: Fluke 345 (±0.1% accuracy)
- Precision Load: Resistive load bank (100W, 200W, 500W)
- Multimeter: Fluke 87V

Test Conditions:
- Ambient Temperature: 22°C
- Humidity: 45% RH
- Voltage: 230V AC ±1%

Calibration Points:
| Load (W) | Reference (W) | Device Reading (W) | Error (%) |
|----------|---------------|--------------------|-----------|
| 100.0    | 100.2         | 100.4              | +0.2%     |
| 200.0    | 200.1         | 199.8              | -0.15%    |
| 500.0    | 500.3         | 500.8              | +0.1%     |

Voltage Calibration:
- Reference: 230.0V
- Device: 230.2V
- Error: +0.09%

Current Calibration:
- Reference: 2.17A (at 500W)
- Device: 2.18A
- Error: +0.46%

RESULT: ✅ PASSED
All measurements within ±1% accuracy specification.

Calibration Adjustments Applied:
- Current sensor offset: +0.02A
- Voltage multiplier: 1.003
- Power factor correction: 0.98

Calibrated By: [Technician Name]
Verified By: [QA Lead Name]
Certificate Number: CAL-2024-001
Valid Until: 2025-12-01 (12 months)
```

**Certificate Template** (PDF):
```text
┌────────────────────────────────────────────────────┐
│        SMART PLUG AI - CALIBRATION CERTIFICATE      │
├────────────────────────────────────────────────────┤
│                                                     │
│  Device Information                                │
│  Model: Smart Plug AI v1.0                         │
│  Serial Number: SPA-2024-001                       │
│  Device ID: device-001                             │
│  Manufacturing Date: 2024-11-25                    │
│                                                     │
│  Calibration Summary                               │
│  Calibration Date: 2024-12-01                      │
│  Certificate Number: CAL-2024-001                  │
│  Calibrated By: John Smith (Technician #123)       │
│  Verified By: Jane Doe (QA Lead)                   │
│  Next Calibration: 2025-12-01                      │
│                                                     │
│  Accuracy Specifications                           │
│  Power Measurement: ±1.0% (±0.2% achieved)         │
│  Voltage Measurement: ±1.0% (±0.09% achieved)      │
│  Current Measurement: ±2.0% (±0.46% achieved)      │
│                                                     │
│  Calibration Standard                              │
│  Reference: Fluke 345 Power Meter                  │
│  Traceability: NIST-traceable (Cert #NIST-2024)    │
│                                                     │
│  [QR Code: Verification URL]                       │
│  Verify at: smartplugai.com/verify/CAL-2024-001   │
│                                                     │
│  [Company Seal]              [Signature]           │
│                                                     │
└────────────────────────────────────────────────────┘
```

##### **Setup Guides and Troubleshooting Documentation**

**File**: `/docs/TROUBLESHOOTING.md`

```markdown
# Troubleshooting Guide

## Common Issues

### Issue: Device Not Pairing

**Symptoms**:
- LED blinking blue continuously
- App says "Device not found"

**Possible Causes**:
1. WiFi password incorrect
2. Device too far from router
3. 5GHz WiFi selected (not supported)

**Solutions**:
1. Double-check WiFi password (case-sensitive)
2. Move device closer to router (within 10 meters)
3. Ensure 2.4GHz WiFi network selected
4. Restart device: Unplug for 10 seconds, plug back in
5. Factory reset: Hold reset button 10 seconds until LED turns red

### Issue: Device Offline

**Symptoms**:
- Device shows "Offline" in app
- Cannot control device

**Possible Causes**:
1. WiFi connection lost
2. Power outage
3. Device firmware crashed

**Solutions**:
1. Check router status (other devices working?)
2. Check device LED: Should be green (online) or yellow (reconnecting)
3. Restart device: Unplug for 10 seconds, plug back in
4. Check for firmware update in app settings
5. Contact support if issue persists

### Issue: Inaccurate Power Readings

**Symptoms**:
- Power reading too high or too low
- Energy calculations incorrect

**Possible Causes**:
1. Calibration drift
2. Non-resistive load (inductive/capacitive)
3. Sensor malfunction

**Solutions**:
1. Compare with known load (e.g., 100W bulb)
2. Recalibrate device (Settings → Calibration)
3. Avoid using with motors or transformers (inductive loads)
4. Contact support for re-calibration certificate

## Error Codes

| Code | Description | Solution |
|------|-------------|----------|
| E001 | WiFi connection failed | Check WiFi password, restart router |
| E002 | Attestation failed | Contact support (security issue) |
| E003 | Over-temperature | Unplug device immediately, check for overload |
| E004 | Over-current | Reduce load below 500W |
| E005 | Relay malfunction | Contact support for replacement |

## Advanced Diagnostics

### Viewing Device Logs

1. Open app → Device Settings → Diagnostics
2. Tap "Download Logs"
3. Share logs with support: support@smartplugai.com

### Factory Reset

⚠️ Warning: This will erase all device settings and unpair from your account.

1. Unplug device
2. Hold reset button
3. While holding, plug device back in
4. Keep holding for 10 seconds
5. LED will blink red 3 times, then turn blue (pairing mode)
6. Release button
7. Re-pair device in app

## Contact Support

**24/7 Support Hotline**: +27-82-123-4567  
**Email**: support@smartplugai.com  
**WhatsApp**: +27-82-123-4567  
**Web**: www.smartplugai.com/support

**Business Hours Support**: Mon-Fri 8AM-6PM SAST  
**After-Hours**: Emergency support for safety issues only
```

##### **Feedback Collection System**

**Objective**: Systematically gather user feedback to inform Phase 2 development.

**Feedback Channels**:

**1. In-App Feedback**:
- "Send Feedback" button in app menu
- Automatically includes: device ID, app version, user ID, logs
- Optional: Screenshot attachment, rating (1-5 stars)

**2. Weekly Survey** (via email):
```text
Subject: How was your Smart Plug experience this week?

Hi [User Name],

Thanks for being a pilot tester! We'd love to hear about your experience this week.

Please take 2 minutes to answer these questions:

1. How satisfied are you with the Smart Plug overall? ⭐⭐⭐⭐⭐

2. Did you experience any issues this week? Yes / No
   If yes, please describe:

3. What feature did you use most?
   [ ] Power control
   [ ] Schedules
   [ ] Energy monitoring
   [ ] Alerts

4. What feature would you like to see next?

5. Would you recommend this product to a friend? Yes / No

[Submit Feedback Button]

Your feedback helps us improve. Thank you!
```

**3. Monthly Video Interview**:
- 30-minute video call with product manager
- Deep dive into user experience
- Compensation: R100 voucher

**Feedback Analysis**:
- Aggregate feedback in spreadsheet
- Identify top 5 pain points
- Prioritize feature requests
- Track NPS (Net Promoter Score)
- Monthly report to team

##### **Testing Criteria**

**Deployment Testing**:
- [ ] All 10-20 devices successfully deployed
- [ ] All users able to pair and control devices
- [ ] 95%+ devices online 24/7
- [ ] Monitoring dashboard shows all devices healthy
- [ ] Zero SEV-1 incidents in first week

**Documentation Testing**:
- [ ] User manual reviewed by non-technical tester (comprehension check)
- [ ] Quick Start Guide enables setup in <5 minutes
- [ ] Troubleshooting guide resolves 80%+ support tickets
- [ ] Developer documentation enables new engineer to understand system

##### **Expected Challenges and Solutions**

| Challenge | Solution |
|-----------|----------|
| Users losing WiFi passwords | Pre-configure devices with guest networks, provide password reset flow |
| Devices going offline unexpectedly | Implement auto-reconnect with exponential backoff |
| Support hotline overwhelmed | Create self-service help center, chatbot for common issues |
| Feedback collection low response rate | Gamify feedback (R50 voucher per survey), keep surveys short |
| Documentation too technical for average users | Simplify language, add more diagrams, video tutorials |
| Incident response too slow | Automate alerts, create runbooks for common scenarios |

##### **Deliverable Breakdown**

**Deployment Deliverables**:
- 10-20 devices deployed to pilot users
- 24/7 monitoring dashboard (Grafana)
- Incident response procedures documented
- On-call rotation schedule established

**User Documentation Deliverables**:
- Quick Start Guide (PDF + printed)
- Complete User Manual (20-page PDF)
- Troubleshooting Guide (online + PDF)
- Video tutorials (device setup, app walkthrough)

**Developer Documentation Deliverables**:
- Expanded API documentation
- Architecture diagrams updated
- Deployment runbooks
- Security audit report (Appendix F)

**Calibration & Compliance Deliverables**:
- Calibration certificates for all 10-20 devices
- Electrical safety test reports
- Compliance checklist (POPIA, SANS)

**Feedback System Deliverables**:
- In-app feedback mechanism
- Weekly survey template
- Feedback analysis dashboard
- Monthly product improvement report

**Success Metric**: All pilot devices operational, users satisfied (>4/5 rating), zero critical incidents, comprehensive documentation enabling Phase 2 scale-up.

---

## 📅 Phase 2: Pilot & Refinement (Months 5-8)

**Goal**: Expand pilot to 500 devices with security monitoring, achieve SABS/ICASA compliance, and complete first production batch.

**Budget**: R3,000,000
- Manufacturing (5,000 secure units): R1,500,000
- Certifications (SABS, ICASA, security audits): R600,000
- Cloud infrastructure scaling (TLS, CDN, monitoring): R500,000
- Security team & tools: R300,000
- Marketing & pilot program: R100,000

### Month 4: Secure Manufacturing Prep
- [ ] PCB v2 design with security zones and improved layout
- [ ] Component sourcing with chain of custody verification
- [ ] Safety certification testing (SABS compliance)
- [ ] Independent security audit (penetration testing)
- [ ] Secure packaging (tamper-evident seals and documentation)
- [ ] Secure assembly process documentation
- [ ] Quality assurance testing (100% security validation)

### Month 5: Secure Software V2.0
- [ ] Energy disaggregation v1 (on-device, encrypted)
- [ ] Daily/weekly energy reports (encrypted in transit and at rest)
- [ ] Cost tracking with PCI DSS compliance considerations
- [ ] Tariff integration (prepaid vs postpaid with secure billing)
- [ ] Export reports with digital signatures (PDF/CSV)
- [ ] Enhanced device attestation (firmware integrity checks)
- [ ] Secure OTA updates with code signing (RSA-3072)

### Month 6: Secure Pilot Expansion
- [ ] 500-device pilot program with security monitoring
- [ ] Customer support system (Zendesk with SSO)
- [ ] Security Operations Center (SOC) dashboard
- [ ] Automated security scanning (CodeQL, dependency checks)
- [ ] Mobile app v2 (app store ready with security review)
- [ ] Bug bounty program launch (R5k-R50k rewards)

### Month 7-8: Certification & Hardening
- [ ] SABS certification completion
- [ ] ICASA type approval submission
- [ ] First penetration testing results and remediation
- [ ] Field-level encryption implementation
- [ ] Rate limiting and DDoS protection
- [ ] Compliance documentation (POPIA readiness)
- [ ] Security incident response procedures

**Milestone**: 500 devices deployed, SABS/ICASA certified, security-hardened platform

---

## 📅 Phase 3: Commercial Launch (Months 9-12)

**Goal**: Scale to 10,000+ devices with nationwide retail launch, SOC2 Type I audit, and enterprise features.

**Budget**: R6,000,000
- Inventory (50,000 units): R3,000,000
- Retail partnerships & marketing: R1,500,000
- SOC2 Type I preparation & audit: R500,000
- Team expansion (20 people including security): R800,000
- Infrastructure & scaling: R200,000

### Month 9: Retail Launch Preparation
- [ ] Retail partnerships (Takealot, Builders, Makro)
- [ ] Marketing campaign with security messaging
- [ ] Enterprise sales team
- [ ] SOC2 Type I controls implementation
- [ ] 24/7 security monitoring
- [ ] Advanced threat detection

### Month 10: Nationwide Launch
- [ ] Public launch event
- [ ] Social media campaign
- [ ] Press releases emphasizing security
- [ ] First 10,000 units shipped
- [ ] Customer onboarding with security training
- [ ] Retail presence in major cities

### Month 11: Enterprise Features
- [ ] Multi-tenant architecture
- [ ] Advanced RBAC (custom roles)
- [ ] Enterprise API with rate limiting
- [ ] White-label options
- [ ] Integration with building management systems
- [ ] Compliance reports for enterprises

### Month 12: SOC2 & Scale
- [ ] SOC2 Type I audit completion
- [ ] Security certification showcase
- [ ] Scale to 50,000 devices capacity
- [ ] International expansion planning (GDPR compliance)
- [ ] Advanced analytics and AI features
- [ ] Quantum-resistant cryptography research

**Milestone**: Nationwide presence, 10,000+ active devices, SOC2 Type I certified, R1.5M+ monthly revenue

---

## 🔮 Future Vision (Months 13-36)

### Phase 4: Enterprise Expansion (Months 13-18)
- **SOC2 Type II certification**
- **ISO 27001 certification**
- **Zero-trust architecture implementation**
- **Multi-region deployment** (3+ African countries)
- **Advanced AI-powered threat detection**
- **100,000+ active devices**

### Phase 5: Regional Expansion (Months 19-24)
- **GDPR compliance** for European markets
- **Supply chain security** (blockchain audit trails)
- **Partnerships with utility companies**
- **Virtual power plant participation**
- **1,000,000+ devices globally**

### Phase 6: Technology Leadership (Months 25-36)
- **Quantum-resistant cryptography** implementation
- **Industry security standards** contribution
- **Open-source security framework**
- **AI appliance recognition** by power signature
- **Grid services** and demand response
- **Global market leadership**

---

## 🎪 Key Success Metrics

### Technical Metrics
- **Uptime**: 99.9%+ for cloud services (with 24/7 security monitoring)
- **Latency**: <200ms for real-time data updates over TLS
- **Accuracy**: ±1% for power measurements (calibrated with ATECC608A)
- **Security**: Zero security breaches, 100% device attestation pass rate
- **Encryption**: 100% of sensitive data encrypted in transit and at rest

### Product Metrics
- **User retention**: 80%+ after 30 days (trust through security)
- **Daily active users**: 70%+ of registered users
- **App rating**: 4.7+ stars (security as differentiator)
- **NPS (Net Promoter Score)**: 60+ (security trust factor)
- **Device attestation**: 99.9%+ devices passing integrity checks

### Development Metrics
- **Team velocity**: 3-4 developers working in parallel
- **Sprint cycle**: 1-week sprints with daily standups
- **Code review**: All PRs require 1+ approvals
- **Pair programming**: Hardware-Software teams collaborate on integration
- **Code coverage**: 85%+ for critical security paths
- **Deployment frequency**: 3+ per week (with security scans)
- **Mean time to recovery**: <30 minutes for critical security issues
- **Bug resolution time**: <24 hours for security vulnerabilities
- **Security scan**: 100% of deployments scanned with CodeQL

### Security Metrics (New)
- **Penetration test results**: 0 critical vulnerabilities
- **Security audit score**: 95%+ compliance
- **Incident response time**: <15 minutes detection, <1 hour mitigation
- **Device tamper detection**: 100% alert success rate
- **Certificate management**: 0 expired certificates
- **2FA adoption**: 90%+ of users enabled

### Business Metrics (New)
- **Security premium**: 10-15% higher pricing vs non-secure competitors
- **Enterprise adoption**: 50+ corporate clients (SOC2 certified)
- **Compliance certifications**: SABS, ICASA, SOC2 Type II, ISO 27001
- **Churn rate**: <5% monthly (vs 15% industry average)
- **Customer acquisition cost**: 30% lower due to security reputation

---

## 📝 Notes

- **Team Structure**: 3-4 developers (2 software, 1-2 hardware)
- **Parallel Development**: Hardware and software teams work concurrently
- **Leveraged Pre-Work**: Mobile app (Flutter), web dashboard (React), and backend structure already established in `/app/`, `/web/`, and `/backend/`
- **Accelerated Timeline**: Week 1 detailed guide (`docs/WEEK1_HARDWARE_MASTERY.md`) enables rapid hardware onboarding
- **Security-First**: All phases prioritize security over feature velocity
- **Compliance-Driven**: SABS, ICASA, SOC2, ISO 27001 certifications guide development
- **User Trust**: Security is the primary differentiator and value proposition
- **Transparency**: Security audit results shared with users (Appendix F)
- **Continuous Improvement**: Weekly security reviews, quarterly penetration tests
- **Community Feedback**: Bug bounty program rewards responsible disclosure
- **Roadmap Flexibility**: Dates may adjust based on security findings and resource availability
- **Documentation**: All security decisions documented for audit trail
- **Budget Allocation**: 20-30% of budget allocated to security in all phases

**Cross-References**:
- `docs/WEEK1_HARDWARE_MASTERY.md` - Comprehensive Week 1 hardware guide
- `docs/WEEK1_QUICK_REFERENCE.md` - Quick reference for hardware setup
- `/app/SETUP.md` and `/app/README.md` - Flutter app setup
- `/web/SETUP.md` and `/web/README.md` - React dashboard setup
- `/backend/README.md` - Backend API setup
- `PRE_HARDWARE_COMPLETE.md` - Pre-hardware work completed

**Security Contacts**:
- **Security Team**: security@smartplugai.com
- **Vulnerability Disclosure**: security@smartplugai.com (PGP key available)
- **Bug Bounty**: R5,000 - R50,000 rewards for critical vulnerabilities

**Last Updated**: December 2024  
**Version**: 2.0 (Security-Enhanced)  
**Next Review**: Monthly during Phase 1 (through April 2025), Quarterly thereafter
