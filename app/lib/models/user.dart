/// User model
class User {
  final String id;
  final String email;
  final String fullName;
  final bool twoFactorEnabled;
  final DateTime createdAt;
  final String? photoUrl;

  User({
    required this.id,
    required this.email,
    required this.fullName,
    required this.twoFactorEnabled,
    required this.createdAt,
    this.photoUrl,
  });

  factory User.fromJson(Map<String, dynamic> json) {
    return User(
      id: json['id'] as String,
      email: json['email'] as String,
      fullName: json['fullName'] as String,
      twoFactorEnabled: json['twoFactorEnabled'] as bool? ?? false,
      createdAt: DateTime.parse(json['createdAt'] as String),
      photoUrl: json['photoUrl'] as String?,
    );
  }

  Map<String, dynamic> toJson() {
    return {
      'id': id,
      'email': email,
      'fullName': fullName,
      'twoFactorEnabled': twoFactorEnabled,
      'createdAt': createdAt.toIso8601String(),
      'photoUrl': photoUrl,
    };
  }
}
