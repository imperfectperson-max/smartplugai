import { User, LoginCredentials, AuthResponse } from '../types/user';

// Mock auth service for pre-hardware development
// In production: integrate OAuth2/Auth0 with 2FA

class AuthService {
  private currentUser: User | null = null;
  private accessToken: string | null = null;

  // Mock login
  async login(credentials: LoginCredentials): Promise<AuthResponse> {
    // Simulate API delay
    await new Promise(resolve => setTimeout(resolve, 1000));

    // Mock validation
    if (credentials.email && credentials.password.length >= 6) {
      const user: User = {
        id: 'mock-user-id',
        email: credentials.email,
        fullName: 'Demo Administrator',
        role: 'admin',
        twoFactorEnabled: true,
        createdAt: new Date(),
      };

      const response: AuthResponse = {
        accessToken: 'mock-jwt-token-' + Date.now(),
        refreshToken: 'mock-refresh-token-' + Date.now(),
        user,
      };

      this.currentUser = user;
      this.accessToken = response.accessToken;

      // Store in sessionStorage for pre-hardware demo only
      // SECURITY NOTE: In production, use secure httpOnly cookies with SameSite=Strict
      // and implement proper CSRF protection. Never store tokens in localStorage or
      // sessionStorage in production as they are vulnerable to XSS attacks.
      sessionStorage.setItem('user', JSON.stringify(user));
      sessionStorage.setItem('accessToken', response.accessToken);

      return response;
    }

    throw new Error('Invalid credentials');
  }

  // Mock 2FA verification
  async verify2FA(code: string): Promise<boolean> {
    await new Promise(resolve => setTimeout(resolve, 500));
    // Accept any 6-digit code for mock
    return code.length === 6;
  }

  // Logout
  async logout(): Promise<void> {
    this.currentUser = null;
    this.accessToken = null;
    sessionStorage.clear();
  }

  // Get current user
  getCurrentUser(): User | null {
    if (this.currentUser) return this.currentUser;

    // Try to restore from sessionStorage
    const userStr = sessionStorage.getItem('user');
    if (userStr) {
      this.currentUser = JSON.parse(userStr);
      return this.currentUser;
    }

    return null;
  }

  // Check if authenticated
  isAuthenticated(): boolean {
    return !!this.getCurrentUser();
  }

  // Get access token
  getAccessToken(): string | null {
    if (this.accessToken) return this.accessToken;
    return sessionStorage.getItem('accessToken');
  }
}

export const authService = new AuthService();
