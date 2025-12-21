export interface Device {
  id: string;
  name: string;
  status: 'online' | 'offline' | 'error';
  isOn: boolean;
  currentPower: number; // Watts
  voltage: number; // Volts
  current: number; // Amps
  lastSeen: Date;
  firmwareVersion: string;
  
  // Security properties
  isEncrypted: boolean;
  isAttested: boolean;
  tamperDetected: boolean;
  certificateExpiry?: Date;
  securityScore: number; // 0-100
}

export interface PowerReading {
  timestamp: Date;
  voltage: number;
  current: number;
  power: number;
  relayState: boolean;
  isEncrypted: boolean;
}

export interface DeviceStatistics {
  totalDevices: number;
  onlineDevices: number;
  totalPower: number;
  totalEnergy: number; // kWh
  secureDevices: number;
  devicesWith2FA: number;
}
