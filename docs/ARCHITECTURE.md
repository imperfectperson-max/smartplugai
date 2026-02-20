# 🏗️ Smart Plug AI — System Architecture

Full 4-layer architecture for the Smart Plug AI platform: Hardware, Firmware, Backend, and Mobile.

## 📋 Table of Contents

- [System Overview](#-system-overview)
- [Hardware Architecture](#️-hardware-architecture)
- [Firmware Architecture](#-firmware-architecture)
- [Backend Architecture](#️-backend-architecture)
- [Mobile Architecture](#-mobile-architecture)
- [Data Flow](#-data-flow)
- [Security Architecture](#-security-architecture)

---

## 🌐 System Overview

```
┌─────────────────────────────────────────────────────────────────────┐
│                        SMART PLUG AI PLATFORM                        │
├───────────────┬────────────────────┬────────────────┬───────────────┤
│   HARDWARE    │      FIRMWARE      │    BACKEND     │    MOBILE     │
│  ESP32-S3 +   │   5-Layer Stack    │  Secure Cloud  │   Flutter     │
│  ATECC608A    │   TFLite Micro     │  FastAPI/MQTT  │   BLoC/Prov.  │
└───────────────┴────────────────────┴────────────────┴───────────────┘
```

---

## 🔧 Hardware Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                    SMART PLUG AI — HARDWARE                          │
├─────────────────────────────────────────────────────────────────────┤
│  POWER SECTION                                                        │
│  AC Input 230V 50Hz → Fuse (3A Quick) → MOV 275V                    │
│  Relay 10A+ → Current Sensor SCT-013-030 → AC Out Socket            │
│  Burden Resistor 33Ω 1W                                              │
├─────────────────────────────────────────────────────────────────────┤
│  CONTROL SECTION                                                      │
│  ┌──────────────────────────────────────────────────────────────┐   │
│  │  ESP32-S3-WROOM-1-N8                                          │   │
│  │    Dual-core Xtensa LX7 @ 240MHz                              │   │
│  │    512KB SRAM, 8MB PSRAM                                      │   │
│  │    Hardware crypto acceleration (RSA, AES, SHA, ECC)          │   │
│  │    Secure Boot V2 + Flash Encryption (AES-256-XTS)            │   │
│  ├────────────────────┬─────────────────────────────────────────┤   │
│  │  ATECC608A          │  MAX6316 Tamper Watchdog                │   │
│  │  Secure Element     │  Physical security monitor              │   │
│  │  ECDSA P256 keys    │  Reset on tamper event                  │   │
│  │  Hardware RNG       │  Battery-backed detection               │   │
│  ├────────────────────┼─────────────────────────────────────────┤   │
│  │  24C256 EEPROM      │  SCT-013-030 Current Sensor             │   │
│  │  32KB encrypted     │  Non-invasive, 30A/1V                   │   │
│  │  config storage     │  3.5mm jack output                      │   │
│  └────────────────────┴─────────────────────────────────────────┘   │
├─────────────────────────────────────────────────────────────────────┤
│  POWER SUPPLY: 5V/2A Phone Charger → 3.3V LDO → ESP32 & ICs         │
├─────────────────────────────────────────────────────────────────────┤
│  PHYSICAL SECURITY                                                    │
│  - Anti-tamper screws (security bit required)                        │
│  - Tamper-evident seals (void if removed)                            │
│  - Enclosure tamper switch connected to MAX6316                      │
│  - Potting compound over sensitive components                        │
└─────────────────────────────────────────────────────────────────────┘
```

---

## 💻 Firmware Architecture

5-layer firmware stack running on ESP32-S3:

```
┌─────────────────────────────────────────────────────────────────────┐
│  LAYER 5: APPLICATION                                                │
│  ML Inference · Anomaly Detection · Load Shedding Prediction        │
│  Energy Optimisation · User Commands                                 │
├─────────────────────────────────────────────────────────────────────┤
│  LAYER 4: SERVICES                                                   │
│  MQTT Client (TLS 1.3) · Secure OTA · Config Manager (encrypted)    │
│  NTP over TLS · Logger                                               │
├─────────────────────────────────────────────────────────────────────┤
│  LAYER 3: SECURITY                                                   │
│  ATECC608A Driver                                                    │
│    - ECDSA signing/verification                                      │
│    - Key storage (slots 0–15)                                        │
│    - Hardware RNG                                                    │
│  Secure Boot · Flash Encryption · TLS 1.3 · Tamper Detection        │
├─────────────────────────────────────────────────────────────────────┤
│  LAYER 2: ML ENGINE                                                  │
│  TFLite Micro · Quantized 128KB model                               │
│  Feature Extractor: sliding window · FFT · statistical features     │
│  Inference Scheduler: every 10 seconds                               │
├─────────────────────────────────────────────────────────────────────┤
│  LAYER 1: HARDWARE ABSTRACTION                                       │
│  ADC Driver · Relay Controller · I2C Driver · GPIO Manager          │
│  Watchdog Interface                                                  │
└─────────────────────────────────────────────────────────────────────┘
```

**Technology**: ESP-IDF v5.0+, TFLite Micro, ESP-MQTT, ATECC608A driver (I2C @ 0x60)

**ML Model**: SA-NILM-Ensemble v1.0 — CNN-LSTM, 91% accuracy, 128KB quantized, 12ms inference on ESP32-S3

---

## ☁️ Backend Architecture

```
┌──────────────────────────────────────────────────────────────────────┐
│  DEVICES                                                              │
│  ESP32-S3 Smart Plugs (MQTT 5.0 / TLS 1.3 / Port 8883)              │
└────────────────────────────┬─────────────────────────────────────────┘
                             │ mTLS + ECDSA client certs
                             ▼
┌──────────────────────────────────────────────────────────────────────┐
│  MQTT BROKER (TLS 1.3)   →   MESSAGE QUEUE (RabbitMQ / Kafka)        │
└────────────────────────────┬─────────────────────────────────────────┘
                             │
                             ▼
┌──────────────────────────────────────────────────────────────────────┐
│  PROCESSING PIPELINE                                                  │
│  Data Validator → ML Inferencer → Anomaly Detector                   │
└──────────┬──────────────────────────────────────────────────────────-┘
           │
           ▼
┌──────────────────────────────────────────────────────────────────────┐
│  STORAGE                                                              │
│  InfluxDB (time-series) · PostgreSQL (users/profiles) · Redis (cache)│
└──────────────────────────────────────────────────────────────────────┘
           │
           ▼
┌──────────────────────────────────────────────────────────────────────┐
│  API LAYER                                                            │
│  REST API (FastAPI) · WebSocket (real-time) · Admin API (RBAC/audit) │
└──────────────────────────────────────────────────────────────────────┘
           │
           ▼
┌──────────────────────────────────────────────────────────────────────┐
│  CLIENTS                                                              │
│  Mobile (Flutter) · Web (React) · Partner API (B2B)                  │
└──────────────────────────────────────────────────────────────────────┘
```

**Technology stack**:
- **API**: FastAPI (Python 3.10+) with Pydantic validation, RBAC, rate limiting
- **Broker**: Eclipse Mosquitto 2.0+ with TLS 1.3 and ACLs
- **Time-series DB**: InfluxDB (1-second granularity, automatic downsampling)
- **Relational DB**: PostgreSQL (users, devices, schedules, audit logs)
- **Cache**: Redis (sessions, rate limiting, real-time telemetry cache)
- **Container**: Docker + Kubernetes

---

## 📱 Mobile Architecture

4-layer Flutter application:

```
┌─────────────────────────────────────────────────────────────────────┐
│  LAYER 4: UI                                                         │
│  Dashboard · Device List · Device Detail · Insights                  │
│  Settings · Auth Flow                                                │
├─────────────────────────────────────────────────────────────────────┤
│  LAYER 3: BLoC / PROVIDER                                            │
│  AuthBloc · DeviceBloc · InsightsBloc · SettingsBloc · SyncBloc      │
├─────────────────────────────────────────────────────────────────────┤
│  LAYER 2: SERVICES                                                   │
│  ApiService (encrypted) · WebSocketService                           │
│  SecureStorage (biometric) · HiveService (encrypted)                 │
│  LocalAuthService · NotificationService                              │
├─────────────────────────────────────────────────────────────────────┤
│  LAYER 1: MODELS                                                     │
│  User · Device · PowerReading · ApplianceEvent · Insight · Alert     │
└─────────────────────────────────────────────────────────────────────┘
```

**Technology**: Flutter 3.x, Dart 3.x, BLoC pattern, Hive (encrypted local storage), TLS 1.3 for all API calls, biometric authentication

See [app/README.md](../app/README.md) for full mobile app documentation.

---

## 🔄 Data Flow

### Telemetry Ingestion

```
ESP32 Device
  │ (1 Hz ADC sampling, 10s ML inference window)
  │ Encrypt payload (AES-256-GCM)
  │ Sign with ATECC608A (ECDSA)
  ▼
MQTT Broker (TLS 1.3, port 8883)
  │ Topic: smartplug/{device_id}/telemetry
  ▼
Processing Pipeline
  │ Validate signature → run ML inference → detect anomalies
  ▼
Storage
  │ InfluxDB (raw readings) + PostgreSQL (events) + Redis (cache)
  ▼
Clients
  Mobile App / Web Dashboard (real-time via WebSocket)
```

### Device Command Flow

```
Mobile App
  │ User taps "Turn On"
  │ POST /devices/{id}/command  (JWT auth)
  ▼
FastAPI Backend
  │ Validate JWT → check ownership → sign command (ECDSA)
  ▼
MQTT Broker
  │ Topic: smartplug/{device_id}/control
  ▼
ESP32 Device
  │ Verify command signature → execute relay → publish status
  ▼
Mobile App
  UI updates to reflect new state (via WebSocket)
```

---

## 🔒 Security Architecture

See [docs/SECURITY.md](SECURITY.md) for the complete security architecture including:
- ATECC608A key slot configuration
- 8-step secure boot chain
- MQTT TLS 1.3 configuration
- End-to-end encryption design

---

*Architecture is a living document and will evolve as the system grows.*
