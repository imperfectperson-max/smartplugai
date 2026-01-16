# Activity Diagrams

This folder contains UML Activity diagrams showing workflows and process flows for the Smart Plug AI system.

## Expected Files

The following diagram files will be added to this directory:

- **2.1_Device_Pairing_Flow.vpp** - Secure device pairing process with QR code and challenge-response authentication
- **2.2_Secure_Command_Flow.vpp** - Signed command execution flow with ECDSA verification
- **2.3_Tamper_Response_Flow.vpp** - Tamper detection and incident response workflow

## Diagram Contents

These diagrams will detail:

### Device Pairing Flow
- Device initialization with ATECC608A key generation
- QR code scanning and data extraction
- Challenge-response authentication
- Device attestation and certificate exchange
- Backend registration and finalization

### Secure Command Flow
- User command initiation
- JWT authentication and RBAC checks
- Command signing with ECDSA P256
- MQTT transmission over TLS 1.3
- Device-side signature verification with ATECC608A
- Command execution and status updates

### Tamper Response Flow
- MAX6316 watchdog tamper detection
- Immediate hardware response (100ms)
- Secure boot verification
- Network alerting via MQTT
- User notification (push, email, dashboard)
- Security review and device recovery

## Tools

- **Primary Tool**: Visual Paradigm Community Edition
- **Backup Tool**: UMLet

## Security Features

All activity diagrams incorporate security checkpoints including:
- Hardware-based cryptographic operations
- Multi-factor authentication
- End-to-end encryption
- Certificate-based mutual authentication
- Comprehensive audit logging
