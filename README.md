# Smart Plug AI

An intelligent IoT smart plug system that monitors power consumption in real-time, learns usage patterns, and provides actionable insights through mobile and web interfaces.

## 🚀 Project Overview

Smart Plug AI is South Africa's first **bank-grade security IoT platform** combining:
- **Hardware**: ESP32-S3-WROOM-1-N8 with hardware crypto acceleration, ATECC608A-TNGTLS secure element for key storage, SCT-013-030 non-invasive current sensor (30A/1V), MAX6316 tamper watchdog, 24C256 encrypted EEPROM, and certified 5V/2A phone charger
- **Mobile App**: Flutter-based cross-platform app with Firebase Auth + 2FA, real-time encrypted monitoring, QR-based secure pairing with challenge-response authentication
- **Web Dashboard**: React-based admin dashboard with OAuth2/SSO, RBAC, encrypted analytics, and security audit log viewer
- **Backend**: FastAPI with Auth0/Firebase Auth, field-level encryption (AES-256-GCM), MQTT over TLS 1.3 with client certificates, and signed commands (ECDSA)
- **Intelligence**: Pattern recognition, anomaly detection, and tamper alert system
- **Security**: Secure boot, flash encryption, device attestation, tamper detection, end-to-end encryption, SOC2 compliance readiness

**Phase 1 Budget**: R2,736 - R3,182 (3 secure prototypes with bank-grade security)

## 📋 Quick Start for Developers

### Prerequisites

**Hardware Components** (Phase 1: 3 Plugs - R2,736-R3,182 total):
- **ESP32-S3-WROOM-1-N8** (3x @ R160 each): Hardware RSA/ECC acceleration, secure boot, flash encryption
- **ATECC608A-TNGTLS** (3x @ R65 each): Hardware secure element for cryptographic key storage (ECDSA P256)
- **SCT-013-030** (3x @ R105 each): 30A/1V non-invasive current sensor with 3.5mm jack
- **MAX6316** (3x @ R40 each): Tamper detection watchdog (physical security)
- **24C256 EEPROM** (3x @ R25 each): 32KB encrypted configuration storage
- **5V/2A Phone Charger** (3x @ R50 each): Certified power supply (safer than HLK-PM01)
- **Relay Module** (3x @ R85 each): 5V, 10A+ with optocoupler isolation
- **33Ω 1W Burden Resistor** (3x @ R8 each): For SCT-013 calibration
- **Fuse + MOV** (3x @ R30/R18 each): Safety protection (3A quick-blow + 275V varistor)
- **PC817 Optocouplers** (6x @ R8 each): Extra isolation for safety
- **Professional Enclosures** (3x @ R150 each): ABS with anti-tamper screws and seals
- **SA Plugs + Sockets** (3 sets @ R90 each): Professional appearance, not salvaged
- **Basic Electronics Kit**: Breadboard, jumpers, resistors, capacitors, diodes, LEDs (shared)
- **Tools**: Multimeter (R280), soldering iron (R200), screwdrivers with security bits (R100), wire strippers (R80)

See [soldering checklist](docs/soldering-checklist.md) for complete shopping list and assembly guide.

**Software Development**:
- **Secure Provisioning**: ATECC608A provisioning tools, ESP-IDF for secure boot and flash encryption setup
- **Mobile App**: Flutter SDK 3.x+, Dart 3.x+ (Firebase Auth + 2FA integration)
- **Web Dashboard**: Node.js 18+, npm/yarn (OAuth2/SSO with Auth0)
- **Backend**: Python 3.10+, FastAPI, Auth0/Firebase Auth, PostgreSQL/Firestore with field-level encryption, Docker (for local MQTT broker with TLS 1.3)

### Getting Started

1. **Clone the repository**
   ```bash
   git clone https://github.com/imperfectperson-max/smartplugai.git
   cd smartplugai
   ```

2. **Choose your development path**:
   - 📱 **Mobile App**: See [app/README.md](app/README.md)
   - 🌐 **Web Dashboard**: See [web/README.md](web/README.md)
   - 🔧 **Firmware**: See [firmware/README.md](firmware/README.md)
   - ☁️ **Backend**: See [backend/README.md](backend/README.md)

3. **Review the roadmap**: Check [docs/ROADMAP.md](docs/ROADMAP.md) for Phase 1-3 milestones (Phase 1: Secure MVP Development, now-4 months, R200,000; Phase 2: Pilot & Refinement, months 5-8, R3M; Phase 3: Commercial Launch, months 9-12, R6M). Detailed week-by-week Phase 1 plan (weeks 1-12) included.

4. **Security documentation**: See [docs/SECURITY.md](docs/SECURITY.md) for comprehensive security architecture, device layer (ESP32-S3 + ATECC608A + MAX6316 + 24C256), MQTT over TLS 1.3, certificate management, secure boot, flash encryption, tamper detection, device attestation, and Security Roadmap (phases 1-6). Includes Appendices E (security specifications) and F (security audit results).

