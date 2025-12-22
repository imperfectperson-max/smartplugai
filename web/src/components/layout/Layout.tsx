import React from 'react';
import { Outlet, Link, useNavigate } from 'react-router-dom';
import { authService } from '../../services/authService';
import './Layout.css';

const Layout: React.FC = () => {
  const navigate = useNavigate();
  const user = authService.getCurrentUser();

  const handleLogout = async () => {
    await authService.logout();
    navigate('/login');
  };

  return (
    <div className="layout">
      {/* Header */}
      <header className="header">
        <div className="header-content">
          <div className="header-left">
            <h1 className="logo">⚡ Smart Plug AI</h1>
            <div className="security-badge">
              <span>🔒</span>
              <span>TLS 1.3 Encrypted</span>
            </div>
          </div>
          <div className="header-right">
            <div className="user-info">
              <span>{user?.fullName}</span>
              {user?.twoFactorEnabled && (
                <span className="badge badge-success">2FA</span>
              )}
            </div>
            <button onClick={handleLogout} className="btn-logout">
              Logout
            </button>
          </div>
        </div>
      </header>

      {/* Sidebar */}
      <aside className="sidebar">
        <nav className="nav">
          <Link to="/" className="nav-link">
            <span>📊</span>
            <span>Dashboard</span>
          </Link>
          <Link to="/devices" className="nav-link">
            <span>🔌</span>
            <span>Devices</span>
          </Link>
          <Link to="/analytics" className="nav-link">
            <span>📈</span>
            <span>Analytics</span>
          </Link>
          <Link to="/security" className="nav-link">
            <span>🔐</span>
            <span>Security</span>
          </Link>
          <Link to="/settings" className="nav-link">
            <span>⚙️</span>
            <span>Settings</span>
          </Link>
        </nav>

        {/* Mock mode indicator */}
        <div className="mock-indicator">
          <div className="mock-badge">
            <span>🧪 Pre-Hardware Mode</span>
          </div>
          <p className="mock-text">Using mock data</p>
        </div>
      </aside>

      {/* Main content */}
      <main className="main-content">
        <Outlet />
      </main>
    </div>
  );
};

export default Layout;
