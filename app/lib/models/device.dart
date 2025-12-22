/// Device model representing a smart plug
class Device {
  final String id;
  final String name;
  final String status; // 'online', 'offline', 'error'
  final bool isOn;
  final double currentPower; // Watts
  final double voltage; // Volts
  final double current; // Amps
  final DateTime lastSeen;
  final String firmwareVersion;
  
  // Security properties
  final bool isEncrypted; // Data encryption status
  final bool isAttested; // Firmware attestation status
  final bool tamperDetected; // Tamper alert status
  final DateTime? certificateExpiry;

  Device({
    required this.id,
    required this.name,
    required this.status,
    required this.isOn,
    required this.currentPower,
    required this.voltage,
    required this.current,
    required this.lastSeen,
    required this.firmwareVersion,
    this.isEncrypted = true,
    this.isAttested = true,
    this.tamperDetected = false,
    this.certificateExpiry,
  });

  // Factory constructor for creating from JSON
  factory Device.fromJson(Map<String, dynamic> json) {
    return Device(
      id: json['id'] as String,
      name: json['name'] as String,
      status: json['status'] as String,
      isOn: json['isOn'] as bool,
      currentPower: (json['currentPower'] as num).toDouble(),
      voltage: (json['voltage'] as num).toDouble(),
      current: (json['current'] as num).toDouble(),
      lastSeen: DateTime.parse(json['lastSeen'] as String),
      firmwareVersion: json['firmwareVersion'] as String,
      isEncrypted: json['isEncrypted'] as bool? ?? true,
      isAttested: json['isAttested'] as bool? ?? true,
      tamperDetected: json['tamperDetected'] as bool? ?? false,
      certificateExpiry: json['certificateExpiry'] != null 
          ? DateTime.parse(json['certificateExpiry'] as String) 
          : null,
    );
  }

  // Convert to JSON
  Map<String, dynamic> toJson() {
    return {
      'id': id,
      'name': name,
      'status': status,
      'isOn': isOn,
      'currentPower': currentPower,
      'voltage': voltage,
      'current': current,
      'lastSeen': lastSeen.toIso8601String(),
      'firmwareVersion': firmwareVersion,
      'isEncrypted': isEncrypted,
      'isAttested': isAttested,
      'tamperDetected': tamperDetected,
      'certificateExpiry': certificateExpiry?.toIso8601String(),
    };
  }

  // Copy with method for updates
  Device copyWith({
    String? name,
    String? status,
    bool? isOn,
    double? currentPower,
    double? voltage,
    double? current,
    DateTime? lastSeen,
    bool? isEncrypted,
    bool? isAttested,
    bool? tamperDetected,
  }) {
    return Device(
      id: id,
      name: name ?? this.name,
      status: status ?? this.status,
      isOn: isOn ?? this.isOn,
      currentPower: currentPower ?? this.currentPower,
      voltage: voltage ?? this.voltage,
      current: current ?? this.current,
      lastSeen: lastSeen ?? this.lastSeen,
      firmwareVersion: firmwareVersion,
      isEncrypted: isEncrypted ?? this.isEncrypted,
      isAttested: isAttested ?? this.isAttested,
      tamperDetected: tamperDetected ?? this.tamperDetected,
      certificateExpiry: certificateExpiry,
    );
  }
}
