# Smart Plug AI Architecture

High-level system architecture and design documentation for the Smart Plug AI platform.

## 🏗️ System Overview

Smart Plug AI is a distributed IoT system consisting of edge devices (ESP32-based smart plugs), cloud backend services, and client applications (mobile and web). The system enables real-time power monitoring, remote device control, and intelligent analytics.

## 📊 High-Level Architecture Diagram

```
┌─────────────────────────────────────────────────────────────────────────┐
│                   SECURE PRESENTATION LAYER                             │
│                                                                         │
│  ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐       │
│  │   Mobile App    │  │  Admin Dashboard│  │  Public API     │       │
│  │   (Flutter)     │  │  (React)        │  │  (REST/GraphQL) │       │
│  │  - TLS 1.3      │  │  - TLS 1.3      │  │  - TLS 1.3      │       │
│  │  - Cert Pinning │  │  - OAuth2/SSO   │  │  - API Keys     │       │
│  │  - 2FA          │  │  - RBAC         │  │  - Rate Limiting│       │
│  └────────┬────────┘  └────────┬────────┘  └────────┬────────┘       │
└───────────┼─────────────────────┼─────────────────────┼────────────────┘
            │                     │                     │
            │                     └──────────┬──────────┘
            │                                │
            ▼                                ▼
┌─────────────────────────────────────────────────────────────────────────┐
│                   SECURE APPLICATION LAYER                              │
│                                                                         │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐  ┌───────────┐ │
│  │  FastAPI     │  │  Auth0       │  │  Redis Cache │  │  MQTT     │ │
│  │  (Python)    │  │  (OAuth2)    │  │  (Encrypted) │  │  Broker   │ │
│  │  - Input Val │  │  - 2FA/MFA   │  │  - Sessions  │  │  - TLS 1.3│ │
│  │  - Rate Limit│  │  - JWT Tokens│  │  - Rate Ctr  │  │  - mTLS   │ │
│  │  - RBAC      │  │  - RBAC      │  │  - Telemetry │  │  - ACL    │ │
│  └──────┬───────┘  └──────┬───────┘  └──────┬───────┘  └─────┬─────┘ │
└─────────┼──────────────────┼──────────────────┼────────────────┼───────┘
          │                  │                  │                │
          │                  └──────────┬───────┘                │
          │                             │                        │
          ▼                             ▼                        ▼
┌─────────────────────────────────────────────────────────────────────────┐
│                       SECURE DATA LAYER                                 │
│                                                                         │
│  ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐       │
│  │  Snowflake/     │  │  PostgreSQL /   │  │  InfluxDB       │       │
│  │  (OLAP)         │  │  Firestore      │  │  (Time-series)  │       │
│  │  - Row-Level    │  │  - Field-Level  │  │  - Retention    │       │
│  │    Security     │  │    Encryption   │  │  - Downsampling │       │
│  │  - Aggregates   │  │  - Users, Dev.  │  │  - Encrypted    │       │
│  └─────────────────┘  └─────────────────┘  └─────────────────┘       │
│                                                                         │
│  ┌─────────────────┐  ┌─────────────────────────────────────────┐    │
│  │  S3 / Cloud     │  │  KMS (Key Management Service)           │    │
│  │  Storage        │  │  - AWS KMS / Google Cloud KMS           │    │
│  │  - ML Data      │  │  - Quarterly Key Rotation               │    │
│  │  - Encrypted    │  │  - HSM-backed for production            │    │
│  └─────────────────┘  └─────────────────────────────────────────┘    │
└─────────────────────────────┬───────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────────────┐
│                       SECURE DEVICE LAYER (Edge)                        │
│                                                                         │
│  ┌──────────────────────────────────────────────────────────┐         │
│  │                    ESP32-S3 + Security ICs                │         │
│  │                                                            │         │
│  │  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐   │         │
│  │  │  ESP32-S3    │  │  ATECC608A   │  │  MAX6316     │   │         │
│  │  │  - Secure    │  │  - ECDSA P256│  │  - Tamper    │   │         │
│  │  │    Boot      │  │  - Key Slots │  │    Watchdog  │   │         │
│  │  │  - Flash     │  │  - Hardware  │  │  - Reset on  │   │         │
│  │  │    Encrypt   │  │    RNG       │  │    Tamper    │   │         │
│  │  └──────┬───────┘  └──────┬───────┘  └──────┬───────┘   │         │
│  │         └──────────────────┴──────────────────┘           │         │
│  │                          │                                 │         │
│  │  ┌──────────────┐  ┌────┴────────┐  ┌──────────────┐   │         │
│  │  │  24C256      │  │  SCT-013    │  │  Relay Module│   │         │
│  │  │  - Encrypted │  │  - Current  │  │  + PC817     │   │         │
│  │  │    EEPROM    │  │    Sensor   │  │  - Optoisol. │   │         │
│  │  └──────────────┘  └─────────────┘  └──────────────┘   │         │
│  └──────────────────────────────────────────────────────────┘         │
│           │  MQTT over TLS 1.3 (Port 8883)                             │
│           │  Client Certificates (mTLS)                                │
│           │  Signed Commands (ECDSA)                                   │
└───────────┴─────────────────────────────────────────────────────────────┘
```

