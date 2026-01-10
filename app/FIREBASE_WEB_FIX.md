# Firebase Web Build Fix

## Problem

When running the Flutter app on web (`flutter run -d chrome`), the build failed with compilation errors originating from `package:firebase_auth_web` (version 5.8.13). The errors indicated missing JS interop symbols including:

- `PromiseJsImpl`
- `core_interop.dartify`
- `core_interop.jsify`
- Multiple undefined `handleThenable` references

## Root Cause

The issue was caused by outdated Firebase package versions:
- `firebase_auth: ^4.15.0`
- `firebase_core: ^2.24.0`

These older versions had transitive dependencies on `firebase_auth_web` v5.8.13, which uses deprecated JS interop APIs that are incompatible with newer Flutter web builds. The JS interop layer underwent significant changes in recent Flutter/Dart versions, and the older Firebase packages don't support these changes.

## Solution

Updated Firebase packages to latest stable versions that support modern JS interop:
- `firebase_auth: ^5.3.1` (was ^4.15.0)
- `firebase_core: ^3.6.0` (was ^2.24.0)

## Changes Made

1. **app/pubspec.yaml**: Updated Firebase dependency versions
2. **app/README.md**: Updated documentation to reflect new versions

## Breaking Changes

None. The app currently uses mock authentication and doesn't directly import or initialize Firebase Auth yet. When Firebase Auth is integrated in the future, the v5.x API is mostly compatible with v4.x with minor differences:

### Notable API Changes (for future reference)

- `FirebaseAuth.instance.authStateChanges()` remains the same
- `FirebaseAuth.instance.userChanges()` remains the same
- Most authentication methods remain backward compatible
- Multi-factor authentication APIs have been enhanced but remain compatible

## Testing

To verify the fix works:

```bash
cd app
flutter pub get
flutter run -d chrome
```

The app should now build and run successfully on web without JS interop errors.

## Migration for Future Firebase Integration

When integrating actual Firebase Auth (replacing the mock service), use these imports:

```dart
import 'package:firebase_core/firebase_core.dart';
import 'package:firebase_auth/firebase_auth.dart';
```

And initialize Firebase before running the app:

```dart
void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp(
    options: DefaultFirebaseOptions.currentPlatform,
  );
  runApp(const MyApp());
}
```

## References

- [Firebase Auth Flutter Plugin Changelog](https://pub.dev/packages/firebase_auth/changelog)
- [Firebase Core Flutter Plugin Changelog](https://pub.dev/packages/firebase_core/changelog)
- [Flutter JS Interop Migration Guide](https://dart.dev/web/js-interop)
