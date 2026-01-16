# Sequence Diagrams

This folder contains UML Sequence diagrams showing interaction flows between system components.

## Expected Files

The following diagram files will be added to this directory:

- **4.1_Secure_Control_Sequence.vpp** - Complete sequence for secure device control with ECDSA signatures
- **4.2_Tamper_Detection_Sequence.vpp** - Tamper detection and response sequence from hardware to user notification
- **4.3_OTA_Update_Sequence.vpp** - Secure over-the-air firmware update process with attestation

## Diagram Contents

### Secure Control Sequence
Participants: User → Mobile App → Backend API → Auth Service → Database → MQTT Broker → IoT Device → ATECC608A → Relay Module

Key interactions:
- JWT authentication and RBAC authorization
- Command signing with server ECDSA private key
- MQTT transmission over TLS 1.3 with client certificates
- Device-side signature verification using ATECC608A
- Relay control execution
- Real-time status updates via WebSocket

### Tamper Detection Sequence
Participants: MAX6316 Watchdog → ESP32-S3 → ATECC608A → EEPROM → MQTT Broker → Backend → Database → Notification Service → Mobile App → User → Security Admin

Key interactions:
- Hardware-level tamper detection (100ms response)
- Secure boot verification
- Encrypted event logging
- Multi-channel notifications (push, email, dashboard)
- Security review and device recovery
- Challenge-response re-verification

### OTA Update Sequence
Participants: Device → MQTT Broker → Backend → Database → CDN/Storage → HSM → Bootloader → Admin → Mobile App

Key interactions:
- Update availability check with version comparison
- Manifest signing with RSA-3072
- Firmware download over HTTPS with certificate pinning
- SHA-256 integrity verification
- Secure Boot V2 firmware signature check
- Device attestation post-update
- Rollback protection and health monitoring

## Tools

- **Primary Tool**: Visual Paradigm Community Edition (best sequence diagram generation)
- **Backup Tool**: UMLet

## Security Annotations

All sequence diagrams include security annotations for:
- Encryption algorithms (AES-256-GCM, ECDSA P256, RSA-3072)
- Authentication mechanisms (JWT, mTLS, challenge-response)
- Transport security (TLS 1.3, certificate pinning)
- Hardware security (ATECC608A, MAX6316, secure boot)
- Anti-replay mechanisms (nonces, timestamps)