## 🔧 Component Responsibilities

### Device Layer (ESP32-S3 + Security ICs)

**Responsibilities**:

- Read power metrics from SCT-013 current sensor (30A/1V output)
- Control relay module based on signed commands with signature verification
- Publish encrypted telemetry data to MQTT broker (1-second intervals)
- Subscribe to control commands from MQTT broker and verify signatures
- Implement secure OTA firmware update mechanism with code signing
- Handle Wi-Fi connectivity and secure reconnection (encrypted credentials)
- Maintain device status and attestation reporting
- Execute fail-safe mechanisms (watchdog, tamper detection, brownout detection)
- Generate device attestation reports (firmware hash, secure boot status)

**Security Features**:
- **Secure Boot V2**: RSA-PSS 3072-bit signature verification on boot
- **Flash Encryption**: AES-256-XTS encryption for all flash contents
- **ATECC608A Integration**: ECDSA P256 operations for signing and verification
- **Tamper Detection**: MAX6316 watchdog monitors for physical tampering
- **Encrypted Storage**: 24C256 EEPROM with AES-256-GCM encryption
- **Client Certificates**: mTLS authentication with MQTT broker
- **Command Verification**: ECDSA signature verification for all commands

**Technology**:

- ESP32-S3 microcontroller (dual-core Xtensa LX7, 240MHz, hardware crypto)
- ESP-IDF framework (v5.0+) with secure boot and flash encryption
- ATECC608A secure element (I2C, address 0x60)
- SCT-013-030 current sensor (30A/1V) with 33Ω burden resistor
- 24C256 EEPROM (32KB, I2C, address 0x50)
- MAX6316 tamper watchdog (reset on tamper, battery-backed)
- MQTT client library with TLS 1.3 support (ESP-MQTT)

### Message Broker Layer (MQTT over TLS)

**Responsibilities**:

- Route messages between devices and backend over secure channels
- Maintain persistent TLS connections with mutual authentication
- Handle message queuing and guaranteed delivery (QoS 0, 1)
- Support Last Will and Testament (LWT) for device offline detection
- Provide WebSocket over TLS support for browser clients
- Enforce authentication via client certificates (mTLS)
- Enforce authorization via Access Control Lists (ACLs)
- Scale to thousands of concurrent connections with load balancing

**Security Features**:
- **TLS 1.3**: Latest protocol with perfect forward secrecy
- **Client Certificates**: Mutual TLS (mTLS) with ATECC608A-generated device certs
- **Access Control Lists**: Per-device topic restrictions
- **Certificate Validation**: CRL and OCSP checking for revoked certificates
- **Cipher Suites**: TLS_AES_256_GCM_SHA384, TLS_CHACHA20_POLY1305_SHA256
- **Rate Limiting**: Connection and message rate limits per device

**Technology**:

