import React, { useState } from 'react';
import { useNavigate } from 'react-router-dom';
import { authService } from '../services/authService';
import './LoginPage.css';

const LoginPage: React.FC = () => {
  const navigate = useNavigate();
  const [email, setEmail] = useState('');
  const [password, setPassword] = useState('');
  const [twoFACode, setTwoFACode] = useState('');
  const [isLoading, setIsLoading] = useState(false);
  const [show2FA, setShow2FA] = useState(false);
  const [error, setError] = useState('');

  const handleLogin = async (e: React.FormEvent) => {
    e.preventDefault();
    setError('');
    setIsLoading(true);

    try {
      await authService.login({ email, password });
      setShow2FA(true);
    } catch (err) {
      setError('Invalid email or password');
    } finally {
      setIsLoading(false);
    }
  };

  const handle2FA = async (e: React.FormEvent) => {
    e.preventDefault();
    setError('');
    setIsLoading(true);

    try {
      const isValid = await authService.verify2FA(twoFACode);
      if (isValid) {
        navigate('/');
      } else {
        setError('Invalid 2FA code');
      }
    } catch (err) {
      setError('2FA verification failed');
    } finally {
      setIsLoading(false);
    }
  };

  return (
    <div className="login-page">
      <div className="login-container">
        <div className="login-header">
          <h1>⚡ Smart Plug AI</h1>
          <div className="security-info">
            <span>🔒</span>
            <span>Bank-Grade Security</span>
          </div>
        </div>

        {!show2FA ? (
          <form onSubmit={handleLogin} className="login-form">
            <h2>Login to Dashboard</h2>
            
            {error && <div className="error-message">{error}</div>}

            <div className="form-group">
              <label htmlFor="email">Email</label>
              <input
                id="email"
                type="email"
                value={email}
                onChange={(e) => setEmail(e.target.value)}
                placeholder="you@example.com"
                required
              />
            </div>

            <div className="form-group">
              <label htmlFor="password">Password</label>
              <input
                id="password"
                type="password"
                value={password}
                onChange={(e) => setPassword(e.target.value)}
                placeholder="••••••••"
                required
              />
            </div>

            <button type="submit" className="btn-primary btn-full" disabled={isLoading}>
              {isLoading ? 'Loading...' : 'Login'}
            </button>

            <div className="mock-hint">
              <strong>Mock Login (Pre-Hardware)</strong>
              <p>Use any email and password (6+ chars)</p>
            </div>
          </form>
        ) : (
          <form onSubmit={handle2FA} className="login-form">
            <h2>Two-Factor Authentication</h2>
            
            {error && <div className="error-message">{error}</div>}

            <p className="instruction">Enter the 6-digit code from your authenticator app</p>

            <div className="form-group">
              <label htmlFor="code">2FA Code</label>
              <input
                id="code"
                type="text"
                value={twoFACode}
                onChange={(e) => setTwoFACode(e.target.value.replace(/\D/g, ''))}
                placeholder="000000"
                maxLength={6}
                required
                className="code-input"
              />
            </div>

            <button type="submit" className="btn-primary btn-full" disabled={isLoading}>
              {isLoading ? 'Verifying...' : 'Verify'}
            </button>

            <div className="mock-hint">
              <strong>Mock 2FA (Pre-Hardware)</strong>
              <p>Enter any 6-digit code</p>
            </div>
          </form>
        )}
      </div>
    </div>
  );
};

export default LoginPage;
