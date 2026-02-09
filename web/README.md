# Smart Plug AI Web Dashboard

React-based admin dashboard for monitoring, managing, and analyzing smart plug devices at scale.

## 🎯 Overview

The web dashboard provides administrators and power users with:
- **Multi-device overview**: Monitor all connected devices at a glance with security status
- **Advanced analytics**: Deep dive into power consumption patterns with encrypted data visualization
- **User management**: Manage user accounts, permissions, and role-based access control (RBAC)
- **Device administration**: Configure and troubleshoot devices remotely with secure commands
- **Data export**: Generate reports and export data for analysis
- **System monitoring**: View system health, performance metrics, and security alerts
- **Security dashboard**: Display device attestation status, tamper alerts, and security audit logs
- **Real-time updates**: WebSocket over TLS for live data streaming

## 🌐 Target Users

- **Homeowners**: Manage multiple smart plugs in their home with secure access
- **Facility managers**: Monitor power usage across buildings with RBAC
- **Energy analysts**: Analyze consumption patterns and trends with encrypted data access
- **System administrators**: Configure and maintain the platform with OAuth2/SSO authentication
- **Security auditors**: Review audit logs, device attestation status, and tamper alerts

## 🔒 Security Features

Smart Plug AI Web Dashboard implements **enterprise-grade security**:

### Authentication & Authorization
- **OAuth2 / SSO** integration (Auth0, Google, Microsoft, Okta)
- **2FA/MFA** support for admin accounts
- **Role-Based Access Control (RBAC)**: Admin, User, Viewer, Auditor roles
- **Session management** with secure httpOnly cookies (no localStorage for tokens)
- **Automatic session timeout** and re-authentication

### Communication Security
- **HTTPS/TLS 1.3** exclusively (no HTTP in production)
- **Content Security Policy (CSP)** headers
- **Subresource Integrity (SRI)** for external dependencies
- **WebSocket over TLS** for real-time data streaming
- **Certificate pinning** for API connections

### Data Protection
- **Field-level encryption** display (data encrypted at rest in backend)
- **Encrypted telemetry** transmission over TLS
- **CSRF protection** with tokens for state-changing operations
- **XSS prevention** through input sanitization and output encoding
- **Rate limiting** on API calls (60 requests/minute per user)

### Security Monitoring Dashboard
- **Device attestation status** display (secure boot, firmware integrity)
- **Tamper alert viewer** with real-time notifications
- **Audit log viewer** for all administrative actions and security events
- **Certificate expiry dashboard** with renewal reminders
- **Security health score** per device and system-wide
- **Compliance metrics** (SABS/ICASA/SOC2 readiness indicators)

### Security Audit & Compliance
- **Audit logging** for all user actions, device commands, configuration changes
- **Security event tracking**: Failed logins, invalid signatures, tamper detections
- **Compliance reports**: SOC2 Type II controls, ISO 27001 alignment
- **Export capabilities**: Encrypted CSV/PDF reports with digital signatures

For comprehensive security architecture and compliance details, see [docs/SECURITY.md](../docs/SECURITY.md).

## 🚀 Getting Started

### Prerequisites

- Node.js 18.x or higher
- npm 9.x or yarn 1.22.x
- Modern web browser (Chrome, Firefox, Safari, Edge)

