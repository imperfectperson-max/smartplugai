# 🔒 Smart Plug AI — Security Architecture

Bank-grade security across every layer: hardware, firmware, transport, and cloud.

## 📋 Table of Contents

- [Security Overview](#-security-overview)
- [Hardware Security](#-hardware-security)
- [ATECC608A Key Configuration](#-atecc608a-key-configuration)
- [Secure Boot Chain](#-secure-boot-chain)
- [MQTT TLS Configuration](#-mqtt-tls-configuration)
- [End-to-End Encryption](#-end-to-end-encryption)
- [Firmware Security](#-firmware-security)
- [Backend Security](#️-backend-security)
- [Mobile App Security](#-mobile-app-security)
- [Physical Security](#️-physical-security)
- [Compliance](#-compliance)

---

## 🛡️ Security Overview

Smart Plug AI implements **defence-in-depth** — multiple independent security layers that must all be compromised simultaneously for an attack to succeed.

### Core Security Principles

| Principle | Implementation |
|-----------|---------------|
| Zero Trust | Every device, user, and command is verified before execution |
| Least Privilege | Each component has the minimum permissions needed |
| Secure by Design | Security built in from Day 1, not bolted on later |
| Defence in Depth | Hardware + Firmware + Transport + Cloud + Physical |
| Privacy by Default | Cloud never sees plaintext device data |

---

## 🔐 Hardware Security

### ESP32-S3-WROOM-1-N8

| Feature | Detail |
|---------|--------|
| Secure Boot V2 | RSA-PSS 3072-bit firmware signature verification |
| Flash Encryption | AES-256-XTS — all flash contents encrypted at rest |
| Hardware Crypto | RSA, AES, SHA, ECC acceleration — no software fallbacks |
| eFuse Protection | One-time programmable keys, irreversible lock |
| True RNG | Hardware random number generator for all crypto operations |
| Memory Protection | MPU enforces secure/non-secure memory boundaries |

### ATECC608A Secure Element

The ATECC608A is the cryptographic heart of the device. Private keys are generated on-chip and **never leave the chip** — not even during firmware updates.

| Feature | Detail |
|---------|--------|
| Key Storage | ECDSA P256 — private keys tamper-resistant inside chip |
| On-chip RNG | True hardware RNG for key generation |
| Hardware SHA-256 | Fast cryptographic hashing |
| Monotonic Counters | 32 counters to prevent replay attacks |
| Config Lock | Irreversible security configuration lock |
| Physical Protection | Resistant to DPA and SPA side-channel attacks |
| Interface | I2C at address 0x60, up to 1 MHz |

### MAX6316 Tamper Watchdog

- Monitors ESP32 activity — resets if firmware hangs
- Monitors physical enclosure tamper switch
- Optional 3V coin-cell backup for tamper detection when powered off
- Configurable timeout (1.6s to 102.4s)
- Active-low reset output to ESP32

---

## 🗝️ ATECC608A Key Configuration

The ATECC608A has 16 key slots. The Smart Plug AI allocation:

```
┌────────┬─────────────────────────┬──────────────────────────────────┐
│  Slot  │  Purpose                │  Notes                           │
├────────┼─────────────────────────┼──────────────────────────────────┤
│   0    │  Device Private Key     │  ECDSA P256 — never readable     │
│   1    │  Device Certificate     │  X.509, rotated every 90 days    │
│   2    │  Cloud Public Key       │  Verify server commands          │
│   3    │  Encryption Key         │  AES-256 for payload encryption  │
│   4    │  Secure Boot Key        │  Firmware signature verification │
│  5–15  │  Application use        │  OTA keys, session keys, etc.    │
└────────┴─────────────────────────┴──────────────────────────────────┘
```

**Security configuration**:
```
SlotConfig[0]: PrivWrite=Encrypt, ReadKey=0, ECC=1, Lockable=1
SlotConfig[1]: IsSecret=0 (certificate, publicly readable), ECC=1
SlotConfig[2]: IsSecret=0 (cloud public key), ECC=1
SlotConfig[3]: PrivWrite=Encrypt, IsSecret=1, AES=1
SlotConfig[4]: PrivWrite=Encrypt, IsSecret=1, ECC=1, Lockable=1
KeyConfig[0]:  Private=1, PubInfo=1, KeyType=P256NIST, Lockable=1
LockConfig:    0x55 (configuration locked) → 0xAA (data locked)
```

> ⚠️ **IRREVERSIBLE**: Configuration lock and data zone lock cannot be undone. Test provisioning thoroughly before locking.

---

## 🔑 Secure Boot Chain

8-step verified boot process — if any step fails, the device halts:

```
┌─────────────────────────────────────────────────────────────────────┐
│  SECURE BOOT CHAIN                                                   │
├─────────────────────────────────────────────────────────────────────┤
│                                                                       │
│  Step 1: Hardware Reset                                              │
│          │                                                           │
│  Step 2: ROM Bootloader (immutable, in silicon)                      │
│          │                                                           │
│  Step 3: Verify Bootloader Signature (RSA)  ←── ATECC608A Slot 4    │
│          │  ✅ Pass → continue   ❌ Fail → HALT                      │
│          │                                                           │
│  Step 4: Load Stage 2 Bootloader                                     │
│          │                                                           │
│  Step 5: Verify Stage 2 Signature           ←── ATECC608A Slot 4    │
│          │  ✅ Pass → continue   ❌ Fail → HALT                      │
│          │                                                           │
│  Step 6: Load Application                                            │
│          │                                                           │
│  Step 7: Verify Application Signature       ←── ATECC608A Slot 4    │
│          │  ✅ Pass → continue   ❌ Fail → HALT                      │
│          │                                                           │
│  Step 8: Execute Application                                         │
│                                                                       │
├─────────────────────────────────────────────────────────────────────┤
│  ON ANY VERIFICATION FAILURE:                                        │
│  - Halt boot immediately                                             │
│  - Blink red LED (tamper indicator)                                  │
│  - Log tamper event to encrypted EEPROM (24C256)                    │
│  - Enter recovery mode (awaits authorised recovery firmware)         │
└─────────────────────────────────────────────────────────────────────┘
```

---

## 📡 MQTT TLS Configuration

```
Protocol:             MQTT 5.0 over TCP
Port:                 8883
TLS Version:          1.3 (TLS 1.2 disabled)
Cipher Suites:        TLS_AES_256_GCM_SHA384
                      TLS_CHACHA20_POLY1305_SHA256
Mutual TLS:           Yes (both sides present certificates)
Client Certificate:   X.509, ECDSA P256, stored in ATECC608A Slot 1
Server Certificate:   X.509, CA-signed, verified against pinned CA cert
Certificate Rotation: Every 90 days (automated via OTA)
Payload Encryption:   AES-256-GCM per message (in addition to TLS)
Sequence Numbers:     Yes (anti-replay protection)
Timestamp Window:     ±30 seconds (reject stale messages)
```

**MQTT topic ACLs** (per-device enforcement at broker):
```
smartplug/{device_id}/telemetry   — device publishes only
smartplug/{device_id}/control     — device subscribes only
smartplug/{device_id}/status      — device publishes only
smartplug/{device_id}/ota         — device subscribes only (signed)
```

---

## 🔏 End-to-End Encryption

Data is encrypted on the device before transmission and decrypted only on authorised client apps. **The cloud never sees plaintext device data.**

```
┌─────────────────────────────────────────────────────────────────────┐
│  E2E ENCRYPTION DESIGN                                               │
├─────────────────────────────────────────────────────────────────────┤
│                                                                       │
│  DEVICE SIDE                                                         │
│  ┌────────────────────────────────────────────┐                     │
│  │ 1. Generate AES-256-GCM session key        │                     │
│  │    (via ATECC608A hardware RNG)             │                     │
│  │ 2. Encrypt session key with user's          │                     │
│  │    RSA public key                           │                     │
│  │ 3. Generate 12-byte nonce (per message)     │                     │
│  │ 4. Encrypt payload: AES-256-GCM(key, nonce) │                     │
│  │ 5. Sign encrypted payload: ECDSA(slot 0)    │                     │
│  └────────────────────────────────────────────┘                     │
│                       │ (over MQTT TLS 1.3)                          │
│                       ▼                                              │
│  CLOUD (opaque — cannot decrypt)                                     │
│  ┌────────────────────────────────────────────┐                     │
│  │ Validates ECDSA signature only             │                     │
│  │ Stores encrypted blob + metadata           │                     │
│  │ Routes to authorised app session           │                     │
│  └────────────────────────────────────────────┘                     │
│                       │                                              │
│                       ▼                                              │
│  MOBILE APP (authorised)                                             │
│  ┌────────────────────────────────────────────┐                     │
│  │ 1. Decrypt session key with user's          │                     │
│  │    RSA private key (in device keychain)     │                     │
│  │ 2. Verify ECDSA signature                   │                     │
│  │ 3. Decrypt payload: AES-256-GCM(key, nonce) │                     │
│  │ 4. Display plaintext data                   │                     │
│  └────────────────────────────────────────────┘                     │
│                                                                       │
├─────────────────────────────────────────────────────────────────────┤
│  KEY PROPERTIES                                                       │
│  - AES-256-GCM session key: unique per device                        │
│  - Session key encrypted with user's RSA public key                  │
│  - Per-message nonce: 12 bytes (96-bit, never reused)                │
│  - Anti-replay: sequence numbers + 30-second timestamp window        │
└─────────────────────────────────────────────────────────────────────┘
```

---

## 💻 Firmware Security

| Feature | Implementation |
|---------|---------------|
| Secure OTA | Firmware signed with RSA-3072; ATECC608A verifies before flash |
| Encrypted NVS | Wi-Fi credentials and config in AES-encrypted NVS partition |
| Command signing | All commands verified with ECDSA (server public key in slot 2) |
| Replay prevention | Nonce + timestamp; reject commands older than 5 minutes |
| Watchdog | Hardware watchdog resets device if firmware hangs |
| Tamper response | Physical tamper → wipe sensitive data → enter recovery mode |

---

## 🖥️ Backend Security

| Feature | Implementation |
|---------|---------------|
| Authentication | JWT (RS256), OAuth2, 2FA/TOTP |
| Authorisation | RBAC (Admin, User, Viewer, Service, Auditor) |
| Transport | TLS 1.3 exclusively (HTTPS + MQTTS) |
| Input validation | Pydantic schemas on all endpoints |
| Rate limiting | Per-user and per-device limits (Redis-backed) |
| Audit logging | All actions logged with user, timestamp, resource |
| Field encryption | Sensitive DB fields encrypted (AES-256-GCM + KMS) |
| Command signing | ECDSA signatures on all device control commands |
| DDoS protection | Cloudflare / AWS Shield at network edge |

---

## 📱 Mobile App Security

| Feature | Implementation |
|---------|---------------|
| Authentication | Firebase Auth / Auth0 with 2FA/TOTP |
| Biometrics | Face ID, Touch ID, fingerprint for app unlock |
| Session | JWT (30-minute expiry), biometric re-auth |
| Cert pinning | Prevents MITM attacks on API connections |
| Local storage | Platform keychain (iOS) / keystore (Android) |
| Encrypted cache | AES-encrypted Hive database for offline data |
| No sensitive logs | Zero PII in crash reports or debug logs |
| Timeout | Auto-logout after 30 minutes of inactivity |

---

## 🏠 Physical Security

```
┌─────────────────────────────────────────────────────────────────────┐
│  PHYSICAL SECURITY LAYERS                                            │
├─────────────────────────────────────────────────────────────────────┤
│  1. Anti-tamper screws (security bit required to open)              │
│  2. Tamper-evident seals (visually void if removed)                 │
│  3. Enclosure tamper switch → triggers MAX6316 → secure wipe       │
│  4. Potting compound over ATECC608A and key components              │
│  5. PCB security zones: isolated AC/DC circuits                     │
└─────────────────────────────────────────────────────────────────────┘
```

**Tamper response sequence**:
1. Enclosure opened → tamper switch triggers MAX6316
2. MAX6316 asserts RESET to ESP32
3. Firmware detects tamper before any application code runs
4. Secure wipe of sensitive NVS data
5. Tamper event logged to 24C256 EEPROM (survives power loss)
6. Device enters locked recovery mode — requires authorised recovery

---

## ✅ Compliance

| Standard | Status | Target |
|----------|--------|--------|
| SABS | Planned | Phase 2 |
| ICASA | Planned | Phase 2 |
| POPIA | Designed-in | Phase 2 |
| SOC2 Type I | Planned | Phase 3 |
| ISO 27001 | Future | Year 3+ |

---

*For security vulnerabilities, contact: security@smartplugai.com*  
*Bug bounty rewards: R5,000 – R50,000 for critical findings*
