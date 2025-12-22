import React from 'react';
import { authService } from '../services/authService';

const Settings: React.FC = () => {
  const user = authService.getCurrentUser();

  return (
    <div className="page">
      <h1 className="page-title">Settings</h1>

      <div className="card" style={{ marginBottom: '1.5rem' }}>
        <h2>User Profile</h2>
        <div style={{ display: 'grid', gap: '1rem' }}>
          <div>
            <strong>Name:</strong> {user?.fullName}
          </div>
          <div>
            <strong>Email:</strong> {user?.email}
          </div>
          <div>
            <strong>Role:</strong> {user?.role}
          </div>
          <div>
            <strong>2FA Status:</strong>{' '}
            <span className={`badge ${user?.twoFactorEnabled ? 'badge-success' : 'badge-warning'}`}>
              {user?.twoFactorEnabled ? 'Enabled' : 'Disabled'}
            </span>
          </div>
        </div>
      </div>

      <div className="card" style={{ marginBottom: '1.5rem' }}>
        <h2>Security Settings</h2>
        <div style={{ display: 'grid', gap: '1rem' }}>
          <div>
            <strong>Encryption:</strong> <span className="badge badge-success">AES-256-GCM Enabled</span>
          </div>
          <div>
            <strong>TLS Version:</strong> <span className="badge badge-success">TLS 1.3</span>
          </div>
          <div>
            <strong>Certificate Pinning:</strong> <span className="badge badge-success">Enabled</span>
          </div>
        </div>
      </div>

      <div className="card">
        <h2>Application Info</h2>
        <div style={{ display: 'grid', gap: '1rem' }}>
          <div>
            <strong>Version:</strong> 0.1.0 (Pre-Hardware)
          </div>
          <div>
            <strong>Mode:</strong> <span className="badge badge-info">Mock Data</span>
          </div>
          <div>
            <strong>Backend:</strong> Not Connected
          </div>
        </div>
      </div>
    </div>
  );
};

export default Settings;
