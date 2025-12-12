# Smart Plug AI - Security Architecture

This document outlines the comprehensive security architecture for the Smart Plug AI project, detailing our security-first approach to IoT device design, cloud infrastructure, and data protection.

## 🛡️ Security Overview

Smart Plug AI implements a **defense-in-depth** security architecture with multiple layers of protection across hardware, firmware, communication, and cloud infrastructure.

### Core Security Principles

1. **Zero Trust Architecture**: Never trust, always verify
2. **Defense in Depth**: Multiple layers of security controls
3. **Least Privilege**: Minimal access rights for users and devices
4. **Secure by Design**: Security built-in from the ground up
5. **Privacy by Default**: User data encrypted and protected
6. **Compliance Ready**: Designed for SABS, ICASA, and SOC2 certification

---

## 🔐 Hardware Security

### ESP32-S3 Security Features

The ESP32-S3-WROOM-1-N8 provides hardware-based security:

- **Secure Boot V2 (RSA-PSS)**: Ensures only signed firmware can run
- **Flash Encryption (AES-256)**: Protects firmware and data in flash memory
- **Hardware Cryptographic Accelerators**: RSA, AES, SHA, ECC operations
- **eFuse Protection**: One-time programmable keys stored securely
- **Random Number Generator**: True hardware RNG for cryptographic operations
- **Memory Protection**: MPU (Memory Protection Unit) for secure memory regions

### ATECC608A Secure Element

Hardware cryptographic co-processor for secure key storage:

- **ECDSA P256 Key Storage**: Private keys never leave the chip
- **Secure Key Generation**: On-chip key generation
- **Hardware SHA-256**: Fast cryptographic hashing
- **Monotonic Counters**: Prevent replay attacks
- **Secure Configuration Lock**: Irreversible security configuration
- **Physical Tamper Resistance**: Protected against side-channel attacks

**Key Slots Configuration**:
- Slot 0: Device private key (ECDSA P256) - never readable
- Slot 1: Server public key for command verification
- Slot 2: Backup/Recovery key
- Slots 3-15: Reserved for future use

### Tamper Detection

- **MAX6316 Watchdog**: Monitors for physical tampering
- **Tamper-Evident Seals**: Physical security indicators on enclosure
- **Secure Erase on Tamper**: Automatic key deletion on detection
- **Battery-Backed Detection**: Tamper monitoring even when unpowered

### Secure Storage

- **24C256 EEPROM (32KB)**: Encrypted configuration storage
- **NVS Encryption**: ESP32-S3 Non-Volatile Storage encryption
- **Secure Credential Storage**: Wi-Fi credentials encrypted in NVS

---

## 🔒 Firmware Security

### Secure Boot Process

1. **ROM Bootloader**: Verifies second-stage bootloader signature
2. **Second-Stage Bootloader**: Verifies application firmware signature
3. **Application Firmware**: Runs only if signature is valid
4. **Rollback Protection**: Prevents downgrade to vulnerable versions

**Signing Keys**:
- Development: Test signing key for prototypes
- Production: Hardware Security Module (HSM) protected key

### Flash Encryption

- **AES-256 Encryption**: All flash contents encrypted
- **Unique Per-Device Keys**: Keys generated in eFuse on first boot
- **Transparent Decryption**: Automatic decryption by hardware
- **Protected Key Storage**: Encryption key never readable

### Over-The-Air (OTA) Updates

- **Signed Firmware**: All updates signed with RSA-3072 or ECDSA P256
- **Secure Download**: HTTPS/TLS 1.3 for firmware download
- **Signature Verification**: Verified before flashing
- **Rollback on Failure**: Automatic rollback if update fails
- **Version Validation**: Anti-rollback protection

---

## 🌐 Communication Security

### MQTT over TLS 1.3

All device-to-cloud communication uses encrypted MQTT:

- **TLS 1.3**: Latest TLS protocol with perfect forward secrecy
- **Client Certificates**: Mutual TLS authentication (mTLS)
- **Certificate Pinning**: Prevents man-in-the-middle attacks
- **Strong Cipher Suites**: TLS_AES_256_GCM_SHA384, TLS_CHACHA20_POLY1305_SHA256
- **Certificate Rotation**: Automated certificate renewal