- Eclipse Mosquitto MQTT broker 2.0+ (with TLS plugin)
- TLS/SSL with OpenSSL 3.0+ (TLS 1.3 support)
- WebSocket bridge over TLS for browsers (port 9001)
- Access Control Lists (ACLs) with dynamic authorization
- Certificate-based authentication (X.509v3 with ECDSA P-256)
- CRL and OCSP for certificate revocation checking

### Backend Layer (Cloud Services)

#### MQTT Subscriber Service

**Responsibilities**:

- Subscribe to all device telemetry topics (`smartplug/+/telemetry`)
- Parse and validate incoming telemetry messages
- Write time-series data to InfluxDB
- Update device metadata in PostgreSQL (last_seen, status)
- Trigger real-time alerts based on thresholds
- Forward data to WebSocket clients for live updates
- Export data to S3 for ML pipeline

#### REST API Service

**Responsibilities**:

- Authenticate users (JWT tokens with Auth0/Firebase Auth + 2FA)
- Manage user accounts and profiles with RBAC
- Provide device CRUD operations with ownership validation
- Query historical telemetry data (encrypted)
- Send signed control commands to devices via MQTT
- Manage device schedules and automation rules with rate limiting
- Generate reports and analytics with field-level encryption
- Expose OpenAPI documentation (Swagger/ReDoc)
- Audit logging for all operations

**Security Features**:
- **TLS 1.3**: HTTPS exclusively
- **Input Validation**: Pydantic schemas prevent injection attacks
- **Rate Limiting**: Distributed rate limiting with Redis
- **CORS**: Strict whitelist-only origin policy
- **CSRF Protection**: Token-based protection
- **Command Signing**: ECDSA signatures for all device commands
- **Audit Logging**: Comprehensive logging of all actions

**Endpoints**:

- `/auth/*` - Authentication and authorization (2FA, JWT, OAuth2)
- `/devices/*` - Device management and control (signed commands)
- `/telemetry/*` - Historical data queries (field-level decryption)
- `/users/*` - User management (RBAC enforcement)
- `/schedules/*` - Automation rules (validated and rate-limited)
- `/alerts/*` - Alert configuration (tamper alerts, thresholds)
- `/attestation/*` - Device attestation and firmware verification
- `/certificates/*` - Certificate lifecycle management (renewal, revocation)

#### WebSocket Service

**Responsibilities**:

- Maintain persistent connections with web/mobile clients
- Broadcast real-time telemetry updates
- Stream device status changes
- Support room-based subscriptions (per device or user)
- Handle connection lifecycle (connect, disconnect, reconnect)

### Storage Layer

#### InfluxDB (Time-Series)

**Responsibilities**:

- Store high-frequency telemetry data (1-second granularity)
- Provide efficient time-range queries
- Perform automatic downsampling (1m, 5m, 1h, 1d aggregates)
- Implement retention policies (7d raw, 30d 1m, 1y 1h, 5y 1d)
- Support Flux queries for complex analytics

**Schema**:

- Measurement: `power_metrics`
- Tags: `device_id`, `location`, `device_type`
- Fields: `voltage`, `current`, `power`, `relay_state`
- Timestamp: Nanosecond precision

#### PostgreSQL (Relational)

**Responsibilities**:

- Store user accounts and authentication data
- Maintain device registry and metadata
- Store device configurations and schedules
- Track user-device associations (many-to-many)
- Maintain audit logs and alert history
- Support complex relational queries

**Tables**:

- `users` - User accounts
- `devices` - Device metadata
- `device_users` - Device sharing
- `schedules` - Automation rules
- `alerts` - Alert configurations
- `audit_logs` - System events

#### Redis (Cache & Sessions)

**Responsibilities**:

- Cache frequently accessed data (latest telemetry)
- Store user session tokens
- Implement rate limiting counters
- Track active WebSocket connections
- Queue background tasks
- Provide pub/sub for internal messaging

### Client Layer

#### Mobile App (Flutter)

**Responsibilities**:

