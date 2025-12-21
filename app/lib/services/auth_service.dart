import 'package:flutter/foundation.dart';
import '../models/user.dart';

/// Mock authentication service
/// In production, this will integrate with Firebase Auth or Auth0 with 2FA
class AuthService extends ChangeNotifier {
  User? _currentUser;
  bool _isAuthenticated = false;

  User? get currentUser => _currentUser;
  bool get isAuthenticated => _isAuthenticated;

  /// Mock login with 2FA simulation
  /// In production: integrate Firebase Auth with TOTP/SMS 2FA
  Future<bool> login(String email, String password) async {
    // Simulate API delay
    await Future.delayed(const Duration(seconds: 1));

    // Mock validation (in production, use Firebase/Auth0)
    if (email.isNotEmpty && password.length >= 6) {
      _currentUser = User(
        id: 'mock-user-id',
        email: email,
        fullName: 'Demo User',
        twoFactorEnabled: true,
        createdAt: DateTime.now(),
      );
      _isAuthenticated = true;
      notifyListeners();
      return true;
    }
    return false;
  }

  /// Mock 2FA verification
  /// In production: verify TOTP code or SMS code
  Future<bool> verify2FA(String code) async {
    await Future.delayed(const Duration(seconds: 1));
    
    // Mock: accept any 6-digit code
    if (code.length == 6) {
      return true;
    }
    return false;
  }

  /// Mock signup
  Future<bool> signup(String email, String password, String fullName) async {
    await Future.delayed(const Duration(seconds: 1));

    if (email.isNotEmpty && password.length >= 6 && fullName.isNotEmpty) {
      _currentUser = User(
        id: 'mock-user-id-${DateTime.now().millisecondsSinceEpoch}',
        email: email,
        fullName: fullName,
        twoFactorEnabled: false,
        createdAt: DateTime.now(),
      );
      _isAuthenticated = true;
      notifyListeners();
      return true;
    }
    return false;
  }

  /// Logout
  Future<void> logout() async {
    _currentUser = null;
    _isAuthenticated = false;
    notifyListeners();
  }

  /// Enable 2FA (mock)
  Future<String> enable2FA() async {
    await Future.delayed(const Duration(seconds: 1));
    // In production: generate TOTP secret and return QR code URL
    return 'otpauth://totp/SmartPlugAI:${_currentUser?.email}?secret=MOCKBASE32SECRET&issuer=SmartPlugAI';
  }

  /// Disable 2FA (mock)
  Future<bool> disable2FA(String password) async {
    await Future.delayed(const Duration(seconds: 1));
    if (password.isNotEmpty) {
      return true;
    }
    return false;
  }
}
