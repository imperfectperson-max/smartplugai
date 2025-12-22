import { Device, PowerReading } from '../types/device';
import { getMockDevices, getMockPowerHistory } from './mockData';

// Mock device service for pre-hardware development
// In production: integrate with backend API over HTTPS

class DeviceService {
  // Get all devices
  async getDevices(): Promise<Device[]> {
    // Simulate API delay
    await new Promise(resolve => setTimeout(resolve, 500));
    return getMockDevices();
  }

  // Get single device
  async getDevice(deviceId: string): Promise<Device | undefined> {
    await new Promise(resolve => setTimeout(resolve, 300));
    const devices = getMockDevices();
    return devices.find(d => d.id === deviceId);
  }

  // Toggle device on/off (mock signed command)
  async toggleDevice(deviceId: string): Promise<boolean> {
    await new Promise(resolve => setTimeout(resolve, 500));
    // In production: send signed MQTT command with ECDSA signature
    console.log(`Mock: Toggling device ${deviceId} (signed command)`);
    return true;
  }

  // Get power history
  async getPowerHistory(
    deviceId: string,
    start: Date,
    end: Date
  ): Promise<PowerReading[]> {
    await new Promise(resolve => setTimeout(resolve, 500));
    return getMockPowerHistory(deviceId);
  }

  // Get real-time power stream (mock)
  getPowerStream(deviceId: string, callback: (reading: PowerReading) => void): () => void {
    // Simulate real-time updates every 2 seconds
    const interval = setInterval(() => {
      const now = new Date();
      const basePower = 50 + Math.random() * 100;
      
      callback({
        timestamp: now,
        voltage: 230 + (Math.random() - 0.5) * 4,
        current: basePower / 230,
        power: basePower,
        relayState: true,
        isEncrypted: true,
      });
    }, 2000);

    // Return cleanup function
    return () => clearInterval(interval);
  }

  // Update device name
  async updateDeviceName(deviceId: string, newName: string): Promise<boolean> {
    await new Promise(resolve => setTimeout(resolve, 300));
    console.log(`Mock: Updating device ${deviceId} name to ${newName}`);
    return true;
  }

  // Remove device
  async removeDevice(deviceId: string): Promise<boolean> {
    await new Promise(resolve => setTimeout(resolve, 500));
    console.log(`Mock: Removing device ${deviceId}`);
    return true;
  }
}

export const deviceService = new DeviceService();
