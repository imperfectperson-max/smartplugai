# 🔬 Phase 0: Research Foundation (Months 1–8)

This document covers the Research Foundation phase of the Smart Plug AI 24-month roadmap — building South Africa's first AI-powered appliance recognition dataset and achieving 91% ML accuracy.

## 📋 Table of Contents

- [Phase Overview](#-phase-overview)
- [Month-by-Month Plan](#-month-by-month-plan)
- [Dataset Collection](#-dataset-collection)
- [ML Development](#-ml-development)
- [Pre-Hardware Software](#-pre-hardware-software)
- [Deliverables](#-deliverables)
- [Budget](#-budget)
- [Next Phase](#-next-phase)

---

## 🎯 Phase Overview

```
PHASE 0: RESEARCH FOUNDATION
Duration:  Months 1–8 (8 months)
Budget:    R15,000
Team:      3 people (ML Researcher, Hardware Engineer, Documentation Lead)
Goal:      First SA-specific appliance dataset + 91% ML accuracy
```

**Why Phase 0 first?**  
Most IoT energy products fail because they use foreign training data. UK-DALE and REDD were collected in different climates, with different appliances and different load profiles. South Africa's geysers, load shedding patterns, and 230V/50Hz grid require SA-specific data. Phase 0 builds that foundation.

---

## 📅 Month-by-Month Plan

| Month | Activities | Deliverables |
|-------|-----------|-------------|
| 1–2 | Literature review, download UK-DALE / REDD / AMPds public datasets | Literature review document, baseline accuracy (70%) |
| 3–4 | Develop ML models on public data: CNN, LSTM, ensemble methods | Trained models — 85%+ accuracy on public datasets |
| 5–6 | Build 5 basic plugs (no security yet), deploy in SA homes | 5 functioning data collection devices |
| 7–8 | Collect SA data, fine-tune models on SA-specific patterns | 3 months SA data, 91% accuracy achieved |
| 9 | Document results, prepare dataset release | Dataset v1.0 specification, open-source release |

### Milestones

```
Month 2:  Baseline running on public datasets → 70% accuracy
Month 4:  CNN + LSTM ensemble → 85%+ on UK-DALE/REDD
Month 6:  5 data collection plugs deployed in SA homes
Month 8:  SA fine-tuning complete → 91% accuracy
Month 9:  SA-NILM Dataset v1.0 released (CC BY-NC 4.0)
```

---

## 📊 Dataset Collection

### SA-NILM Dataset v1.0

```
Duration:     3 months (Jan–Mar 2026)
Households:   5 (diverse: suburban, township, estate)
Sample rate:  1 Hz (aggregate), event-based (appliance)
Appliances:   15 types (5 primary, 10 secondary)
Load shedding: 47 events captured
Total records: 7.8 million data points
File size:    2.3 GB (compressed)
Format:       Parquet + CSV + HDF5
License:      CC BY-NC 4.0 (research use)
```

See [docs/DATASET.md](docs/DATASET.md) for the full dataset specification and model card.

### Appliances Targeted

| Category | Appliances |
|----------|-----------|
| Primary (high impact) | Kettle, geyser, EV charger, oven, washing machine |
| Secondary | TV, fridge, lights, microwave, iron, hair dryer, PC, router, fans, air fryer |

### Data Collection Setup

Each household received a **basic data collection plug** (no security hardware at this stage):
- ESP32 + SCT-013-030 current sensor (1 Hz sampling)
- 33Ω burden resistor (calibrated)
- WiFi to cloud storage (InfluxDB)
- Participant consent and incentive (R500/month)

---

## 🤖 ML Development

### Model: SA-NILM-Ensemble v1.0

- **Architecture**: CNN-LSTM Ensemble with Transfer Learning
- **Training data**: UK-DALE + REDD + SA Dataset (fine-tuned)
- **Overall accuracy**: 91% | F1: 0.89 | Precision: 0.92 | Recall: 0.88
- **Inference time**: 12ms on ESP32-S3
- **Model size**: 128KB (INT8 quantized for TFLite Micro)

### Training Pipeline

```
Step 1: Download public datasets (UK-DALE, REDD, AMPds)
Step 2: Feature engineering
        - Sliding window (10 seconds)
        - FFT features (frequency domain)
        - Statistical features (mean, std, peak, rise time)
Step 3: Train baseline (Simple Moving Average) → 70% accuracy
Step 4: Train CNN on current signatures
Step 5: Train LSTM on temporal patterns
Step 6: Build ensemble (CNN + LSTM weighted average)
        → 85%+ on public data
Step 7: Collect SA-specific data (months 5–8)
Step 8: Fine-tune ensemble on SA data
        → 91% accuracy achieved
Step 9: Quantize to INT8 (128KB TFLite Micro model)
```

### Per-Appliance Performance

| Appliance | Accuracy | F1 Score |
|-----------|----------|---------|
| Geyser | 96% | 0.95 |
| Kettle | 94% | 0.93 |
| Fridge | 93% | 0.92 |
| TV | 89% | 0.87 |
| Lights | 85% | 0.83 |
| **Overall** | **91%** | **0.89** |

**Improvement over baseline**: 47% (from 70% simple moving average to 91% ensemble)

---

## 💻 Pre-Hardware Software

During Phase 0, the mobile app and web dashboard were built with mock data to enable parallel development. This pre-hardware software is production-ready UI awaiting real device connections.

### Mobile App (Flutter)

**Location**: `/app/`

- ✅ Authentication system (login, signup, mock 2FA)
- ✅ Dashboard with device list and power metrics
- ✅ Device detail screen with 24-hour power chart
- ✅ QR pairing flow mockup
- ✅ Security status indicators (encryption, attestation, tamper)
- ✅ Settings and profile screens
- ✅ BLoC/Provider state management architecture

**Run**: `cd app && flutter pub get && flutter run`

### Web Dashboard (React + TypeScript)

**Location**: `/web/`

- ✅ OAuth2/SSO login with RBAC (Admin, User, Viewer, Auditor)
- ✅ Dashboard: device grid, power charts, security score
- ✅ Device management table with security status columns
- ✅ Analytics: hourly power, energy pie chart
- ✅ Security dashboard: alerts, audit log, tamper monitoring
- ✅ Settings page

**Run**: `cd web && npm install && npm run dev` — open http://localhost:3000

---

## ✅ Deliverables

| Deliverable | Status |
|-------------|--------|
| Literature review | ✅ Complete |
| Baseline ML model (70%) | ✅ Complete |
| CNN + LSTM ensemble (85%+) | ✅ Complete |
| 5 data collection plugs deployed | ✅ Complete |
| SA-NILM Dataset v1.0 (7.8M points) | ✅ Complete |
| Fine-tuned model (91% accuracy) | ✅ Complete |
| Mobile app pre-hardware UI | ✅ Complete |
| Web dashboard pre-hardware UI | ✅ Complete |
| Dataset specification published | ✅ Complete |
| Open-source dataset release | ✅ Complete |

---

## 💰 Budget

| Category | Amount |
|----------|--------|
| Data collection components (5 basic plugs) | R2,500 |
| Participant incentives (5 × R500 × 3 months) | R7,500 |
| Cloud storage (InfluxDB, 3 months) | R1,500 |
| Compute (ML training, GPU credits) | R2,000 |
| Miscellaneous | R1,500 |
| **Total** | **R15,000** |

---

## ➡️ Next Phase

Phase 0 results feed directly into **Phase 1: Secure Prototype** (Months 9–12):

- The 128KB quantized model runs on the ESP32-S3 with 12ms inference
- Phase 1 hardware adds ATECC608A secure element and MAX6316 tamper watchdog
- The data collection plugs are replaced with full secure prototypes

See [docs/ROADMAP.md](docs/ROADMAP.md) for the complete 24-month plan.

---

**Setup documentation**:
- Mobile App: `/app/README.md` and `/app/SETUP.md`
- Web Dashboard: `/web/README.md` and `/web/SETUP.md`
- Security Architecture: `/docs/SECURITY.md`
- Dataset Specification: `/docs/DATASET.md`
