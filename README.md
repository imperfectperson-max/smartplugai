# Smart Plug AI

An intelligent, **security-first** IoT smart plug system that monitors power consumption in real-time, learns usage patterns, and provides actionable insights through mobile and web interfaces.

## 🚀 Project Overview

Smart Plug AI is a comprehensive IoT solution combining hardware security, encrypted communications, and user privacy protection with powerful energy monitoring capabilities:

- **🔒 Security-First Design**: Hardware secure boot, TLS 1.3 encryption, certificate-based authentication
- **⚡ Smart Hardware**: ESP32-based plug with INA219 power monitoring and secure firmware
- **📱 Mobile App**: Flutter cross-platform app with biometric auth and encrypted storage
- **🌐 Web Dashboard**: React-based admin panel with role-based access control
- **☁️ Secure Backend**: Cloud API with POPIA/GDPR compliance and encrypted data storage
- **🧠 Intelligence**: ML-powered pattern recognition and security anomaly detection

## 📋 Quick Start

### Prerequisites

- **Hardware**: ESP32, INA219, relay modules ([soldering checklist](docs/soldering-checklist.md))
- **Mobile**: Flutter SDK 3.x+, Dart 3.x+
- **Web**: Node.js 18+, npm/yarn
- **Backend**: Python 3.10+, Docker

### Getting Started

```bash
# Clone the repository
git clone https://github.com/imperfectperson-max/smartplugai.git
cd smartplugai

# Choose your development area:
# 📱 Mobile App    → see app/README.md
# 🌐 Web Dashboard → see web/README.md
# 🔧 Firmware     → see firmware/README.md
# ☁️ Backend      → see backend/README.md
```

## ⚠️ Hardware Safety Warning

**DANGER - HIGH VOLTAGE**: This project involves mains electricity (110-240V AC). Improper handling can cause:

- Electric shock, serious injury, or death
- Fire hazards and property damage
- Equipment damage

**Safety Requirements**:

- ⚡ Only qualified individuals should work with mains voltage
- 🔌 Always disconnect power before working on circuits
- 🛡️ Use proper insulation, fusing, and safety enclosures
- ✅ Follow local electrical codes and regulations
- 🧪 Test thoroughly in safe environments before deployment

See the [soldering checklist](docs/soldering-checklist.md) for complete safety procedures.

## 📚 Documentation

### Core Documentation

- **[Complete Project Plan](Complete%20Smart%20Plug%20AI%20plan.txt)** - Comprehensive security-enhanced implementation roadmap
- **[Security Policy](SECURITY.md)** - Security architecture, compliance, and vulnerability disclosure
- **[Architecture](docs/ARCHITECTURE.md)** - System design and component interactions
- **[API Reference](docs/API.md)** - REST API, WebSocket, and MQTT documentation
- **[Roadmap](docs/ROADMAP.md)** - Development phases and milestones
- **[Changelog](CHANGELOG.md)** - Version history and notable changes

### Development Guides

- [Soldering Checklist](docs/soldering-checklist.md) - Hardware assembly and safety
- [Contributing Guidelines](CONTRIBUTING.md) - How to contribute to the project
- [Code of Conduct](CODE_OF_CONDUCT.md) - Community guidelines

## 🤝 Contributing

We welcome contributions! **Security-conscious contributions are especially valued.**

**Before Contributing**:

1. Read [CONTRIBUTING.md](CONTRIBUTING.md) for development workflow
2. Review [SECURITY.md](SECURITY.md) for security requirements
3. Check [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) for community standards
4. Review the [Complete Project Plan](Complete%20Smart%20Plug%20AI%20plan.txt) for technical direction

**How to Contribute**:

- 🐛 Report bugs and security issues (see [SECURITY.md](SECURITY.md) for security reports)
- 💡 Propose new features aligned with project goals
- 📝 Improve documentation and examples
- 🔧 Submit pull requests with tests and documentation
- 🔒 Help with security reviews and testing

## 🔐 Security

Security is a fundamental principle of Smart Plug AI, not an afterthought.

**Key Security Features**:

- Hardware secure boot and flash encryption (ESP32)
- TLS 1.3 encryption for all communications
- Certificate-based device authentication
- POPIA and GDPR compliance
- Regular security audits and updates

**Report Security Vulnerabilities**: See [SECURITY.md](SECURITY.md) for our vulnerability disclosure policy.

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

Built with a commitment to secure IoT innovation, user privacy, and smart home automation.

---

**Project Status**: Active Development | **Version**: 0.2.0 | **Last Updated**: December 2025
