import 'dart:async';
import 'dart:math';
import '../models/device.dart';
import '../models/power_reading.dart';

/// Mock device service for pre-hardware development
/// In production: integrate with backend API and MQTT over TLS 1.3
class DeviceService {
  final List<Device> _devices = [];
  final _deviceStreamController = StreamController<List<Device>>.broadcast();
  Timer? _updateTimer;

  Stream<List<Device>> get devicesStream => _deviceStreamController.stream;
  List<Device> get devices => List.unmodifiable(_devices);

  DeviceService() {
    _initializeMockDevices();
    _startMockUpdates();
  }

  void _initializeMockDevices() {
    _devices.addAll([
      Device(
        id: 'smartplug_001',
        name: 'Living Room Lamp',
        status: 'online',
        isOn: true,
        currentPower: 60.5,
        voltage: 230.2,
        current: 0.263,
        lastSeen: DateTime.now(),
        firmwareVersion: '1.0.0',
        isEncrypted: true,
        isAttested: true,
        tamperDetected: false,
        certificateExpiry: DateTime.now().add(const Duration(days: 365)),
      ),
      Device(
        id: 'smartplug_002',
        name: 'Kitchen Coffee Maker',
        status: 'online',
        isOn: false,
        currentPower: 0.0,
        voltage: 229.8,
        current: 0.0,
        lastSeen: DateTime.now(),
        firmwareVersion: '1.0.0',
        isEncrypted: true,
        isAttested: true,
        tamperDetected: false,
        certificateExpiry: DateTime.now().add(const Duration(days: 360)),
      ),
      Device(
        id: 'smartplug_003',
        name: 'Home Office Monitor',
        status: 'online',
        isOn: true,
        currentPower: 45.2,
        voltage: 230.5,
        current: 0.196,
        lastSeen: DateTime.now(),
        firmwareVersion: '1.0.0',
        isEncrypted: true,
        isAttested: true,
        tamperDetected: false,
        certificateExpiry: DateTime.now().add(const Duration(days: 350)),
      ),
    ]);
    _deviceStreamController.add(_devices);
  }

  void _startMockUpdates() {
    // Update device states every 2 seconds to simulate real-time data
    _updateTimer = Timer.periodic(const Duration(seconds: 2), (_) {
      _updateMockDeviceData();
    });
  }

  void _updateMockDeviceData() {
    final random = Random();
    for (var i = 0; i < _devices.length; i++) {
      if (_devices[i].isOn) {
        // Simulate slight variations in power readings
        final basePower = _devices[i].currentPower;
        final variation = (random.nextDouble() - 0.5) * 10; // ±5W variation
        final newPower = (basePower + variation).clamp(0, 3000);
        
        _devices[i] = _devices[i].copyWith(
          currentPower: newPower,
          voltage: 230.0 + (random.nextDouble() - 0.5) * 4, // ±2V variation
          current: newPower / 230.0,
          lastSeen: DateTime.now(),
        );
      }
    }
    _deviceStreamController.add(_devices);
  }

  /// Toggle device on/off (mock signed command)
  /// In production: send signed MQTT command with ECDSA signature
  Future<bool> toggleDevice(String deviceId) async {
    await Future.delayed(const Duration(milliseconds: 500));
    
    final index = _devices.indexWhere((d) => d.id == deviceId);
    if (index != -1) {
      final device = _devices[index];
      final newIsOn = !device.isOn;
      
      _devices[index] = device.copyWith(
        isOn: newIsOn,
        currentPower: newIsOn ? (device.currentPower > 0 ? device.currentPower : 60.0) : 0.0,
        current: newIsOn ? (device.current > 0 ? device.current : 0.26) : 0.0,
        lastSeen: DateTime.now(),
      );
      
      _deviceStreamController.add(_devices);
      return true;
    }
    return false;
  }

  /// Get power reading stream for a specific device
  Stream<PowerReading> getPowerReadingStream(String deviceId) async* {
    final random = Random();
    while (true) {
      final device = _devices.firstWhere(
        (d) => d.id == deviceId,
        orElse: () => _devices.first,
      );

      yield PowerReading(
        timestamp: DateTime.now(),
        voltage: device.voltage,
        current: device.current,
        power: device.currentPower,
        relayState: device.isOn,
        isEncrypted: true,
      );

      await Future.delayed(const Duration(seconds: 1));
    }
  }

  /// Get historical power data (mock)
  Future<List<PowerReading>> getHistoricalData(
    String deviceId, {
    required DateTime start,
    required DateTime end,
  }) async {
    await Future.delayed(const Duration(milliseconds: 500));

    final random = Random();
    final readings = <PowerReading>[];
    final device = _devices.firstWhere(
      (d) => d.id == deviceId,
      orElse: () => _devices.first,
    );

    // Generate mock historical data points
    var current = start;
    while (current.isBefore(end)) {
      readings.add(PowerReading(
        timestamp: current,
        voltage: 230.0 + (random.nextDouble() - 0.5) * 4,
        current: device.isOn ? 0.2 + random.nextDouble() * 0.3 : 0.0,
        power: device.isOn ? 40.0 + random.nextDouble() * 40 : 0.0,
        relayState: device.isOn,
        isEncrypted: true,
      ));
      current = current.add(const Duration(minutes: 5));
    }

    return readings;
  }

  /// Add new device (mock QR pairing with challenge-response)
  Future<Device> addDevice(String deviceId, String name) async {
    await Future.delayed(const Duration(seconds: 1));

    final newDevice = Device(
      id: deviceId,
      name: name,
      status: 'online',
      isOn: false,
      currentPower: 0.0,
      voltage: 230.0,
      current: 0.0,
      lastSeen: DateTime.now(),
      firmwareVersion: '1.0.0',
      isEncrypted: true,
      isAttested: true,
      tamperDetected: false,
      certificateExpiry: DateTime.now().add(const Duration(days: 365)),
    );

    _devices.add(newDevice);
    _deviceStreamController.add(_devices);
    return newDevice;
  }

  /// Remove device
  Future<bool> removeDevice(String deviceId) async {
    await Future.delayed(const Duration(milliseconds: 500));
    
    final removed = _devices.removeWhere((d) => d.id == deviceId);
    if (removed > 0) {
      _deviceStreamController.add(_devices);
      return true;
    }
    return false;
  }

  /// Update device name
  Future<bool> updateDeviceName(String deviceId, String newName) async {
    await Future.delayed(const Duration(milliseconds: 500));
    
    final index = _devices.indexWhere((d) => d.id == deviceId);
    if (index != -1) {
      _devices[index] = _devices[index].copyWith(name: newName);
      _deviceStreamController.add(_devices);
      return true;
    }
    return false;
  }

  void dispose() {
    _updateTimer?.cancel();
    _deviceStreamController.close();
  }
}