- Authenticate users and manage sessions
- Display real-time power monitoring
- Control devices (on/off, schedules)
- Show historical data and analytics
- Handle QR code pairing
- Receive push notifications
- Support offline mode with local cache

#### Web Dashboard (React)

**Responsibilities**:

- Provide admin interface for device management
- Display comprehensive analytics and reports
- Manage multiple devices at scale
- Configure system settings and alerts
- Export data (CSV, PDF)
- Real-time monitoring with WebSocket

#### ML Pipeline (Python)

**Responsibilities**:

- Batch data export from InfluxDB
- Feature engineering (time-based, statistical)
- Train anomaly detection models
- Train usage prediction models
- Serve inference endpoints
- Continuous model monitoring and retraining

## 🔄 Data Flow Examples

### Telemetry Ingestion Flow

```
┌─────────────┐
│ ESP32       │  1. Read sensor every 1 second
│ Device      │     (voltage, current, power)
└──────┬──────┘
       │
       │  2. Publish to MQTT
       │     Topic: smartplug/{device_id}/telemetry
       │     Payload: {"voltage": 120.2, "current": 0.5, ...}
       │
       ▼
┌─────────────┐
│ MQTT Broker │  3. Route message to subscribers
└──────┬──────┘
       │
       │  4. Backend MQTT subscriber receives message
       │
       ▼
┌─────────────┐
│ Backend     │  5. Parse and validate JSON
│ Subscriber  │  6. Write to InfluxDB (time-series)
└──────┬──────┘  7. Update PostgreSQL (last_seen)
       │         8. Check alert thresholds
       │         9. Broadcast to WebSocket clients
       │
       ├──────────────────┬──────────────────┐
       ▼                  ▼                  ▼
┌─────────────┐   ┌─────────────┐   ┌─────────────┐
│ InfluxDB    │   │ PostgreSQL  │   │ WebSocket   │
│ (telemetry) │   │ (metadata)  │   │ (real-time) │
└─────────────┘   └─────────────┘   └──────┬──────┘
                                            │
                                            │  10. Push to connected clients
                                            ▼
                                    ┌─────────────┐
                                    │ Mobile App  │
                                    │ Web Dashboard│
                                    └─────────────┘
```

### Device Control Command Flow

```
┌─────────────┐
│ Mobile App  │  1. User taps "Turn On" button
│             │
└──────┬──────┘
       │
       │  2. HTTP POST /devices/{id}/command
       │     Body: {"command": "relay_on"}
       │     Headers: Authorization: Bearer <token>
       │
       ▼
┌─────────────┐
│ FastAPI     │  3. Validate JWT token
│ Backend     │  4. Check user has access to device
└──────┬──────┘  5. Publish MQTT command
       │
       │  6. MQTT Publish
       │     Topic: smartplug/{device_id}/control
       │     Payload: {"command": "relay_on"}
       │
       ▼
┌─────────────┐
│ MQTT Broker │  7. Route message to device
└──────┬──────┘
       │
       │  8. Device receives command
       │
       ▼
┌─────────────┐
│ ESP32       │  9. Parse command
│ Device      │  10. Set GPIO HIGH (relay ON)
└──────┬──────┘  11. Update internal state
       │
       │  12. Publish status update
       │      Topic: smartplug/{device_id}/status
       │      Payload: {"status": "on", ...}
       │
       ▼
┌─────────────┐
│ MQTT Broker │  13. Route status to backend
└──────┬──────┘
       │
       ▼
┌─────────────┐
│ Backend     │  14. Update device state in PostgreSQL
│ Subscriber  │  15. Broadcast to WebSocket clients
└──────┬──────┘
       │
       │  16. Real-time update
       ▼
┌─────────────┐
│ Mobile App  │  17. UI updates to show "ON" state
│             │
└─────────────┘
```

### User Authentication Flow

