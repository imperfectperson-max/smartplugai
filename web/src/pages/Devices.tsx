import React, { useEffect, useState } from 'react';
import { deviceService } from '../services/deviceService';
import { Device } from '../types/device';

const Devices: React.FC = () => {
  const [devices, setDevices] = useState<Device[]>([]);
  const [isLoading, setIsLoading] = useState(true);

  useEffect(() => {
    loadDevices();
  }, []);

  const loadDevices = async () => {
    const data = await deviceService.getDevices();
    setDevices(data);
    setIsLoading(false);
  };

  const handleToggle = async (deviceId: string) => {
    await deviceService.toggleDevice(deviceId);
    await loadDevices();
  };

  if (isLoading) return <div className="loading">Loading devices...</div>;

  return (
    <div className="page">
      <h1 className="page-title">Devices</h1>
      
      <div className="card">
        <table className="devices-table">
          <thead>
            <tr>
              <th>Device Name</th>
              <th>Status</th>
              <th>Power</th>
              <th>Security</th>
              <th>Actions</th>
            </tr>
          </thead>
          <tbody>
            {devices.map(device => (
              <tr key={device.id}>
                <td>
                  <strong>{device.name}</strong>
                  <div style={{ fontSize: '0.75rem', color: '#6b7280' }}>{device.id}</div>
                </td>
                <td>
                  <span className={`badge badge-${device.status === 'online' ? 'success' : 'danger'}`}>
                    {device.status}
                  </span>
                </td>
                <td>{device.currentPower.toFixed(1)} W</td>
                <td>
                  <div style={{ display: 'flex', gap: '0.25rem', flexWrap: 'wrap' }}>
                    {device.isEncrypted && <span className="badge badge-info">🔒</span>}
                    {device.isAttested && <span className="badge badge-success">✓</span>}
                    {device.tamperDetected && <span className="badge badge-danger">⚠️</span>}
                  </div>
                </td>
                <td>
                  <button
                    onClick={() => handleToggle(device.id)}
                    className={`btn ${device.isOn ? 'btn-danger' : 'btn-primary'}`}
                    disabled={device.status !== 'online'}
                  >
                    {device.isOn ? 'Turn OFF' : 'Turn ON'}
                  </button>
                </td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>
    </div>
  );
};

export default Devices;
