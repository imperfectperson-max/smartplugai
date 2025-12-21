/// Power reading model for telemetry data
class PowerReading {
  final DateTime timestamp;
  final double voltage; // Volts
  final double current; // Amps
  final double power; // Watts
  final bool relayState;
  final bool isEncrypted; // Indicates if this reading was encrypted in transit

  PowerReading({
    required this.timestamp,
    required this.voltage,
    required this.current,
    required this.power,
    required this.relayState,
    this.isEncrypted = true,
  });

  factory PowerReading.fromJson(Map<String, dynamic> json) {
    return PowerReading(
      timestamp: DateTime.parse(json['timestamp'] as String),
      voltage: (json['voltage'] as num).toDouble(),
      current: (json['current'] as num).toDouble(),
      power: (json['power'] as num).toDouble(),
      relayState: json['relayState'] as bool,
      isEncrypted: json['isEncrypted'] as bool? ?? true,
    );
  }

  Map<String, dynamic> toJson() {
    return {
      'timestamp': timestamp.toIso8601String(),
      'voltage': voltage,
      'current': current,
      'power': power,
      'relayState': relayState,
      'isEncrypted': isEncrypted,
    };
  }

  // Calculate energy in kWh for a given duration
  double calculateEnergy(Duration duration) {
    return (power * duration.inSeconds) / 3600000; // kWh
  }
}