```
┌─────────────┐
│ Mobile App  │  1. User enters email/password
│             │
└──────┬──────┘
       │
       │  2. HTTP POST /auth/login
       │     Body: {"email": "...", "password": "..."}
       │
       ▼
┌─────────────┐
│ FastAPI     │  3. Query user from PostgreSQL
│ Backend     │  4. Verify password hash (bcrypt)
└──────┬──────┘  5. Generate JWT token (30min expiry)
       │         6. Store refresh token in Redis
       │
       │  7. Return tokens
       │     Response: {
       │       "access_token": "eyJ...",
       │       "refresh_token": "...",
       │       "token_type": "bearer"
       │     }
       │
       ▼
┌─────────────┐
│ Mobile App  │  8. Store tokens securely (keychain)
│             │  9. Include in Authorization header
└─────────────┘     for subsequent requests

┌─────────────┐
│ Mobile App  │  10. Make authenticated request
│             │      Headers: Authorization: Bearer <token>
└──────┬──────┘
       │
       ▼
┌─────────────┐
│ FastAPI     │  11. Decode and verify JWT
│ Backend     │  12. Extract user_id from token
└──────┬──────┘  13. Check token expiry
       │         14. Proceed with request if valid
       │
       ▼
     Success
```

### Device Pairing Flow

```
┌─────────────┐
│ ESP32       │  1. Generate QR code with device_id
│ Device      │     and pairing_code (displayed on screen
└─────────────┘     or printed on device)

┌─────────────┐
│ Mobile App  │  2. User scans QR code
│             │  3. Extract device_id and pairing_code
└──────┬──────┘
       │
       │  4. HTTP POST /devices
       │     Body: {
       │       "device_id": "smartplug_001",
       │       "name": "Living Room Lamp",
       │       "pairing_code": "ABC123"
       │     }
       │     Headers: Authorization: Bearer <token>
       │
       ▼
┌─────────────┐
│ FastAPI     │  5. Verify pairing_code (stored in device)
│ Backend     │  6. Create device record in PostgreSQL
└──────┬──────┘  7. Associate device with user_id
       │         8. Publish config to device via MQTT
       │
       │  9. Response: device details
       │
       ▼
┌─────────────┐
│ Mobile App  │  10. Add device to local list
│             │  11. Subscribe to device telemetry
└─────────────┘
```

## 🔒 Security Considerations

### Authentication & Authorization

- **User Authentication**: JWT tokens with RS256 or HS256 signing, Auth0/Firebase Auth
- **Token Expiry**: Short-lived access tokens (30 minutes), refresh tokens (7 days) in Redis
- **2FA/MFA**: TOTP or SMS for admin accounts and sensitive operations
- **Device Authentication**: 
  - Unique device ID + ECDSA private key (ATECC608A Slot 0)
  - Client certificates (X.509v3 with ECDSA P-256)
  - mTLS for MQTT connections
- **API Key**: Optional API keys for third-party integrations with scoped permissions
- **RBAC**: Role-Based Access Control (Admin, User, Viewer, Service, Auditor)

### Data Encryption

- **In Transit**:
  - TLS 1.3 for HTTPS (API, WebSocket)
  - MQTT over TLS (MQTTS on port 8883) with mTLS
  - Certificate pinning for mobile apps
  - Perfect forward secrecy (PFS) with ephemeral keys
  - Strong cipher suites: TLS_AES_256_GCM_SHA384, TLS_CHACHA20_POLY1305_SHA256
  
- **At Rest**:
  - Encrypted database volumes (AES-256)
  - Field-level encryption for sensitive data (AES-256-GCM with KMS)
  - Encrypted S3 buckets for ML data (server-side encryption)
  - Encrypted secrets in Kubernetes Secrets or AWS Secrets Manager
  - Encrypted backups with separate keys
  - Device flash encryption (AES-256-XTS on ESP32-S3)
  - Encrypted EEPROM (24C256 with AES-256-GCM)

### Network Security

- **Firewall Rules**: Restrict access to backend services (allowlist only)
- **Rate Limiting**: Prevent abuse and DDoS attacks
  - Authentication: 5 requests/15 minutes
  - API calls: 60 requests/minute per user
  - Device commands: 10 requests/minute per device