**MQTT Configuration**:
- Port: 8883 (TLS)
- Protocol: MQTT 5.0
- QoS: 1 (at least once delivery) for commands
- Retained Messages: Status updates only

### Signed Commands

All control commands are cryptographically signed:

1. **Command Generation**: Backend signs command with server private key (ECDSA)
2. **Command Transmission**: Sent over MQTT/TLS
3. **Signature Verification**: Device verifies using server public key in ATECC608A
4. **Execution**: Command executed only if signature is valid
5. **Audit Logging**: All commands logged with timestamp and user

**Command Format**:
```json
{
  "command": "relay_on",
  "timestamp": 1234567890,
  "nonce": "random-unique-value",
  "signature": "base64-encoded-ecdsa-signature"
}
```

### Encrypted Telemetry

Sensitive telemetry data is encrypted:

- **End-to-End Encryption**: AES-256-GCM for sensitive fields
- **Field-Level Encryption**: Only sensitive data encrypted (power, usage patterns)
- **Key Management**: Per-device encryption keys stored in ATECC608A
- **Integrity Protection**: HMAC-SHA256 for data integrity

---

## 🔐 Device Provisioning & Pairing

### Secure Provisioning Workflow

1. **Factory Provisioning**:
   - Generate unique device ID
   - Provision ATECC608A with device private key
   - Generate device certificate signed by Device CA
   - Lock ATECC608A configuration (irreversible)
   - Enable secure boot and flash encryption
   - Write device certificate and CA certificate to flash

2. **First Boot**:
   - Verify secure boot signatures
   - Initialize flash encryption (generates unique key)
   - Load device certificate from ATECC608A
   - Enter Wi-Fi provisioning mode (AP mode with WPA2)

3. **Wi-Fi Provisioning**:
   - User connects to device AP
   - Provide Wi-Fi credentials via secure web interface (HTTPS)
   - Credentials encrypted and stored in NVS
   - Device connects to Wi-Fi network

### QR-Based Secure Pairing

Challenge-response authentication prevents unauthorized pairing:

1. **QR Code Generation**:
   - Device displays QR code with device ID and challenge
   - Challenge is time-limited (5 minutes) and single-use

2. **Mobile App Scan**:
   - App scans QR code
   - Extracts device ID and challenge

3. **Challenge-Response**:
   - App sends challenge to backend with user credentials
   - Backend generates response using device's public key
   - App sends response to device

4. **Device Verification**:
   - Device verifies response using its private key in ATECC608A
   - If valid, device accepts pairing and registers with backend
   - Generates session token for future communication

5. **Backend Registration**:
   - Device sends attestation data (certificate, firmware version)
   - Backend verifies device authenticity
   - Associates device with user account

---

## ☁️ Cloud Security

### Backend Security Architecture

#### Authentication & Authorization

- **Auth0/Firebase Auth**: Industry-standard identity providers
- **OAuth2/OpenID Connect**: Modern authentication protocols
- **2FA/MFA Support**: Time-based OTP (TOTP) or SMS
- **Role-Based Access Control (RBAC)**:
  - **Admin**: Full system access
  - **User**: Device management and monitoring
  - **Viewer**: Read-only access
  - **Service**: Backend service accounts

#### API Security

- **FastAPI Framework**: Built-in security features
- **HTTPS/TLS 1.3**: All API communication encrypted
- **JWT Tokens**: Short-lived access tokens (30 minutes)
- **Refresh Tokens**: Secure refresh mechanism (7 days)
- **Rate Limiting**: Prevent brute force and DoS attacks
  - Authentication: 5 requests/minute
  - API calls: 60 requests/minute per user
  - Device commands: 10 requests/minute per device
- **Input Validation**: Pydantic schemas for all endpoints
- **CORS**: Strict origin policy
- **CSRF Protection**: Token-based protection for state-changing operations

#### Data Encryption

**Field-Level Encryption**:
- Firestore/PostgreSQL: Sensitive fields encrypted before storage
- Encryption Algorithm: AES-256-GCM
- Key Management: Google Cloud KMS or AWS KMS
- Key Rotation: Automatic quarterly rotation