## 🔨 Hardware Soldering Preparation

**Next Session**: [Soldering Checklist](docs/soldering-checklist.md)

**Phase 1 Hardware Shopping List** (3 plugs with bank-grade security):
- ✅ ESP32-S3-WROOM-1-N8 (3x @ R160 each): Hardware crypto acceleration
- ✅ ATECC608A-TNGTLS (3x @ R65 each): Secure element for key storage
- ✅ SCT-013-030 (3x @ R105 each): 30A/1V current sensor with burden resistor (33Ω 1W)
- ✅ MAX6316 (3x @ R40 each): Tamper watchdog for physical security
- ✅ 24C256 EEPROM (3x @ R25 each): Encrypted configuration storage
- ✅ 5V/2A Phone Charger (3x @ R50 each): Certified power supply (safer than HLK-PM01)
- ✅ Relay Modules (3x @ R85 each): 5V, 10A+ with optocoupler isolation
- ✅ Fuse & MOV (3x): 3A quick-blow fuses + 275V 7mm varistors for safety
- ✅ PC817 Optocouplers (6x @ R8 each): Extra isolation
- ✅ Professional Enclosures (3x @ R150 each): ABS with anti-tamper screws and seals
- ✅ SA Plugs + Sockets (3 sets @ R90 each): Professional appearance
- ✅ Basic Electronics Kit (shared): Breadboard, jumpers, resistors, capacitors, diodes, LEDs

**Phase 1 Budget**: R2,736 - R3,182 (3 secure prototypes)

Before the soldering session:
- ✅ Verify complete parts list (see [soldering checklist](docs/soldering-checklist.md))
- ✅ Review safety procedures and workspace setup including fuse and MOV protection
- ✅ Test components individually with breadboard
- ✅ Plan ATECC608A provisioning and secure boot setup
- ✅ Prepare professional enclosure with tamper-evident seals
- ✅ Verify certified 5V/2A phone charger for power supply (prototype phase)

## 🤝 Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for:
- Code style and conventions
- How to submit issues and pull requests
- Development workflow and branch naming
- Testing requirements

Also review our [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md).

## 📚 Documentation

- [Roadmap](docs/ROADMAP.md) - Phase 1-3 implementation plan with week-by-week Phase 1 (weeks 1-12) security milestones, budgets (Phase 1: R200k, Phase 2: R3M, Phase 3: R6M)
- [Security Architecture](docs/SECURITY.md) - Comprehensive security design: device layer (ESP32-S3 + ATECC608A + MAX6316 + 24C256), MQTT over TLS 1.3, certificate management, secure boot, flash encryption, tamper detection, device attestation, Security Roadmap (phases 1-6), Appendices E & F
- [Soldering Checklist](docs/soldering-checklist.md) - Hardware shopping list and assembly guide with Phase 1 budget (R2,736-R3,182 for 3 plugs)
- [Architecture](docs/ARCHITECTURE.md) - Secure system stack: Presentation/Application/Data/Device layers with TLS 1.3, client certs, RBAC
- [API Documentation](docs/API.md) - REST API, WebSocket over TLS, MQTT topics with signed commands and client certificate requirements

## 🔒 Security Enhancements

This project implements a **security-first architecture** with the following key features:

- **Hardware Security**: 
  - ESP32-S3 with secure boot and flash encryption
  - ATECC608A secure element for cryptographic key storage
  - Tamper detection with MAX6316 watchdog
  - Secure storage with 24C256 EEPROM

- **Communication Security**:
  - TLS 1.3 for all network communications
  - MQTT over TLS with client certificates
  - Signed control commands with ECDSA
  - End-to-end encrypted telemetry data

- **Device Security**:
  - QR-based secure device pairing with challenge-response authentication
  - Device attestation to verify firmware integrity
  - Secure provisioning workflow for ATECC608A
  - Tamper-evident physical seals on enclosures

- **Backend Security**:
  - FastAPI with OAuth2/Auth0 authentication and 2FA
  - Field-level encryption for sensitive data in Firestore/PostgreSQL
  - Role-based access control (RBAC)
  - Rate limiting and comprehensive audit logging
  - SOC2 compliance readiness

For detailed security documentation, see [docs/SECURITY.md](docs/SECURITY.md).

## 🎯 Where to Start

**Before Hardware is Ready**:
1. **Mobile App**: Build mock UI for auth with 2FA, power monitoring with encrypted display, and secure QR pairing with challenge-response
2. **Web Dashboard**: Create placeholder dashboard with sample data and security status display
3. **Firmware**: Write test sketch to publish dummy SCT-013 data to MQTT over TLS
4. **Infrastructure**: Set up local MQTT broker with TLS via docker-compose

**After Soldering**:
1. Flash firmware and test power readings
2. Connect mobile app to live data feed
3. Implement cloud storage and analytics
4. Deploy web dashboard

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

Built with passion for IoT innovation and smart home automation.
