# 🔌 Smart Plug AI

> South Africa's first AI-powered energy intelligence platform with bank-grade security

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Status: Pre-Seed](https://img.shields.io/badge/Status-Pre--Seed-orange.svg)]()
[![Accuracy: 91%](https://img.shields.io/badge/ML%20Accuracy-91%25-brightgreen.svg)]()
[![Security: Bank-Grade](https://img.shields.io/badge/Security-Bank--Grade-blue.svg)]()

---

## 📋 Table of Contents

- [Executive Summary](#-executive-summary)
- [The Problem](#-the-problem)
- [The Solution](#-the-solution)
- [Hardware Architecture](#-hardware-architecture)
- [Quick Start](#-quick-start)
- [Documentation](#-documentation)
- [Roadmap](#-roadmap)
- [Security](#-security)
- [Contributing](#-contributing)
- [License](#-license)

---

## 🌍 Executive Summary

```
PROJECT:        Smart Plug AI
DURATION:       24 Months (2026–2027)
TEAM:           4 People (expandable to 6)
BUDGET:         R40,000 (Year 1: R15k, Year 2: R25k)
STATUS:         Pre-Seed / Research Phase

THE VISION:
"South Africa's first AI-powered energy intelligence platform
with bank-grade security, delivering 91% accurate appliance
detection and actionable savings to households and businesses."
```

---

## ⚡ The Problem

- 🇿🇦 SA loses **R80 billion** annually to energy inefficiency
- 🔦 Load shedding costs **R1 billion per stage per day**
- ❓ No one knows **WHAT** is using power, **WHEN**, or **HOW** to optimise
- 💸 Existing solutions: expensive, insecure, or inaccurate — and none built for SA

---

## 🎯 The Solution

Smart Plug AI combines five capabilities into one platform:

| # | Capability | Description |
|---|-----------|-------------|
| 1 | **Research** | First SA-specific appliance dataset — 91% accuracy |
| 2 | **Hardware** | Bank-grade security with ATECC608A secure element |
| 3 | **AI** | On-device ML with 91% appliance recognition |
| 4 | **Insights** | Mobile app showing exactly what's running and how to save |
| 5 | **Impact** | 30% average electricity bill reduction |

### 📊 Market Opportunity

| Segment | Size | Annual Spend |
|---------|------|-------------|
| SA Households | 6 million | R30 billion |
| SA SMEs | 500,000+ | R50 billion |
| Global Emerging Markets | 1.4 billion people | $200B+ opportunity |

---

## ⚙️ Hardware Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                   SMART PLUG AI — HARDWARE                   │
├─────────────────────────────────────────────────────────────┤
│  POWER SECTION                                               │
│  AC Input 230V 50Hz → Fuse (3A Quick) → MOV 275V            │
│  Relay 10A+ → Current Sensor SCT-013-030 → AC Out Socket    │
│  Burden Resistor 33Ω 1W                                      │
├─────────────────────────────────────────────────────────────┤
│  CONTROL SECTION                                             │
│  ESP32-S3-WROOM-1-N8                                         │
│    Dual-core Xtensa LX7 @ 240MHz                             │
│    512KB SRAM, 8MB PSRAM                                     │
│    Hardware crypto acceleration                              │
│    Secure Boot + Flash Encryption                            │
│  ATECC608A   — Secure Element (key storage, ECDSA)          │
│  MAX6316     — Tamper Watchdog                               │
│  24C256      — Encrypted EEPROM                              │
├─────────────────────────────────────────────────────────────┤
│  POWER SUPPLY                                                │
│  5V/2A Phone Charger → 3.3V LDO → ESP32 & ICs               │
├─────────────────────────────────────────────────────────────┤
│  PHYSICAL SECURITY                                           │
│  Anti-tamper screws (security bit required)                  │
│  Tamper-evident seals (void if removed)                      │
│  Enclosure tamper switch → MAX6316                           │
│  Potting compound over sensitive components                  │
└─────────────────────────────────────────────────────────────┘
```

**Bill of Materials** (per plug):

| Component | Specification | Purpose |
|-----------|--------------|---------|
| ESP32-S3-WROOM-1-N8 | Dual-core @ 240MHz, 8MB PSRAM | Main processor + crypto |
| ATECC608A-TNGTLS | ECDSA P256, hardware RNG | Secure key storage |
| SCT-013-030 | 30A/1V non-invasive | Current sensing |
| MAX6316 | Tamper watchdog | Physical security |
| 24C256 EEPROM | 32KB encrypted | Config storage |
| Relay Module | 5V, 10A+, optocoupler | Load control |
| Fuse + MOV | 3A quick-blow + 275V | Safety protection |

> ⚠️ **SAFETY**: This project involves 230V AC mains voltage. Follow all safety procedures in [docs/soldering-checklist.md](docs/soldering-checklist.md).

---

## 🚀 Quick Start

### Prerequisites

- **Firmware**: ESP-IDF v5.0+, Python 3.10+
- **Mobile App**: Flutter SDK 3.x+, Dart 3.x+
- **Backend**: Python 3.10+, FastAPI, Docker
- **Web Dashboard**: Node.js 18+, npm/yarn

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

3. **Review the roadmap**: See [docs/ROADMAP.md](docs/ROADMAP.md) for the full 24-month, 4-phase plan.

4. **Security architecture**: See [docs/SECURITY.md](docs/SECURITY.md) for ATECC608A configuration, secure boot chain, and MQTT TLS setup.

---

## 📚 Documentation

| Document | Description |
|----------|-------------|
| [ROADMAP.md](docs/ROADMAP.md) | 24-month roadmap — 4 phases, milestones, budgets |
| [ARCHITECTURE.md](docs/ARCHITECTURE.md) | 4-layer architecture: Hardware, Firmware, Backend, Mobile |
| [SECURITY.md](docs/SECURITY.md) | Bank-grade security: ATECC608A, secure boot, TLS 1.3, E2E encryption |
| [API.md](docs/API.md) | REST API, WebSocket, and MQTT topic reference |
| [DATASET.md](docs/DATASET.md) | SA-NILM dataset specs and model card (91% accuracy) |
| [BUSINESS.md](docs/BUSINESS.md) | Executive summary, revenue model, competition matrix |
| [TEAM.md](docs/TEAM.md) | Team structure for Years 1–3 |
| [PITCH.md](docs/PITCH.md) | 10-slide investor pitch deck |
| [COMPETITION.md](docs/COMPETITION.md) | Competition strategy for Years 1–3 |
| [soldering-checklist.md](docs/soldering-checklist.md) | Hardware shopping list and assembly guide |

---

## 🗺️ Roadmap

| Phase | Period | Budget | Focus |
|-------|--------|--------|-------|
| **Phase 0** | Months 1–8 | R15,000 | Research Foundation — SA dataset, 91% ML models |
| **Phase 1** | Months 9–12 | R15,000 | Secure Prototype — ESP32-S3 + ATECC608A |
| **Phase 2** | Months 13–18 | R15,000 | Full System — Backend, mobile app, ML pipeline |
| **Phase 3** | Months 19–24 | R10,000 | Polish + Competition — Enclosures, demos, championship |

See [docs/ROADMAP.md](docs/ROADMAP.md) for full details.

---

## 🔒 Security Highlights

Smart Plug AI implements **bank-grade security** across every layer:

- **Hardware**: ATECC608A secure element — ECDSA keys never leave the chip
- **Boot**: 8-step verified secure boot chain via ATECC608A
- **Transport**: MQTT 5.0 over TLS 1.3, mutual authentication, cert rotation every 90 days
- **Encryption**: AES-256-GCM per message, E2E — cloud never sees plaintext
- **Physical**: Anti-tamper screws, void seals, potting compound, MAX6316 watchdog

See [docs/SECURITY.md](docs/SECURITY.md) for the complete security architecture.

---

## 🤝 Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for:
- Code style and conventions
- How to submit issues and pull requests
- Development workflow and branch naming
- Testing requirements

Also review our [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md).

---

## 📄 License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

---

*Built in South Africa 🇿🇦 — solving Africa's energy challenge with AI and bank-grade security.*
