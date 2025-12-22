import { Device, DeviceStatistics, PowerReading } from '../types/device';
import { SecurityAlert, AuditLogEntry } from '../types/security';

// Mock devices data
export const getMockDevices = (): Device[] => [
  {
    id: 'smartplug_001',
    name: 'Living Room Lamp',
    status: 'online',
    isOn: true,
    currentPower: 60.5,
    voltage: 230.2,
    current: 0.263,
    lastSeen: new Date(),
    firmwareVersion: '1.0.0',
    isEncrypted: true,
    isAttested: true,
    tamperDetected: false,
    certificateExpiry: new Date(Date.now() + 365 * 24 * 60 * 60 * 1000),
    securityScore: 95,
  },
  {
    id: 'smartplug_002',
    name: 'Kitchen Coffee Maker',
    status: 'online',
    isOn: false,
    currentPower: 0.0,
    voltage: 229.8,
    current: 0.0,
    lastSeen: new Date(),
    firmwareVersion: '1.0.0',
    isEncrypted: true,
    isAttested: true,
    tamperDetected: false,
    certificateExpiry: new Date(Date.now() + 360 * 24 * 60 * 60 * 1000),
    securityScore: 98,
  },
  {
    id: 'smartplug_003',
    name: 'Home Office Monitor',
    status: 'online',
    isOn: true,
    currentPower: 45.2,
    voltage: 230.5,
    current: 0.196,
    lastSeen: new Date(),
    firmwareVersion: '1.0.0',
    isEncrypted: true,
    isAttested: true,
    tamperDetected: false,
    certificateExpiry: new Date(Date.now() + 350 * 24 * 60 * 60 * 1000),
    securityScore: 92,
  },
  {
    id: 'smartplug_004',
    name: 'Bedroom Air Conditioner',
    status: 'offline',
    isOn: false,
    currentPower: 0.0,
    voltage: 0.0,
    current: 0.0,
    lastSeen: new Date(Date.now() - 2 * 60 * 60 * 1000),
    firmwareVersion: '0.9.5',
    isEncrypted: true,
    isAttested: false,
    tamperDetected: false,
    certificateExpiry: new Date(Date.now() + 340 * 24 * 60 * 60 * 1000),
    securityScore: 75,
  },
  {
    id: 'smartplug_005',
    name: 'Garage Workshop',
    status: 'online',
    isOn: true,
    currentPower: 1250.8,
    voltage: 231.0,
    current: 5.415,
    lastSeen: new Date(),
    firmwareVersion: '1.0.0',
    isEncrypted: true,
    isAttested: true,
    tamperDetected: true,
    certificateExpiry: new Date(Date.now() + 355 * 24 * 60 * 60 * 1000),
    securityScore: 65,
  },
];

// Mock device statistics
export const getMockStatistics = (): DeviceStatistics => {
  const devices = getMockDevices();
  return {
    totalDevices: devices.length,
    onlineDevices: devices.filter(d => d.status === 'online').length,
    totalPower: devices.reduce((sum, d) => sum + d.currentPower, 0),
    totalEnergy: 245.8, // kWh (mock)
    secureDevices: devices.filter(d => d.isAttested && !d.tamperDetected).length,
    devicesWith2FA: devices.filter(d => d.isEncrypted).length,
  };
};

// Mock power history for charts
export const getMockPowerHistory = (deviceId?: string): PowerReading[] => {
  const readings: PowerReading[] = [];
  const now = Date.now();
  
  // Generate 24 hours of data at 5-minute intervals
  for (let i = 0; i < 288; i++) {
    const timestamp = new Date(now - (288 - i) * 5 * 60 * 1000);
    const hour = timestamp.getHours();
    
    // Simulate realistic power pattern
    let basePower = 50;
    if (hour >= 6 && hour < 9) basePower = 200; // Morning peak
    if (hour >= 18 && hour < 23) basePower = 300; // Evening peak
    if (hour >= 0 && hour < 6) basePower = 20; // Night low
    
    const variation = (Math.random() - 0.5) * 50;
    const power = Math.max(0, basePower + variation);
    
    readings.push({
      timestamp,
      voltage: 230 + (Math.random() - 0.5) * 4,
      current: power / 230,
      power,
      relayState: power > 5,
      isEncrypted: true,
    });
  }
  
  return readings;
};

// Mock security alerts
export const getMockSecurityAlerts = (): SecurityAlert[] => [
  {
    id: 'alert_001',
    deviceId: 'smartplug_005',
    deviceName: 'Garage Workshop',
    type: 'tamper',
    severity: 'critical',
    message: 'Tamper detection triggered - physical security breach detected',
    timestamp: new Date(Date.now() - 30 * 60 * 1000),
    resolved: false,
  },
  {
    id: 'alert_002',
    deviceId: 'smartplug_004',
    deviceName: 'Bedroom Air Conditioner',
    type: 'attestation_failed',
    severity: 'high',
    message: 'Device attestation failed - firmware integrity check unsuccessful',
    timestamp: new Date(Date.now() - 2 * 60 * 60 * 1000),
    resolved: false,
  },
  {
    id: 'alert_003',
    deviceId: 'smartplug_001',
    deviceName: 'Living Room Lamp',
    type: 'anomaly',
    severity: 'medium',
    message: 'Unusual power consumption pattern detected',
    timestamp: new Date(Date.now() - 24 * 60 * 60 * 1000),
    resolved: true,
  },
];

// Mock audit log
export const getMockAuditLog = (): AuditLogEntry[] => [
  {
    id: 'log_001',
    timestamp: new Date(Date.now() - 5 * 60 * 1000),
    userId: 'user_001',
    userName: 'Admin User',
    action: 'DEVICE_CONTROL',
    resource: 'smartplug_001',
    details: 'Turned device ON',
    ipAddress: '192.168.1.100',
  },
  {
    id: 'log_002',
    timestamp: new Date(Date.now() - 15 * 60 * 1000),
    userId: 'user_001',
    userName: 'Admin User',
    action: 'LOGIN',
    resource: 'authentication',
    details: 'User logged in with 2FA',
    ipAddress: '192.168.1.100',
  },
  {
    id: 'log_003',
    timestamp: new Date(Date.now() - 30 * 60 * 1000),
    userId: 'user_002',
    userName: 'Guest User',
    action: 'VIEW_DEVICE',
    resource: 'smartplug_003',
    details: 'Viewed device details',
    ipAddress: '192.168.1.105',
  },
  {
    id: 'log_004',
    timestamp: new Date(Date.now() - 60 * 60 * 1000),
    userId: 'user_001',
    userName: 'Admin User',
    action: 'CONFIG_CHANGE',
    resource: 'system',
    details: 'Updated security settings',
    ipAddress: '192.168.1.100',
  },
];
