import React, { useEffect, useState } from 'react';
import { deviceService } from '../services/deviceService';
import { getMockStatistics, getMockPowerHistory, getMockSecurityAlerts } from '../services/mockData';
import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, ResponsiveContainer } from 'recharts';
import { Device, DeviceStatistics } from '../types/device';
import { SecurityAlert } from '../types/security';
import './Dashboard.css';

const Dashboard: React.FC = () => {
  const [devices, setDevices] = useState<Device[]>([]);
  const [stats, setStats] = useState<DeviceStatistics | null>(null);
  const [powerHistory, setPowerHistory] = useState<any[]>([]);
  const [alerts, setAlerts] = useState<SecurityAlert[]>([]);
  const [isLoading, setIsLoading] = useState(true);

  useEffect(() => {
    loadData();
  }, []);

  const loadData = async () => {
    setIsLoading(true);
    const devicesData = await deviceService.getDevices();
    const statsData = getMockStatistics();
    const historyData = getMockPowerHistory();
    const alertsData = getMockSecurityAlerts();

    setDevices(devicesData);
    setStats(statsData);
    setPowerHistory(historyData.map(r => ({
      time: r.timestamp.getHours() + ':00',
      power: r.power,
    })));
    setAlerts(alertsData.filter(a => !a.resolved));
    setIsLoading(false);
  };

  if (isLoading) {
    return <div className="loading">Loading dashboard...</div>;
  }

  return (
    <div className="dashboard">
      <h1 className="page-title">Dashboard</h1>

      {/* Summary Cards */}
      <div className="stats-grid">
        <div className="stat-card">
          <div className="stat-icon">🔌</div>
          <div className="stat-content">
            <div className="stat-label">Total Devices</div>
            <div className="stat-value">{stats?.totalDevices}</div>
            <div className="stat-detail">{stats?.onlineDevices} online</div>
          </div>
        </div>

        <div className="stat-card">
          <div className="stat-icon">⚡</div>
          <div className="stat-content">
            <div className="stat-label">Total Power</div>
            <div className="stat-value">{stats?.totalPower.toFixed(1)} W</div>
            <div className="stat-detail">{stats?.totalEnergy} kWh today</div>
          </div>
        </div>

        <div className="stat-card">
          <div className="stat-icon">🔒</div>
          <div className="stat-content">
            <div className="stat-label">Security Score</div>
            <div className="stat-value">
              {stats ? Math.round((stats.secureDevices / stats.totalDevices) * 100) : 0}%
            </div>
            <div className="stat-detail">{stats?.secureDevices} devices secure</div>
          </div>
        </div>

        <div className="stat-card">
          <div className="stat-icon">🛡️</div>
          <div className="stat-content">
            <div className="stat-label">Active Alerts</div>
            <div className="stat-value">{alerts.length}</div>
            <div className="stat-detail">
              {alerts.filter(a => a.severity === 'critical').length} critical
            </div>
          </div>
        </div>
      </div>

      {/* Security Alerts */}
      {alerts.length > 0 && (
        <div className="card alerts-section">
          <h2>Security Alerts</h2>
          <div className="alerts-list">
            {alerts.map(alert => (
              <div key={alert.id} className={`alert alert-${alert.severity}`}>
                <div className="alert-icon">
                  {alert.type === 'tamper' && '⚠️'}
                  {alert.type === 'attestation_failed' && '🔐'}
                  {alert.type === 'anomaly' && '📊'}
                </div>
                <div className="alert-content">
                  <div className="alert-title">{alert.deviceName}</div>
                  <div className="alert-message">{alert.message}</div>
                  <div className="alert-time">
                    {new Date(alert.timestamp).toLocaleString()}
                  </div>
                </div>
                <div className={`badge badge-${alert.severity}`}>
                  {alert.severity}
                </div>
              </div>
            ))}
          </div>
        </div>
      )}

      {/* Power Usage Chart */}
      <div className="card chart-section">
        <h2>24-Hour Power Usage (Encrypted Data)</h2>
        <ResponsiveContainer width="100%" height={300}>
          <LineChart data={powerHistory}>
            <CartesianGrid strokeDasharray="3 3" />
            <XAxis dataKey="time" />
            <YAxis />
            <Tooltip />
            <Line type="monotone" dataKey="power" stroke="#2563eb" strokeWidth={2} />
          </LineChart>
        </ResponsiveContainer>
      </div>

      {/* Device Status Grid */}
      <div className="card devices-section">
        <h2>Device Status</h2>
        <div className="devices-grid">
          {devices.map(device => (
            <div key={device.id} className="device-mini-card">
              <div className="device-header">
                <div className="device-name">{device.name}</div>
                <div className={`status-dot ${device.status}`}></div>
              </div>
              <div className="device-power">{device.currentPower.toFixed(1)} W</div>
              <div className="device-security">
                {device.isEncrypted && <span className="badge badge-info">🔒 Encrypted</span>}
                {device.tamperDetected && <span className="badge badge-danger">⚠️ Tamper</span>}
                {!device.isAttested && <span className="badge badge-warning">⚠️ Not Attested</span>}
              </div>
            </div>
          ))}
        </div>
      </div>
    </div>
  );
};

export default Dashboard;