**Encryption at Rest**:
- Database: Native encryption (Firestore, PostgreSQL)
- Backups: Encrypted with separate keys
- Logs: Encrypted in storage

**Encryption in Transit**:
- TLS 1.3 for all connections
- Certificate pinning for mobile apps
- Perfect forward secrecy

#### Audit Logging

Comprehensive logging of security-relevant events:

- **Authentication Events**: Login, logout, 2FA, password changes
- **Authorization Events**: Permission changes, role assignments
- **Device Events**: Pairing, unpairing, commands, tamper alerts
- **Data Access**: Telemetry queries, exports
- **Administrative Actions**: Configuration changes, user management
- **Security Events**: Failed authentication, invalid signatures, tamper detection

**Log Format** (JSON):
```json
{
  "timestamp": "2024-01-15T10:30:00Z",
  "event_type": "device_command",
  "user_id": "user_123",
  "device_id": "smartplug_001",
  "action": "relay_on",
  "result": "success",
  "ip_address": "192.168.1.100",
  "user_agent": "SmartPlugApp/1.0"
}
```

**Log Retention**:
- Security events: 2 years
- Audit logs: 1 year
- Access logs: 90 days
- Application logs: 30 days

---

## 🔍 Device Attestation

Device attestation ensures firmware integrity:

1. **Boot-Time Attestation**:
   - Device generates attestation report
   - Includes firmware version, hash, secure boot status
   - Signed by device private key in ATECC608A

2. **Periodic Attestation**:
   - Devices attest every 24 hours
   - Backend verifies attestation signature
   - Flags devices with mismatched firmware

3. **Attestation Failure**:
   - Device marked as "unverified" in backend
   - User notified via push notification
   - Device commands disabled until verified
   - Manual investigation required

**Attestation Report Format**:
```json
{
  "device_id": "smartplug_001",
  "timestamp": 1234567890,
  "firmware_version": "1.0.0",
  "firmware_hash": "sha256-hash",
  "secure_boot_enabled": true,
  "flash_encryption_enabled": true,
  "attestation_signature": "ecdsa-signature"
}
```

---

## 🚨 Tamper Detection & Response

### Tamper Detection Mechanisms

1. **Physical Tamper**:
   - MAX6316 watchdog monitors enclosure
   - Tamper-evident seals on enclosure screws
   - Detection triggers immediate alert

2. **Firmware Tamper**:
   - Secure boot prevents modified firmware
   - Flash encryption protects stored data
   - Attestation detects compromised devices

3. **Communication Tamper**:
   - TLS prevents eavesdropping
   - Signed commands prevent injection
   - Certificate pinning prevents MITM

### Tamper Response Actions

1. **Immediate Actions**:
   - Send tamper alert to backend
   - Notify user via push notification
   - Log event with timestamp and details
   - Disable device commands (safety mode)

2. **Secure Erase** (if configured):
   - Erase sensitive data from NVS
   - Erase ATECC608A keys (if possible)
   - Require re-provisioning

3. **Manual Recovery**:
   - User verifies physical device integrity
   - Re-pair device with challenge-response
   - Backend re-enables device

---

## 📜 Compliance & Certifications

### South African Compliance

#### SABS (South African Bureau of Standards)

- **SANS 164**: Plugs and socket-outlets
- **SANS 60950**: Safety of IT equipment
- **SANS 60335**: Safety of household appliances
- **Compliance Roadmap**: Phase 2 (production devices)

#### ICASA (Independent Communications Authority)

- **Type Approval**: Required for RF devices
- **Frequency Allocation**: 2.4 GHz Wi-Fi band
- **EMC Testing**: Electromagnetic compatibility
- **Compliance Roadmap**: Phase 2 (before market release)

### International Standards

- **IEC 62443**: Industrial cybersecurity standard
- **ETSI EN 303 645**: Cybersecurity for consumer IoT
- **ISO 27001**: Information security management (backend)

### SOC2 Compliance

