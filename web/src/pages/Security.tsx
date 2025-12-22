import React from 'react';
import { getMockSecurityAlerts, getMockAuditLog } from '../services/mockData';

const Security: React.FC = () => {
  const alerts = getMockSecurityAlerts();
  const auditLog = getMockAuditLog();

  return (
    <div className="page">
      <h1 className="page-title">Security Dashboard</h1>

      <div className="card" style={{ marginBottom: '1.5rem' }}>
        <h2>Security Alerts</h2>
        <table className="devices-table">
          <thead>
            <tr>
              <th>Device</th>
              <th>Type</th>
              <th>Severity</th>
              <th>Message</th>
              <th>Time</th>
              <th>Status</th>
            </tr>
          </thead>
          <tbody>
            {alerts.map(alert => (
              <tr key={alert.id}>
                <td>{alert.deviceName}</td>
                <td>{alert.type.replace('_', ' ')}</td>
                <td>
                  <span className={`badge badge-${alert.severity}`}>
                    {alert.severity}
                  </span>
                </td>
                <td>{alert.message}</td>
                <td>{new Date(alert.timestamp).toLocaleString()}</td>
                <td>
                  <span className={`badge ${alert.resolved ? 'badge-success' : 'badge-warning'}`}>
                    {alert.resolved ? 'Resolved' : 'Active'}
                  </span>
                </td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>

      <div className="card">
        <h2>Audit Log</h2>
        <table className="devices-table">
          <thead>
            <tr>
              <th>Time</th>
              <th>User</th>
              <th>Action</th>
              <th>Resource</th>
              <th>Details</th>
              <th>IP Address</th>
            </tr>
          </thead>
          <tbody>
            {auditLog.map(log => (
              <tr key={log.id}>
                <td>{new Date(log.timestamp).toLocaleString()}</td>
                <td>{log.userName}</td>
                <td><code>{log.action}</code></td>
                <td><code>{log.resource}</code></td>
                <td>{log.details}</td>
                <td><code>{log.ipAddress}</code></td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>
    </div>
  );
};

export default Security;
