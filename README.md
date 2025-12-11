# Smart Plug AI

An intelligent IoT smart plug system that monitors power consumption in real-time, learns usage patterns, and provides actionable insights through mobile and web interfaces.

## 🚀 Project Overview

Smart Plug AI combines:
- **Hardware**: ESP32-based smart plug with INA219 power monitoring
- **Mobile App**: Flutter-based cross-platform app for real-time monitoring
- **Web Dashboard**: React-based admin dashboard for analytics
- **Backend**: Cloud-based API with Firebase/Supabase integration
- **Intelligence**: Pattern recognition and anomaly detection for power usage

## 📋 Quick Start for Developers

### Prerequisites
- **Hardware Development**: ESP32, INA219, relay modules (see [soldering checklist](docs/soldering-checklist.md))
- **Mobile App**: Flutter SDK 3.x+, Dart 3.x+
- **Web Dashboard**: Node.js 18+, npm/yarn
- **Backend**: Python 3.10+, Docker (optional for local MQTT)

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

3. **Review the roadmap**: Check [docs/ROADMAP.md](docs/ROADMAP.md) for milestones and priorities

## 🔨 Hardware Soldering Preparation

**Next Session**: [Soldering Checklist](docs/soldering-checklist.md)

Before the soldering session:
- ✅ Verify parts list (ESP32, INA219, relay, connectors)
- ✅ Review safety procedures and workspace setup
- ✅ Test components individually with breadboard
- ✅ Plan data collection and initial firmware test

## 🤝 Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for:
- Code style and conventions
- How to submit issues and pull requests
- Development workflow and branch naming
- Testing requirements

Also review our [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md).

## 📚 Documentation

- [Roadmap](docs/ROADMAP.md) - Project phases and milestones
- [Soldering Checklist](docs/soldering-checklist.md) - Hardware preparation guide
- [Architecture](docs/ARCHITECTURE.md) - System architecture and design
- [API Documentation](docs/API.md) - REST API, WebSocket, and MQTT reference

## 🎯 Where to Start

**Before Hardware is Ready**:
1. **Mobile App**: Build mock UI for auth, power monitoring, and QR pairing
2. **Web Dashboard**: Create placeholder dashboard with sample data
3. **Firmware**: Write test sketch to publish dummy INA219 data to MQTT
4. **Infrastructure**: Set up local MQTT broker with docker-compose

**After Soldering**:
1. Flash firmware and test power readings
2. Connect mobile app to live data feed
3. Implement cloud storage and analytics
4. Deploy web dashboard

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

Built with passion for IoT innovation and smart home automation.
