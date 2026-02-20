# 📱 Smart Plug AI — Mobile App

Flutter-based cross-platform mobile application for the Smart Plug AI platform — South Africa's first AI-powered energy intelligence platform.

## 📋 Table of Contents

- [Overview](#-overview)
- [Architecture](#-architecture)
- [Features](#-features)
- [Getting Started](#-getting-started)
- [Project Structure](#️-project-structure)
- [State Management](#-state-management)
- [Security Features](#-security-features)
- [Testing](#-testing)
- [Building for Production](#-building-for-production)
- [Contributing](#-contributing)

---

## 🎯 Overview

The Smart Plug AI mobile app gives users:
- **Appliance insights**: See exactly which appliance is running (91% AI accuracy)
- **Real-time monitoring**: Live power consumption with end-to-end encryption
- **Load shedding intelligence**: Predictions and impact tracking
- **Actionable savings**: Personalised recommendations to cut electricity bills by 30%
- **Secure device control**: ON/OFF relay control with ECDSA-signed commands
- **Multi-device management**: Manage all smart plugs from one dashboard

## 📱 Supported Platforms

- **iOS**: 13.0 and above
- **Android**: 5.0 (API level 21) and above

---

## 🏗️ Architecture

4-layer Flutter architecture:

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

**State management**: BLoC pattern (flutter_bloc) for complex state, Provider for simple global state.

---

## ✨ Features

### Phase 0 (Months 1–8): Data Collection UI

- ✅ Authentication (email/password + 2FA)
- ✅ Dashboard with device list
- ✅ Real-time power monitoring (mock data)
- ✅ Device detail screen with 24-hour chart
- ✅ QR pairing flow mockup
- ✅ Security status indicators
- ✅ Settings and profile screens

### Phase 2 (Months 13–18): Full Integration

- [ ] Live data from ESP32-S3 devices via MQTT/TLS 1.3
- [ ] AI appliance recognition display (91% accuracy)
- [ ] Load shedding predictions and tracking
- [ ] Signed relay control commands
- [ ] Push notifications for anomalies and alerts
- [ ] Encrypted offline mode (Hive)
- [ ] Biometric authentication (Face ID / fingerprint)
- [ ] Device attestation status display

### Phase 3 (Months 19–24): Polish

- [ ] Insights dashboard with savings recommendations
- [ ] Historical analytics and trends
- [ ] Energy cost tracking
- [ ] Export reports

---

## 🚀 Getting Started

### Prerequisites

- Flutter SDK 3.10.0 or higher
- Dart 3.0.0 or higher
- Android Studio / Xcode (for platform builds)
- VS Code with Flutter extensions (recommended)

### Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/imperfectperson-max/smartplugai.git
   cd smartplugai/app
   ```

2. **Install dependencies**:
   ```bash
   flutter pub get
   ```

3. **Verify setup**:
   ```bash
   flutter doctor
   ```

4. **Run the app**:
   ```bash
   # iOS Simulator
   flutter run -d "iPhone 14 Pro"

   # Android Emulator
   flutter run -d emulator-5554

   # Physical device
   flutter run
   ```

**Mock login**: any email + password (6+ chars) + any 6-digit 2FA code

---

## 🗂️ Project Structure

```
app/
├── lib/
│   ├── main.dart                   # App entry point (BLoC/Provider setup)
│   ├── models/                     # Data models (Layer 1)
│   │   ├── user.dart
│   │   ├── device.dart
│   │   ├── power_reading.dart
│   │   ├── appliance_event.dart
│   │   ├── insight.dart
│   │   └── alert.dart
│   ├── blocs/                      # BLoC state management (Layer 3)
│   │   ├── auth/
│   │   ├── device/
│   │   ├── insights/
│   │   ├── settings/
│   │   └── sync/
│   ├── services/                   # Services layer (Layer 2)
│   │   ├── api_service.dart        # Encrypted REST API calls
│   │   ├── websocket_service.dart  # Real-time updates
│   │   ├── secure_storage.dart     # Biometric-protected storage
│   │   ├── hive_service.dart       # Encrypted local database
│   │   ├── local_auth_service.dart # Biometric authentication
│   │   └── notification_service.dart
│   ├── screens/                    # UI screens (Layer 4)
│   │   ├── auth/
│   │   │   ├── login_screen.dart
│   │   │   └── signup_screen.dart
│   │   ├── home_screen.dart
│   │   ├── device_detail_screen.dart
│   │   ├── insights_screen.dart
│   │   ├── pairing/
│   │   │   └── qr_scanner_screen.dart
│   │   └── settings_screen.dart
│   ├── widgets/                    # Reusable widgets
│   │   ├── power_gauge.dart
│   │   ├── device_card.dart
│   │   ├── power_chart.dart
│   │   └── appliance_badge.dart
│   ├── utils/
│   │   ├── constants.dart
│   │   └── formatters.dart
│   └── theme/
│       └── app_theme.dart
├── test/                           # Unit & widget tests
├── integration_test/               # Integration tests
├── assets/
├── pubspec.yaml
└── README.md
```

---

## 🧩 State Management

The app uses **BLoC** (Business Logic Component) pattern:

| BLoC | Manages |
|------|---------|
| `AuthBloc` | Login, logout, 2FA, session refresh |
| `DeviceBloc` | Device list, telemetry, relay control |
| `InsightsBloc` | Appliance recognition results, savings recommendations |
| `SettingsBloc` | User preferences, notifications, security settings |
| `SyncBloc` | Background sync, offline/online state |

---

## 🔒 Security Features

| Feature | Implementation |
|---------|---------------|
| Authentication | Firebase Auth / Auth0 with 2FA (TOTP) |
| Biometrics | Face ID, Touch ID, fingerprint for app unlock |
| Cert pinning | Prevents MITM on API connections |
| Encrypted storage | Hive with AES encryption + platform keychain |
| Secure transport | TLS 1.3 for all API and WebSocket connections |
| E2E decryption | App decrypts device data locally (cloud never sees plaintext) |
| Session timeout | Auto-logout after 30 minutes of inactivity |
| No sensitive logs | Zero PII in crash reports or debug output |

See [docs/SECURITY.md](../docs/SECURITY.md) for the full security architecture.

---

## 🧪 Testing

```bash
# Unit tests
flutter test

# Integration tests
flutter test integration_test/app_test.dart

# With coverage
flutter test --coverage
genhtml coverage/lcov.info -o coverage/html
```

---

## 🏭 Building for Production

```bash
# Android APK
flutter build apk --release

# Android App Bundle (Play Store)
flutter build appbundle --release

# iOS
flutter build ios --release
# Then archive and distribute via Xcode
```

---

## 🤝 Contributing

See [CONTRIBUTING.md](../CONTRIBUTING.md) for general guidelines.

For app-specific contributions:
- Follow the BLoC architecture pattern (add new BLoCs in `lib/blocs/`)
- Run `flutter analyze` before committing
- Write widget tests for new UI components
- Update this README if adding new features or screens

---

*Part of the Smart Plug AI platform — see [docs/ROADMAP.md](../docs/ROADMAP.md) for the full 24-month roadmap.*
