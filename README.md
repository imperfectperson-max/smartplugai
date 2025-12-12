# Smart Plug AI

An intelligent IoT smart plug system that monitors power consumption in real-time, learns usage patterns, and provides actionable insights through mobile and web interfaces.

## 🚀 Project Overview

Smart Plug AI combines:
- **Hardware**: ESP32-S3-based smart plug with SCT-013 non-invasive current sensing, ATECC608A secure element, tamper detection, and certified power supply
- **Mobile App**: Flutter-based cross-platform app for real-time monitoring with end-to-end encryption
- **Web Dashboard**: React-based admin dashboard for analytics with role-based access control
- **Backend**: Secure cloud API (FastAPI) with Auth0/Firebase Auth, field-level encryption, and TLS 1.3
- **Intelligence**: Pattern recognition and anomaly detection for power usage
- **Security**: Secure boot, flash encryption, signed commands, device attestation, and tamper detection

## 📋 Quick Start for Developers

### Prerequisites
- **Hardware Development**: ESP32-S3-WROOM-1-N8, ATECC608A secure element, SCT-013 current sensor with burden resistor, relay modules, certified 5V/2A phone charger (see [soldering checklist](docs/soldering-checklist.md))
- **Secure Provisioning**: ATECC608A provisioning tools, ESP-IDF for secure boot and flash encryption setup
- **Mobile App**: Flutter SDK 3.x+, Dart 3.x+
- **Web Dashboard**: Node.js 18+, npm/yarn
- **Backend**: Python 3.10+, FastAPI, Auth0/Firebase Auth, Firestore with field-level encryption, Docker (for local MQTT with TLS)

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

3. **Review the roadmap**: Check [docs/ROADMAP.md](docs/ROADMAP.md) for milestones and priorities (Phase 1: 12 weeks with security milestones)

4. **Security documentation**: See [docs/SECURITY.md](docs/SECURITY.md) for comprehensive security architecture and threat model

## 🔨 Hardware Soldering Preparation

**Next Session**: [Soldering Checklist](docs/soldering-checklist.md)

Before the soldering session:
- ✅ Verify parts list (ESP32-S3-WROOM-1-N8, ATECC608A secure element, SCT-013 with burden resistor, relay, connectors, 24C256 secure storage, MAX6316 tamper watchdog)
- ✅ Review safety procedures and workspace setup including fuse and MOV (Metal Oxide Varistor) protection
- ✅ Test components individually with breadboard
- ✅ Plan data collection and initial firmware test
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

- [Roadmap](docs/ROADMAP.md) - Project phases and milestones (Phase 1: 12 weeks with security focus)
- [Security Architecture](docs/SECURITY.md) - Comprehensive security design, threat model, and certifications
- [Soldering Checklist](docs/soldering-checklist.md) - Hardware preparation guide
- [Architecture](docs/ARCHITECTURE.md) - System architecture and design
- [API Documentation](docs/API.md) - REST API, WebSocket, and MQTT reference

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