**Security Prerequisites**:
- **OAuth2/SSO provider** account (Auth0, Google, Microsoft)
- **TLS certificates** for HTTPS (Let's Encrypt or commercial CA)
- **API base URL** with TLS 1.3 enabled
- **WebSocket endpoint** with TLS support

### Installation

1. **Navigate to web directory**:
   ```bash
   cd smartplugai/web
   ```

2. **Install dependencies**:
   ```bash
   npm install
   # or
   yarn install
   ```

3. **Set up environment variables**:
   ```bash
   cp .env.example .env.local
   # Edit .env.local with your configuration
   ```

4. **Run development server**:
   ```bash
   npm run dev
   # or
   yarn dev
   ```

5. **Open in browser**:
   Navigate to `http://localhost:3000`

## 🏗️ Project Structure

```
web/
├── public/                    # Static assets
│   ├── favicon.ico
│   └── logo.png
├── src/
│   ├── App.tsx               # Main app component
│   ├── main.tsx              # Entry point
│   ├── components/           # Reusable components
│   │   ├── layout/
│   │   │   ├── Header.tsx
│   │   │   ├── Sidebar.tsx
│   │   │   └── Footer.tsx
│   │   ├── charts/
│   │   │   ├── PowerChart.tsx
│   │   │   ├── UsageChart.tsx
│   │   │   └── PieChart.tsx
│   │   ├── devices/
│   │   │   ├── DeviceCard.tsx
│   │   │   ├── DeviceList.tsx
│   │   │   └── DeviceDetail.tsx
│   │   └── common/
│   │       ├── Button.tsx
│   │       ├── Card.tsx
│   │       └── Modal.tsx
│   ├── pages/                # Page components
│   │   ├── Dashboard.tsx
│   │   ├── Devices.tsx
│   │   ├── Analytics.tsx
│   │   ├── Settings.tsx
│   │   └── Login.tsx
│   ├── services/             # API & business logic
│   │   ├── api.ts
│   │   ├── deviceService.ts
│   │   ├── authService.ts
│   │   └── mockData.ts       # Mock data for development
│   ├── hooks/                # Custom React hooks
│   │   ├── useDevices.ts
│   │   ├── usePowerData.ts
│   │   └── useAuth.ts
│   ├── store/                # State management (Redux/Zustand)
│   │   ├── index.ts
│   │   ├── deviceSlice.ts
│   │   └── authSlice.ts
│   ├── types/                # TypeScript types
│   │   ├── device.ts
│   │   ├── user.ts
│   │   └── analytics.ts
│   ├── utils/                # Utility functions
│   │   ├── formatters.ts
│   │   ├── constants.ts
│   │   └── helpers.ts
│   └── styles/               # Global styles
│       ├── globals.css
│       └── theme.ts
├── package.json
├── tsconfig.json
├── vite.config.ts            # Build configuration
└── README.md                 # This file
```

## 🎨 Key Features to Implement

### Phase 1: Basic Dashboard (Before Hardware Ready)

Build these features with placeholder/mock data:

#### 1. Dashboard Overview
- [ ] **Summary cards**: Total devices, active devices, total power consumption, cost estimate, security health score
- [ ] **Real-time power chart**: Line chart showing aggregate power usage over time (encrypted data)
- [ ] **Device status grid**: Visual grid showing all devices, their states, and security status (attestation, tamper detection)
- [ ] **Recent alerts**: List of recent high-power events, anomalies, and security alerts (tamper attempts, failed attestations)

**Suggested Mock Implementation**:
```typescript
// src/services/mockData.ts
export const getMockDevices = () => [
  {
    id: '1',
    name: 'Living Room Lamp',
    status: 'online',
    power: 60.5,
    voltage: 120.2,
    current: 0.503,
    isOn: true,
  },
  {
    id: '2',
    name: 'Kitchen Coffee Maker',
    status: 'online',
    power: 1200.0,
    voltage: 119.8,
    current: 10.02,
    isOn: true,
  },
  // Add more mock devices...
];

export const getMockPowerHistory = () => {
  // Generate 24 hours of mock data
  const data = [];
  for (let i = 0; i < 24; i++) {
    data.push({
      timestamp: Date.now() - (24 - i) * 3600000,
      power: 500 + Math.random() * 1000,
    });
  }
  return data;
};
```

#### 2. Device Management Page
- [ ] **Device list table**: Sortable, filterable table of all devices
- [ ] **Device search**: Real-time search by name or ID
- [ ] **Device actions**: Turn on/off, rename, delete (with placeholder behavior)
- [ ] **Device details modal**: Show detailed info and settings for a device

#### 3. Analytics Page
- [ ] **Power consumption over time**: Interactive charts (day/week/month views)
- [ ] **Cost analysis**: Estimate electricity costs based on usage
- [ ] **Device comparison**: Compare power usage across devices
- [ ] **Usage patterns**: Identify peak usage times

**Recommended Chart Library**: 
- Recharts (simple, React-friendly)
- Chart.js with react-chartjs-2
- D3.js (for custom visualizations)

#### 4. Settings Page
- [ ] **User profile settings** (mock)
- [ ] **Notification preferences**
- [ ] **Display preferences** (units, time zone, theme)
- [ ] **API keys** (for future integrations)

#### 5. Authentication with OAuth2/SSO
- [ ] **Login page** with OAuth2/SSO support (Auth0, Google, Microsoft)
- [ ] **Mock authentication** (bypass real auth initially)
- [ ] **Protected routes** with role-based access control
- [ ] **Logout functionality**
- [ ] **2FA support** for admin accounts

### Phase 2: Backend Integration (After Hardware Ready)

- [ ] Replace mock data with real API calls over HTTPS
- [ ] Implement WebSocket over TLS for real-time device updates
- [ ] Add OAuth2/SSO user authentication (Auth0/Firebase)
- [ ] Implement device control with signed commands (relay on/off via API)
- [ ] Add data export (CSV, PDF reports) with encryption
- [ ] Implement role-based access control (admin, user, viewer roles)
- [ ] Add system health monitoring dashboard
- [ ] Display device security state: attestation status, tamper alerts, certificate expiry
- [ ] Implement audit log viewer for security events
- [ ] Add security dashboard with compliance metrics (SABS/ICASA readiness, SOC2 status)

## 📦 Recommended Tech Stack

### Core
- **React** 18.x (with TypeScript)
- **Vite** for fast builds and hot module replacement
- **React Router** for navigation

### UI Components
```bash
npm install @mui/material @emotion/react @emotion/styled
# or
npm install antd
# or
npm install @chakra-ui/react
```

### Charts & Visualization
```bash
npm install recharts
# or
npm install chart.js react-chartjs-2
```

### State Management
```bash
npm install zustand
# or
npm install @reduxjs/toolkit react-redux
```

### API Client
```bash
npm install axios
npm install react-query  # For data fetching and caching
```

### Forms & Validation
```bash
npm install react-hook-form
npm install zod  # For schema validation
```

### Date & Time
```bash
npm install date-fns
# or
npm install dayjs
```

## 🎯 First Tasks (Suggested)

If starting from scratch:

1. **Initialize React + TypeScript project**:
   ```bash
   npm create vite@latest . -- --template react-ts
   npm install
   ```

2. **Set up basic layout**:
   - Create Header, Sidebar, and Footer components
   - Set up routing with React Router
   - Implement responsive layout (mobile-friendly)

3. **Create mock data service**:
   - Define TypeScript interfaces for devices and power readings
   - Create functions to generate mock data
   - Simulate API delays with setTimeout

4. **Build dashboard page**:
   - Summary statistics cards
   - Power consumption chart with mock data
   - Device status grid

5. **Implement device list**:
   - Sortable table/grid of devices
   - Search and filter functionality
   - Click to view device details

6. **Add basic theming**:
   - Define color palette
   - Light/dark mode toggle
   - Responsive breakpoints

## 🧪 Testing

### Run Tests
```bash
npm run test
```

### Run Tests with Coverage
```bash
npm run test:coverage
```

### E2E Tests (with Playwright or Cypress)
```bash
npm run test:e2e
```

## 🏗️ Building for Production

### Create Production Build
```bash
npm run build
# Output: dist/
```

### Preview Production Build
```bash
npm run preview
```

### Analyze Bundle Size
```bash
npm run build -- --analyze
```

## 🎨 Design Guidelines

- **Responsive**: Support desktop (1920px), tablet (768px), and mobile (375px)
- **Accessible**: Follow WCAG 2.1 AA standards
- **Performance**: Lazy load components, optimize images, code splitting
- **Consistent**: Use a design system (Material UI, Ant Design, or custom)

### Color Palette Suggestion
```css
:root {
  --primary: #2563eb;      /* Blue */
  --secondary: #10b981;    /* Green */
  --warning: #f59e0b;      /* Orange */
  --danger: #ef4444;       /* Red */
  --background: #f9fafb;   /* Light gray */
  --surface: #ffffff;      /* White */
  --text-primary: #111827; /* Dark gray */
  --text-secondary: #6b7280; /* Medium gray */
}
```

## 📱 Responsive Breakpoints

```typescript
export const breakpoints = {
  mobile: '640px',
  tablet: '768px',
  laptop: '1024px',
  desktop: '1280px',
  wide: '1536px',
};
```

## 🔒 Security Considerations

- Use HTTPS exclusively in production (TLS 1.3)
- Implement CSRF protection with tokens
- Sanitize all user inputs to prevent XSS attacks
- Use secure authentication (OAuth2/SSO with Auth0 or Firebase)
- Store sensitive data securely (never in localStorage - use secure httpOnly cookies)
- Implement rate limiting on API calls
- Regular dependency updates for security patches
- Display security status: device attestation, tamper alerts, certificate expiry
- Implement audit logging for all administrative actions
- Use Content Security Policy (CSP) headers
- Enable Subresource Integrity (SRI) for external dependencies

## 📝 Environment Variables

Create `.env.local` file:

```env
VITE_API_BASE_URL=https://api.smartplugai.com
VITE_MQTT_BROKER_URL=wss://mqtt.smartplugai.com:9001
VITE_ENABLE_MOCK_DATA=true
VITE_AUTH_PROVIDER=auth0
VITE_AUTH0_DOMAIN=your-tenant.auth0.com
VITE_AUTH0_CLIENT_ID=your-client-id
VITE_WEBSOCKET_TLS_ENABLED=true
```

Access in code:
```typescript
const apiUrl = import.meta.env.VITE_API_BASE_URL;
```

## 🔗 Useful Resources

- [React Documentation](https://react.dev/)
- [TypeScript Handbook](https://www.typescriptlang.org/docs/)
- [Vite Guide](https://vitejs.dev/guide/)
- [React Router](https://reactrouter.com/)
- [Recharts Documentation](https://recharts.org/)
- [Material-UI](https://mui.com/)

## 🤝 Contributing

See the main [CONTRIBUTING.md](../CONTRIBUTING.md) for general guidelines.

For web-specific contributions:
- Follow React and TypeScript best practices
- Use functional components and hooks
- Write unit tests for components and utilities
- Run linter (`npm run lint`) before committing
- Update this README when adding features

## 🐛 Troubleshooting

### Port already in use
```bash
# Kill process on port 3000
lsof -ti:3000 | xargs kill -9
# Or use a different port
npm run dev -- --port 3001
```

### Build fails
```bash
# Clear cache and reinstall
rm -rf node_modules package-lock.json
npm install
```

---

**Let's create a powerful analytics dashboard for Smart Plug AI!** 📊⚡

For comprehensive security architecture, OAuth2/SSO configuration, RBAC implementation, audit logging details, and compliance guidelines (SABS/ICASA/SOC2), see [docs/SECURITY.md](../docs/SECURITY.md) and [docs/ROADMAP.md](../docs/ROADMAP.md) for Phase 1 (7 weeks) timeline with security milestones.