- **DDoS Protection**: Use Cloudflare or AWS Shield
- **IP Whitelisting**: Optional for admin endpoints
- **Security Headers**: CSP, HSTS, X-Frame-Options, X-Content-Type-Options

### Input Validation

- **API Endpoints**: Pydantic schemas for request validation
- **MQTT Messages**: JSON schema validation
- **SQL Injection**: Parameterized queries (SQLAlchemy ORM only)
- **XSS Prevention**: Sanitize outputs in web dashboard
- **Command Injection**: No shell execution, validated inputs only

### Device Security

- **Signed Commands**: All commands signed with ECDSA (server private key)
- **Command Verification**: Device verifies signature using server public key (ATECC608A Slot 1)
- **Nonce + Timestamp**: Prevent replay attacks (commands expire after 5 minutes)
- **OTA Security**: Verify firmware signature before flashing (RSA-3072 or ECDSA P-256)
- **Secure Boot**: ESP32-S3 secure boot v2 enabled in production
- **Flash Encryption**: Encrypt firmware and data in flash memory
- **Credential Storage**: ESP32 NVS encryption for Wi-Fi credentials
- **Device Attestation**: Periodic firmware integrity checks (SHA-256 hash)
- **Tamper Detection**: MAX6316 watchdog monitors for physical tampering

### Access Control

- **Role-Based Access Control (RBAC)**:
  - **Admin**: Full system access, user management, security settings
  - **User**: Access to owned devices, device control, data viewing
  - **Viewer**: Read-only access to shared devices, no control
  - **Service**: Backend service accounts, API-only access
  - **Auditor**: Security log access, no device control

- **Device Ownership**: Users can only control devices they own or are shared with
- **Multi-tenancy**: Logical separation of user data
- **Least Privilege**: Each role has minimum necessary permissions

## 📈 Scalability Considerations

### Horizontal Scaling

- **API Service**: Stateless design, scale with load balancer
- **MQTT Broker**: Cluster Mosquitto instances with shared subscriptions
- **Database**: PostgreSQL read replicas, InfluxDB clustering
- **Redis**: Redis Cluster for distributed cache

### Vertical Scaling

- **Database**: Increase compute resources for heavy queries
- **MQTT Broker**: Increase connections per broker instance

### Performance Optimization

- **Caching**: Use Redis for frequently accessed data
- **Database Indexing**: Index commonly queried fields
- **Connection Pooling**: Reuse database connections
- **Async Processing**: Use background tasks for heavy operations
- **CDN**: Serve static assets from CDN

### Monitoring & Alerting

- **Metrics**: Prometheus for system metrics
- **Visualization**: Grafana dashboards
- **Logging**: Centralized logging (ELK stack or CloudWatch)
- **Alerting**: PagerDuty or Opsgenie for critical issues
- **Tracing**: Jaeger or Zipkin for distributed tracing

## 🧪 Testing Strategy

- **Unit Tests**: Test individual functions and classes
- **Integration Tests**: Test API endpoints and database interactions
- **E2E Tests**: Test complete user flows (mobile, web)
- **Load Tests**: Simulate thousands of devices and users
- **Security Tests**: Penetration testing and vulnerability scanning
- **Hardware Tests**: Test firmware with real ESP32 devices

## 🚀 Deployment Strategy

- **Development**: Docker Compose on local machine
- **Staging**: Kubernetes cluster with isolated namespace
- **Production**: Kubernetes with auto-scaling and rolling updates
- **CI/CD**: GitHub Actions for automated testing and deployment
- **Blue-Green Deployment**: Zero-downtime deployments
- **Canary Releases**: Gradual rollout of new features

## 📖 Related Documentation

- [Firmware README](../firmware/README.md) - ESP32 firmware development
- [Backend README](../backend/README.md) - Backend service setup
- [API Documentation](API.md) - Detailed API reference
- [Mobile App README](../app/README.md) - Flutter app development
- [Web Dashboard README](../web/README.md) - React dashboard development

---

**Architecture is a living document**: This design will evolve as the system grows and new requirements emerge.
