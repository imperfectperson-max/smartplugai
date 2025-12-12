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

## 🚀 Getting Started

### Prerequisites

- Flutter SDK 3.10.0 or higher
- Dart 3.0.0 or higher
- Android Studio / Xcode (for platform-specific builds)
- VS Code with Flutter extensions (recommended)

### Installation

1. **Clone the repository** (if not already done):
   ```bash
   git clone https://github.com/imperfectperson-max/smartplugai.git
   cd smartplugai/app
   ```

2. **Install Flutter dependencies**:
   ```bash
   flutter pub get
   ```

3. **Verify Flutter installation**:
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

## 🏗️ Project Structure

```
app/
├── lib/
│   ├── main.dart              # App entry point
│   ├── models/                # Data models
│   │   ├── device.dart
│   │   ├── power_reading.dart
│   │   └── user.dart
│   ├── screens/               # UI screens
│   │   ├── home_screen.dart
│   │   ├── device_detail_screen.dart
│   │   ├── auth/
│   │   │   ├── login_screen.dart
│   │   │   └── signup_screen.dart
│   │   ├── pairing/
│   │   │   └── qr_scanner_screen.dart
│   │   └── settings_screen.dart
│   ├── widgets/               # Reusable widgets
│   │   ├── power_gauge.dart
│   │   ├── device_card.dart
│   │   └── power_chart.dart
│   ├── services/              # Business logic & API
│   │   ├── auth_service.dart
│   │   ├── mqtt_service.dart
│   │   ├── device_service.dart
│   │   └── notification_service.dart
│   ├── utils/                 # Utilities & helpers
│   │   ├── constants.dart
│   │   └── formatters.dart
│   └── theme/                 # App theming
│       └── app_theme.dart
├── test/                      # Unit & widget tests
├── integration_test/          # Integration tests
├── assets/                    # Images, fonts, etc.
├── pubspec.yaml              # Dependencies
└── README.md                 # This file
```

## 🎨 Key Features to Implement

### Phase 1: UI Mockups (Before Hardware Ready) - Mobile App V0.1

These can be built and tested with mock/placeholder data:

#### 1. Authentication Flow with 2FA
- [ ] **Login screen** with email/password and 2FA support (Firebase/Auth0 integration planned)
- [ ] **Sign-up screen** with basic validation
- [ ] **Mock auth service** that simulates login with 2FA flow
- [ ] **Onboarding screens** explaining app features and security model

**Suggested Mock Implementation**:
```dart
class MockAuthService {
  Future<bool> login(String email, String password) async {
    await Future.delayed(Duration(seconds: 1));
    return email.isNotEmpty && password.length >= 6;
  }
}
```

#### 2. Real-Time Power Monitoring UI (Encrypted Display)
- [ ] **Dashboard screen** showing list of devices with encryption status indicator
- [ ] **Device card widget** displaying device name, status, current power (encrypted in transit)
- [ ] **Power gauge widget** with animated needle/circular indicator
- [ ] **Real-time chart** using fl_chart showing 24-hour encrypted historical data
- [ ] **Mock data generator** to simulate live encrypted power readings with security status

**Suggested Mock Implementation**:
```dart
Stream<PowerReading> getMockPowerStream() async* {
  while (true) {
    yield PowerReading(
      voltage: 120.0 + Random().nextDouble() * 2,
      current: 0.5 + Random().nextDouble() * 2.5,
      power: 60.0 + Random().nextDouble() * 300,
      timestamp: DateTime.now(),
    );
    await Future.delayed(Duration(seconds: 1));
  }
}
```

#### 3. Secure QR Code Pairing Flow with Challenge-Response
- [ ] **QR scanner screen** using mobile_scanner for secure device pairing
- [ ] **Challenge-response authentication** flow after QR scan
- [ ] **Pairing instructions screen** with security explanations
- [ ] **Success/failure feedback** with security validation status
- [ ] **Mock device addition** with simulated challenge-response (adds device to local list)

#### 4. Device Management with Signed Commands
- [ ] **Device list view** with search/filter and security status
- [ ] **Device detail screen** with controls (signed ON/OFF commands) and settings
- [ ] **Device settings** (rename, icon, notifications, attestation status)
- [ ] **Delete device** confirmation with security considerations

#### 5. Settings & Profile with Security
- [ ] **User profile screen** with 2FA management
- [ ] **App settings** (theme, units, notifications, security preferences)
- [ ] **Security dashboard** showing encryption status and device attestation
- [ ] **About screen** with version info and security certifications

### Phase 2: Backend Integration (After Hardware Ready) - Mobile App V2

Once the hardware and backend are functional:

- [ ] Replace mock auth with Firebase/Auth0 authentication with 2FA enabled
- [ ] Implement MQTT client with TLS 1.3 to receive real encrypted device data
- [ ] Add WebSocket support for real-time encrypted updates
- [ ] Implement device control with signed commands (relay on/off via API)
- [ ] Add push notifications for security alerts and tamper detection
- [ ] Implement data persistence with local database (sqflite/hive) using encryption
- [ ] Add offline mode support with encrypted local storage
- [ ] Implement challenge-response pairing with real devices
- [ ] Display device attestation status and security health

## 📦 Recommended Packages

Add these to `pubspec.yaml` as you implement features:

```yaml
dependencies:
  # State management
  provider: ^6.1.0  # or riverpod, bloc, get
  
  # Networking
  http: ^1.1.0
  mqtt_client: ^10.0.0  # For MQTT communication
  
  # Charts & Visualization
  fl_chart: ^0.65.0
  
  # QR Code
  mobile_scanner: ^3.5.0
  
  # Local Storage
  shared_preferences: ^2.2.0
  sqflite: ^2.3.0  # or hive
  
  # Authentication (when ready)
  firebase_auth: ^4.15.0
  firebase_core: ^2.24.0
  
  # Notifications
  flutter_local_notifications: ^16.0.0
  
  # UI Utilities
  google_fonts: ^6.1.0
  flutter_svg: ^2.0.0
  intl: ^0.18.0  # For formatting dates/numbers
```

## 🧪 Testing

### Run Unit Tests
```bash
flutter test
```

### Run Integration Tests
```bash
flutter test integration_test/app_test.dart
```

### Run with Coverage
```bash
flutter test --coverage
genhtml coverage/lcov.info -o coverage/html
open coverage/html/index.html
```

## 🎯 First Tasks (Suggested)

Start here if you're new to the project:

1. **Set up Flutter project** (if not already initialized):
   ```bash
   flutter create --org com.smartplugai --project-name smartplug_app .
   ```

2. **Create basic app structure**:
   - Set up folder structure (models, screens, services, widgets)
   - Configure app theme and colors
   - Set up routing (use go_router or named routes)

3. **Build authentication mockup**:
   - Login screen UI
   - Sign-up screen UI
   - Mock authentication service
   - Navigation after login

4. **Create power monitoring UI**:
   - Device list screen
   - Device card widget with power display
   - Mock data service
   - Animated power gauge widget

5. **Implement QR scanner**:
   - Add mobile_scanner package
   - Build scanner screen
   - Handle QR code data (device ID)
   - Add device to local list

## 🐛 Debugging

### Enable Flutter DevTools
```bash
flutter pub global activate devtools
flutter pub global run devtools
```

### Debug on Physical Device

**iOS**:
```bash
flutter run --debug
```

**Android** with USB debugging enabled:
```bash
flutter run --debug
```

## 🚀 Building for Production

### Android APK
```bash
flutter build apk --release
# Output: build/app/outputs/flutter-apk/app-release.apk
```

### iOS IPA
```bash
flutter build ios --release
# Then use Xcode to archive and distribute
```

## 📝 Notes for Developers

- **Mock everything initially**: Don't wait for hardware. Build the UI with mock data streams.
- **Use Provider/Riverpod**: Choose a state management solution early and stick with it.
- **Test on real devices**: Simulators don't reflect real performance.
- **Design responsive**: Support different screen sizes (phones, tablets).
- **Plan for offline mode**: Users should see cached data when offline.
- **Handle errors gracefully**: Network issues, Bluetooth problems, device disconnections.

## 🔗 Useful Resources

- [Flutter Documentation](https://flutter.dev/docs)
- [Dart Language Tour](https://dart.dev/guides/language/language-tour)
- [Flutter Cookbook](https://flutter.dev/docs/cookbook)
- [MQTT in Flutter](https://pub.dev/packages/mqtt_client)
- [fl_chart Examples](https://github.com/imaNNeo/fl_chart/tree/master/example)

## 🤝 Contributing

See the main [CONTRIBUTING.md](../CONTRIBUTING.md) for general guidelines.

For app-specific contributions:
- Follow Flutter/Dart style guide
- Run `flutter analyze` before committing
- Write widget tests for new UI components
- Update this README if adding new features

---

**Let's build an amazing mobile experience for Smart Plug AI!** 📱⚡

For comprehensive security architecture, encryption details, and Phase 1 (12 weeks) timeline with security milestones, see [docs/SECURITY.md](../docs/SECURITY.md) and [docs/ROADMAP.md](../docs/ROADMAP.md).
