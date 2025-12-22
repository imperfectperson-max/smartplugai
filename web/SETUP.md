# Smart Plug AI Web Dashboard - Setup Guide

## Pre-Hardware Development Status ✅

The web dashboard is ready for development with **mock data and full UI**. All security features are designed in and ready for backend integration.

## What's Implemented

### ✅ Complete Features (Mock)
- **Authentication**: OAuth2/SSO login mockup with 2FA
- **Dashboard**: Real-time overview with security metrics
- **Device Management**: Full device table with control actions
- **Analytics**: Power consumption charts and device usage breakdown
- **Security Dashboard**: Security alerts and audit log viewer
- **Settings**: User profile and security configuration

### 🎨 UI Components
- Responsive layout with sidebar navigation
- Real-time power charts (Recharts)
- Security alert system with severity levels
- Device status grid
- Audit log table

## Setup Instructions

### Prerequisites
- Node.js 18+
- npm 9+ or yarn 1.22+

### Installation

1. **Install dependencies**:
   ```bash
   cd web
   npm install
   ```

2. **Run development server**:
   ```bash
   npm run dev
   ```
   
   Open [http://localhost:3000](http://localhost:3000) in your browser.

3. **Mock Login**:
   - Email: any valid email format
   - Password: any 6+ character password
   - 2FA Code: any 6-digit code

### Build for Production

```bash
npm run build
npm run preview
```

## Project Structure

```
web/
├── src/
│   ├── main.tsx              # Entry point
│   ├── App.tsx               # App router and routes
│   ├── components/
│   │   └── layout/          # Header, Sidebar, Layout
│   ├── pages/               # Page components
│   │   ├── LoginPage.tsx   # OAuth2/2FA login
│   │   ├── Dashboard.tsx   # Main dashboard
│   │   ├── Devices.tsx     # Device management
│   │   ├── Analytics.tsx   # Charts and analytics
│   │   ├── Security.tsx    # Security alerts & audit log
│   │   └── Settings.tsx    # User settings
│   ├── services/            # Business logic
│   │   ├── authService.ts  # Mock auth with 2FA
│   │   ├── deviceService.ts # Mock device API
│   │   └── mockData.ts     # Mock data generators
│   ├── types/              # TypeScript definitions
│   │   ├── device.ts
│   │   ├── user.ts
│   │   └── security.ts
│   └── styles/
│       └── globals.css     # Global styles
├── package.json
├── vite.config.ts
└── tsconfig.json
```

## Available Scripts

- `npm run dev` - Start development server
- `npm run build` - Build for production
- `npm run preview` - Preview production build
- `npm run lint` - Run ESLint

## Features Overview

### Dashboard Page
- Summary cards: Total devices, power usage, security score, active alerts
- 24-hour power consumption chart (encrypted data)
- Device status grid with security indicators
- Real-time security alerts

### Devices Page
- Sortable device table
- Security status columns (encryption, attestation, tamper)
- Device control actions (ON/OFF)
- Device metrics (power, voltage, current)

### Analytics Page
- Hourly power consumption bar chart
- Energy usage pie chart by device
- Exportable data (planned)

### Security Page
- Security alerts table with severity levels
- Audit log with user actions and timestamps
- Tamper detection monitoring
- Device attestation status

### Settings Page
- User profile information
- Security configuration display
- Application information

## Next Steps for Production

When backend is ready:

1. **Replace Mock Services**:
   - Integrate Auth0 or Firebase Auth with real OAuth2
   - Connect to backend REST API over HTTPS
   - Implement WebSocket over TLS for real-time updates

2. **Security Integration**:
   - Enable certificate pinning
   - Implement secure httpOnly cookies (no localStorage for tokens)
   - Add CSP headers
   - Enable rate limiting

3. **Real Data Sources**:
   - Connect to MQTT broker over TLS
   - Fetch device data from API
   - Stream telemetry via WebSocket

4. **Production Deployment**:
   - Configure environment variables
   - Set up reverse proxy (Nginx) with TLS
   - Deploy to cloud (AWS, GCP, Azure)

## Environment Variables

Copy `.env.example` to `.env.local` and configure:

```env
VITE_API_BASE_URL=https://api.smartplugai.com
VITE_ENABLE_MOCK_DATA=true
VITE_AUTH_PROVIDER=auth0
VITE_TLS_ENABLED=true
```

## Security Features (Designed)

- **TLS 1.3**: All communications encrypted (badge in header)
- **2FA**: Two-factor authentication flow
- **Encryption Indicators**: Blue badges on encrypted data
- **Attestation Monitoring**: Device integrity verification display
- **Tamper Alerts**: Real-time tamper detection notifications
- **Audit Logging**: Complete user action tracking
- **Role-Based Access**: Admin, User, Viewer, Auditor roles

## Development Notes

- Using **Vite** for fast development and optimized builds
- **TypeScript** for type safety
- **Recharts** for data visualization
- **React Router** for navigation
- Currently using **mock data** - all services return simulated responses
- Responsive design works on desktop and tablet

## Troubleshooting

**Port 3000 already in use**:
```bash
# Kill process on port 3000
lsof -ti:3000 | xargs kill -9

# Or use different port
npm run dev -- --port 3001
```

**Build fails**:
```bash
rm -rf node_modules package-lock.json
npm install
```

**TypeScript errors**:
```bash
npm run lint
```

## Resources

- [React Documentation](https://react.dev/)
- [Vite Guide](https://vitejs.dev/guide/)
- [Recharts Documentation](https://recharts.org/)
- [Project SECURITY.md](../docs/SECURITY.md) - Security architecture
- [Project ROADMAP.md](../docs/ROADMAP.md) - Development timeline
