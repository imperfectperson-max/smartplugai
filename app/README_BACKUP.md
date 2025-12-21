# Smart Plug AI Mobile App

Flutter-based cross-platform mobile application for monitoring and controlling smart plugs in real-time.

## 🎯 Overview

The mobile app provides users with:
- **Real-time power monitoring**: Live current and power consumption with end-to-end encryption
- **Secure device control**: Turn devices on/off remotely with signed commands
- **Historical data**: View encrypted 24-hour power usage trends and analytics
- **Smart notifications**: Alerts for high power usage or anomalies
- **Secure QR code pairing**: Easy device setup with challenge-response authentication
- **Multi-device management**: Control multiple smart plugs with secure 2FA authentication
- **Security features**: Firebase/Auth0 authentication with 2FA, encrypted telemetry in transit, device attestation status display

## 📱 Supported Platforms

- **iOS**: 13.0 and above
- **Android**: 5.0 (API level 21) and above

## 🔒 Security Features

Smart Plug AI Mobile App implements **bank-grade security** from Day 1:

### Authentication & Authorization
- **Firebase Auth / Auth0** integration with 2FA/MFA support
- **Time-based OTP (TOTP)** for second factor authentication
- **Secure session management** with JWT tokens (30-minute expiry)
- **Biometric authentication** (Face ID, Touch ID, fingerprint) for app unlock

### Secure Device Pairing
- **QR code scanning** with challenge-response authentication
- **Device verification** using server-signed challenges
- **Certificate-based device identity** validation
- **Tamper detection status** display during pairing

### Encrypted Communications
- **TLS 1.3** for all API and WebSocket connections
- **Certificate pinning** to prevent man-in-the-middle attacks
- **End-to-end encryption** for sensitive telemetry data (AES-256-GCM)
- **MQTT over TLS** with client certificates (production)

### Data Protection
- **Secure local storage** using platform keychain (iOS) / keystore (Android)
- **Encrypted telemetry cache** for offline viewing
- **No sensitive data** in logs or crash reports
- **Automatic session timeout** after 30 minutes of inactivity

### Device Security Monitoring
- **Device attestation status** indicator (firmware integrity)
- **Tamper alert notifications** (MAX6316 watchdog triggers)
- **Security health score** display (secure boot, flash encryption status)
- **Certificate expiry warnings** for devices

For comprehensive security architecture, see [docs/SECURITY.md](../docs/SECURITY.md).

## 🚀 Getting Started

### Prerequisites

- Flutter SDK 3.10.0 or higher
- Dart 3.0.0 or higher
- Android Studio / Xcode (for platform-specific builds)
- VS Code with Flutter extensions (recommended)

**Security Prerequisites**:
- **Firebase project** with Authentication enabled (or Auth0 account)
- **2FA setup** in Firebase/Auth0 (TOTP or SMS)
- **TLS certificates** for certificate pinning (production)
- **MQTT broker** with TLS 1.3 and client certificate support (production)

### Installation

1. **Clone the repository** (if not already done):
   ```bash
