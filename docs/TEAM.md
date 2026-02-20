# 👥 Smart Plug AI — Team Structure

Team composition and roles across the 3-year Smart Plug AI journey.

## 📋 Table of Contents

- [Year 1: Research Team](#-year-1-research-team-3-people)
- [Year 2: Production Team](#-year-2-production-team-4-people)
- [Year 3: Commercial Team](#-year-3-commercial-team-8-people)
- [Hiring Plan](#-hiring-plan)

---

## 🔬 Year 1: Research Team (3 People)

**Period**: Months 1–8 (Phase 0: Research Foundation)  
**Focus**: SA-NILM dataset collection, ML model development, documentation

| Role | Responsibilities | Skills Required |
|------|-----------------|----------------|
| **ML Researcher** | Dataset collection, model development (CNN, LSTM, ensemble), accuracy validation, documentation | Python, TensorFlow/Keras, Pandas, NumPy, research methodology, Jupyter |
| **Hardware Engineer** | Basic plug builds, sensor calibration, data collection deployment, PCB basics | Electronics, ESP32, C/C++, soldering, oscilloscope, multimeter |
| **Documentation Lead** | Architecture docs, research logging, project management, GitHub, dataset preparation | Technical writing, organisation, GitHub, Markdown, LaTeX (optional) |

### Year 1 Deliverables by Role

| Role | Key Deliverables |
|------|-----------------|
| ML Researcher | SA-NILM Dataset v1.0, SA-NILM-Ensemble v1.0 (91% accuracy), DATASET.md |
| Hardware Engineer | 5 data collection plugs deployed, calibration reports |
| Documentation Lead | ARCHITECTURE.md, ROADMAP.md, SECURITY.md, DATASET.md, BUSINESS.md |

---

## ⚙️ Year 2: Production Team (4 People)

**Period**: Months 9–18 (Phase 1 + Phase 2)  
**Focus**: Secure hardware prototype + full system build

| Role | Responsibilities | Skills Required |
|------|-----------------|----------------|
| **Firmware Engineer** | Secure boot implementation, TFLite Micro ML integration, OTA updates, TLS 1.3, ATECC608A driver | C/C++, ESP-IDF, TFLite Micro, security protocols, MQTT |
| **Mobile Developer** | Flutter app with 4-layer BLoC architecture, AI insights UI, biometric auth, offline mode | Flutter, Dart, Firebase, BLoC pattern, secure storage |
| **Backend Developer** | Secure FastAPI, MQTT broker, database design, scaling, audit logging | Python, FastAPI, PostgreSQL, InfluxDB, Docker, Redis |
| **Hardware Lead** | Production PCB design, secure enclosure, component sourcing, safety testing | Electronics, KiCad, safety certification, manufacturing |

### Year 2 Deliverables by Role

| Role | Key Deliverables |
|------|-----------------|
| Firmware Engineer | v1.0 secure firmware, TFLite Micro integration, OTA system |
| Mobile Developer | v1.0 Flutter app with all features, app store ready |
| Backend Developer | v1.0 backend handling 1,000+ devices, security audit passed |
| Hardware Lead | 3 secure prototypes, production PCB, enclosure design |

---

## 🚀 Year 3: Commercial Team (8 People)

**Period**: Months 19–24+ (Phase 3 + Commercial Launch)  
**Focus**: Competition, investor readiness, commercial launch

| Department | Headcount | Roles |
|------------|-----------|-------|
| **Engineering** | 4 | Firmware, Mobile, Backend, Hardware (retained + additions) |
| **Sales & Marketing** | 2 | Enterprise sales, digital marketing / community |
| **Operations** | 1 | Finance, procurement, compliance, HR |
| **CEO / Founder** | 1 | Strategy, investor relations, partnerships |

### Year 3 Key Hires

| Role | Priority | Source |
|------|---------|--------|
| Enterprise Sales Lead | High | Hire externally — energy/IoT sector experience |
| Digital Marketer | Medium | Hire externally or freelance |
| Operations Manager | High | Hire externally or promote internally |
| Additional Engineer | Medium | Hire from SA university programs |

---

## 📋 Hiring Plan

```
Month 1–8:   3-person research team (founders/co-founders)
Month 9:     Add Firmware Engineer (4th person)
Month 13:    Firmware + Mobile + Backend + Hardware lead fully active
Month 19:    Begin hiring Sales and Operations roles
Month 22:    Full 8-person commercial team in place
Month 24:    Seed round close → scale team as funded
```

### Compensation Philosophy

| Stage | Model | Notes |
|-------|-------|-------|
| Year 1 | Equity only or minimal stipend | Bootstrap phase — competition prizes supplement |
| Year 2 | Partial salary + equity | Competition winnings and early revenue |
| Year 3 | Full market salaries | Post-seed investment |

---

*See [docs/BUSINESS.md](BUSINESS.md) for the business plan and [docs/COMPETITION.md](COMPETITION.md) for the competition strategy that funds team growth.*
