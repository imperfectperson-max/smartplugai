# Smart Plug AI Mobile App - Setup Guide

## Pre-Hardware Development Status ✅

The mobile app is ready for development with **mock data and UI mockups**. All security features are designed in and ready for backend integration.

## What's Implemented

### ✅ Complete Features (Mock)
- **Authentication**: Login/Signup screens with 2FA mockup
- **Dashboard**: Real-time device monitoring with security indicators
- **Device Management**: Device list, detail view, and control mockups
- **QR Pairing**: Secure device pairing screen placeholder
- **Settings**: User profile and security settings
- **Security Features**: 
  - Encryption badges on all data display
  - Device attestation status indicators
  - Tamper detection alerts
  - 2FA integration mockup

### 🎨 UI Components
- Device cards with power metrics and security status
- 24-hour power consumption charts (fl_chart)
- Security status indicators
- Responsive layouts

## Setup Instructions

### Prerequisites
- Flutter SDK 3.10.0+
- Dart 3.0.0+
- Android Studio / Xcode (for platform builds)
- VS Code with Flutter extensions (recommended)

### Installation

1. **Install dependencies**:
   ```bash
   cd app
   flutter pub get
   ```

2. **Run the app**:
   ```bash
   # iOS Simulator
   flutter run -d "iPhone 14 Pro"
   
   # Android Emulator
   flutter run -d emulator-5554
   
   # Physical device
   flutter run
   ```

3. **Mock Login**:
   - Email: any valid email format
   - Password: any 6+ character password
   - 2FA Code: any 6-digit code

## Project Structure

```
app/
├── lib/
│   ├── main.dart              # App entry point
│   ├── models/                # Data models (Device, User, PowerReading)
│   ├── screens/               # UI screens
│   │   ├── auth/             # Login, Signup
│   │   ├── home_screen.dart  # Main dashboard
│   │   ├── device_detail_screen.dart
│   │   ├── pairing/          # QR scanner
│   │   └── settings_screen.dart
│   ├── widgets/              # Reusable components
│   │   └── device_card.dart
│   ├── services/             # Business logic
│   │   ├── auth_service.dart # Mock auth with 2FA
│   │   └── device_service.dart # Mock device management
│   └── theme/                # App theming
│       └── app_theme.dart
├── pubspec.yaml              # Dependencies
└── README.md                 # This file
```

## Next Steps for Production

When hardware and backend are ready:

1. **Replace Mock Services**:
   - Integrate Firebase Auth or Auth0 with real 2FA
   - Connect to backend API over HTTPS
   - Implement MQTT over TLS 1.3 for real-time data

2. **Security Integration**:
   - Add certificate pinning for API connections
   - Implement encrypted local storage
   - Add biometric authentication
   - Integrate device attestation checks

3. **Add Real QR Scanner**:
   - The `mobile_scanner` package is already in dependencies
   - Implement challenge-response authentication flow

4. **Testing**:
   - Run `flutter test` for unit tests
   - Add integration tests for authentication flow
   - Test on real devices with hardware

## Security Features (Designed)

All UI elements reflect the security-first approach:

- **Encryption Indicators**: Blue badges showing data is encrypted
- **Attestation Status**: Green checkmarks for verified devices
- **Tamper Alerts**: Red warning badges for tamper detection
- **2FA Integration**: Built into login flow
- **Certificate Status**: Shows certificate expiry dates

## Development Notes

- Currently using **mock data** - all services return simulated responses
- Device power readings update every 2 seconds via streams
- All security features are UI mockups pending backend integration
- State management uses Provider pattern
- Theme supports light/dark mode (system default)

## Troubleshooting

**Flutter not found**:
```bash
# Install Flutter: https://flutter.dev/docs/get-started/install
```

**Dependencies not installing**:
```bash
flutter clean
flutter pub get
```

**Build errors**:
```bash
flutter doctor
# Fix any issues reported
```

## Resources

- [Flutter Documentation](https://flutter.dev/docs)
- [Project SECURITY.md](../docs/SECURITY.md) - Security architecture
- [Project ROADMAP.md](../docs/ROADMAP.md) - Development timeline
