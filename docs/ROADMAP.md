# 🗺️ Smart Plug AI — 24-Month Roadmap

South Africa's first AI-powered energy intelligence platform, built over 24 months in 4 phases.

## 📋 Table of Contents

- [Phase Overview](#-phase-overview)
- [Phase 0: Research Foundation](#-phase-0-research-foundation-months-1-8)
- [Phase 1: Secure Prototype](#-phase-1-secure-prototype-months-9-12)
- [Phase 2: Full System](#-phase-2-full-system-months-13-18)
- [Phase 3: Polish + Competition](#-phase-3-polish--competition-months-19-24)
- [Success Metrics](#-success-metrics)
- [Budget Summary](#-budget-summary)

---

## 📊 Phase Overview

| Phase | Period | Budget | Focus |
|-------|--------|--------|-------|
| **Phase 0** | Months 1–8 | R15,000 | Research Foundation — SA dataset, 91% ML models |
| **Phase 1** | Months 9–12 | R15,000 | Secure Prototype — ESP32-S3 + ATECC608A |
| **Phase 2** | Months 13–18 | R15,000 | Full System — Backend, mobile app, ML pipeline |
| **Phase 3** | Months 19–24 | R10,000 | Polish + Competition — Enclosures, demos, championship |
| **TOTAL** | 24 months | **R55,000** | |

**Weekly guides** for Phase 1 hardware work are in the `docs/` folder:
[WEEK1](WEEK1_HARDWARE_MASTERY.md) · [WEEK1 Quick Ref](WEEK1_QUICK_REFERENCE.md) · [WEEK3](WEEK3_BACKEND_INTEGRATION.md) · [WEEK4](WEEK4_SAFETY_ENCLOSURE_PCB.md) · [WEEK5](WEEK5_AUTOMATION_RULES.md) · [WEEK7](WEEK7_PILOT_DEPLOYMENT.md) · [Soldering Checklist](soldering-checklist.md)

---

## 🔬 Phase 0: Research Foundation (Months 1–8)

**Goal**: Build South Africa's first appliance recognition dataset and achieve 91% ML accuracy.

**Budget**: R15,000 (components, participant incentives, cloud storage)

| Month | Activities | Deliverables |
|-------|-----------|-------------|
| 1–2 | Literature review, public dataset download (UK-DALE, REDD, AMPds) | Literature review document, baseline accuracy (70%) |
| 3–4 | Develop ML models on public data (CNN, LSTM, ensemble) | Trained models with 85%+ accuracy |
| 5–6 | Build 5 basic plugs, deploy in SA homes | 5 functioning data collection devices |
| 7–8 | Collect SA data, fine-tune models | 3 months SA data, 91% accuracy achieved |
| 9 | Document results, prepare write-up | Dataset specification, dataset release |

### Phase 0 Milestones

```
Month 2:  Baseline model running on public datasets — 70% accuracy
Month 4:  CNN + LSTM ensemble — 85%+ accuracy on UK-DALE/REDD
Month 6:  5 data collection plugs deployed in SA homes
Month 8:  SA-specific fine-tuning complete — 91% accuracy achieved
Month 9:  Dataset v1.0 released (CC BY-NC 4.0)
```

### Phase 0 Deliverables

- �� **SA-NILM Dataset v1.0**: 7.8M data points, 5 households, 47 load-shedding events
- 🤖 **SA-NILM-Ensemble v1.0**: CNN-LSTM ensemble, 91% accuracy, 128KB quantized model
- 📄 **Dataset specification** and model card (see [docs/DATASET.md](DATASET.md))
- �� **Open-source release** under CC BY-NC 4.0

---

## 🔐 Phase 1: Secure Prototype (Months 9–12)

**Goal**: Build 3 secure prototypes with bank-grade hardware security.

**Budget**: R15,000 (components, PCB fabrication)

| Month | Activities | Deliverables |
|-------|-----------|-------------|
| 9–10 | Design secure hardware (ESP32-S3 + ATECC608A) | Schematics, PCB layout, BOM |
| 10–11 | Develop secure firmware (secure boot, TLS, ML integration) | v0.1 firmware with basic security |
| 11–12 | Build 3 secure prototypes | 3 working plugs with bank-grade security |
| 12 | Security testing + validation | Security audit report |

**Hardware components per plug**: ESP32-S3-WROOM-1-N8, ATECC608A-TNGTLS, SCT-013-030, MAX6316 tamper watchdog, 24C256 EEPROM, 10A relay, fuse + MOV.

**Weekly guides** (for the detailed build process):
- [WEEK1_HARDWARE_MASTERY.md](WEEK1_HARDWARE_MASTERY.md) — Assembly and soldering
- [WEEK1_QUICK_REFERENCE.md](WEEK1_QUICK_REFERENCE.md) — Quick reference card
- [WEEK4_SAFETY_ENCLOSURE_PCB.md](WEEK4_SAFETY_ENCLOSURE_PCB.md) — PCB design and safety
- [soldering-checklist.md](soldering-checklist.md) — Shopping list and prep checklist

---

## 🏗️ Phase 2: Full System (Months 13–18)

**Goal**: Build the complete end-to-end system — backend, mobile app, web dashboard, and ML pipeline.

**Budget**: R15,000 (cloud credits, testing devices)

| Month | Activities | Deliverables |
|-------|-----------|-------------|
| 13–14 | Backend development (FastAPI, MQTT, DB) | v0.1 backend with basic APIs |
| 14–15 | Mobile app development (Flutter) | v0.1 app with device pairing |
| 15–16 | Web dashboard development (React) | v0.1 dashboard with analytics |
| 16–17 | ML pipeline integration | End-to-end ML working |
| 17–18 | System integration + testing | v1.0 prototype complete |

**Weekly guides**:
- [WEEK3_BACKEND_INTEGRATION.md](WEEK3_BACKEND_INTEGRATION.md) — Backend and MQTT setup
- [WEEK5_AUTOMATION_RULES.md](WEEK5_AUTOMATION_RULES.md) — Automation rule engine
- [WEEK7_PILOT_DEPLOYMENT.md](WEEK7_PILOT_DEPLOYMENT.md) — Deployment and monitoring

---

## 🏆 Phase 3: Polish + Competition (Months 19–24)

**Goal**: Achieve championship-ready polish and win major competitions.

**Budget**: R10,000 (enclosures, demo materials, travel)

| Month | Activities | Deliverables |
|-------|-----------|-------------|
| 19–20 | Load testing, security audit | Performance report, security certification |
| 20–21 | Enclosure design, professional appearance | Production-ready enclosures |
| 21–22 | Demo development, video production | 5-minute championship demo |
| 22–23 | Rehearsals, feedback, refinements | Polished presentation |
| 23–24 | Competition preparation | Championship-ready |

**Target competitions**: See [docs/COMPETITION.md](COMPETITION.md) for the full Year 1–3 competition strategy.

---

## 🎯 Success Metrics

### Year 1 (Months 1–8): Research

| Metric | Target |
|--------|--------|
| Dataset collected | 5 homes, 3 months |
| ML accuracy | 91% |
| Documentation published | ✅ |
| Open-source release | ✅ |
| Competition presentations | 3+ |

### Year 2 (Months 9–18): Production

| Metric | Target |
|--------|--------|
| Working secure prototypes | 3 |
| Mobile app features | All features live |
| Backend capacity | 1,000+ devices |
| Security audit | Passed |
| Competition wins | 3+ |

### Year 3 (Months 19–24+): Commercial

| Metric | Target |
|--------|--------|
| Funding raised | R2M seed |
| Units sold | 1,000 |
| Revenue | R5M |
| Enterprise clients | 10+ |

---

## 💰 Budget Summary

| Phase | Period | Amount | Key Spend |
|-------|--------|--------|-----------|
| Phase 0 | Months 1–8 | R15,000 | Components, participant incentives, cloud storage |
| Phase 1 | Months 9–12 | R15,000 | Components, PCB fabrication |
| Phase 2 | Months 13–18 | R15,000 | Cloud credits, testing devices |
| Phase 3 | Months 19–24 | R10,000 | Enclosures, demo materials, travel |
| **TOTAL** | **24 months** | **R55,000** | |

---

*See [ARCHITECTURE.md](ARCHITECTURE.md) for technical stack details and [SECURITY.md](SECURITY.md) for the security architecture.*