**Type II Controls** (planned):
- **Security**: Access controls, encryption, monitoring
- **Availability**: Uptime SLA, disaster recovery
- **Confidentiality**: Data protection, NDA enforcement
- **Compliance Timeline**: SOC2 Type II audit in Year 2

---

## 🔧 Security Best Practices

### For Developers

- Never commit secrets to version control
- Use environment variables for credentials
- Enable all compiler security flags
- Perform regular security code reviews
- Keep dependencies updated
- Run static analysis tools (CodeQL, Bandit)
- Implement input validation everywhere
- Use parameterized queries (prevent SQL injection)
- Sanitize user inputs (prevent XSS)

### For Deployment

- Use strong passwords (minimum 16 characters)
- Enable 2FA for all admin accounts
- Rotate credentials quarterly
- Monitor audit logs daily
- Apply security patches within 7 days
- Perform quarterly penetration testing
- Backup encryption keys securely
- Document incident response procedures

### For Users

- Use strong, unique passwords
- Enable 2FA in mobile app
- Verify device pairing QR codes
- Report suspicious device behavior
- Keep firmware updated
- Check device attestation status
- Review audit logs monthly
- Secure Wi-Fi network (WPA3 preferred)

---

## 🚨 Incident Response

### Security Incident Classification

- **P0 (Critical)**: Data breach, device compromise, ransomware
- **P1 (High)**: Failed attestation, tamper detection, authentication bypass
- **P2 (Medium)**: Suspicious activity, anomalous usage patterns
- **P3 (Low)**: Failed login attempts, configuration errors

### Response Procedures

1. **Detection & Analysis**:
   - Monitor security alerts
   - Investigate anomalies
   - Determine incident severity

2. **Containment**:
   - Isolate affected devices
   - Disable compromised accounts
   - Block malicious IP addresses

3. **Eradication**:
   - Remove malware or unauthorized access
   - Patch vulnerabilities
   - Rotate compromised credentials

4. **Recovery**:
   - Restore from clean backups
   - Re-provision devices
   - Verify system integrity

5. **Post-Incident**:
   - Document incident details
   - Conduct root cause analysis
   - Update security controls
   - Notify affected users (if required)

### Security Contacts

- **Security Team**: security@smartplugai.com
- **Vulnerability Disclosure**: security@smartplugai.com (PGP key available)
- **Emergency Contact**: +27 (0)XX XXX XXXX (24/7)

---

## 🔄 Security Roadmap

### Phase 1 (Weeks 1-12): Foundation

- ✅ Design security architecture
- ✅ Select secure hardware (ESP32-S3, ATECC608A)
- [ ] Implement secure boot and flash encryption
- [ ] Provision ATECC608A secure elements
- [ ] Implement MQTT over TLS 1.3
- [ ] Deploy backend with Auth0/Firebase Auth
- [ ] Implement signed commands
- [ ] Create QR-based secure pairing

### Phase 2 (Weeks 13-26): Hardening

- [ ] Complete device attestation
- [ ] Implement field-level encryption
- [ ] Deploy rate limiting and RBAC
- [ ] Conduct first security audit
- [ ] Implement automated security scanning
- [ ] Create security documentation
- [ ] Train team on security practices

### Phase 3 (Weeks 27-52): Certification

- [ ] Begin SABS compliance testing
- [ ] Submit ICASA type approval application
- [ ] Conduct penetration testing
- [ ] Prepare for SOC2 Type I audit
- [ ] Implement additional monitoring
- [ ] Establish security incident response team
- [ ] Create user security guides

---

## 📚 Additional Resources

- [OWASP IoT Security Guidance](https://owasp.org/www-project-internet-of-things/)
- [NIST Cybersecurity Framework](https://www.nist.gov/cyberframework)
- [ESP32-S3 Security Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/security/index.html)
- [Microchip ATECC608A Datasheet](https://www.microchip.com/en-us/product/ATECC608A)
- [MQTT Security Fundamentals](https://mqtt.org/mqtt-security-fundamentals/)

---

**Last Updated**: December 2024  
**Version**: 1.0  
**Status**: Living Document - Updated Regularly

For questions or security concerns, contact: security@smartplugai.com
