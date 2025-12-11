# Smart Plug AI Architecture

High-level system architecture and design documentation for the Smart Plug AI platform.

## 🏗️ System Overview

Smart Plug AI is a distributed IoT system consisting of edge devices (ESP32-based smart plugs), cloud backend services, and client applications (mobile and web). The system enables real-time power monitoring, remote device control, and intelligent analytics.

## 📊 High-Level Architecture Diagram

```
┌─────────────────────────────────────────────────────────────────────────┐
│                            DEVICE LAYER (Edge)                          │
│                                                                         │
│  ┌──────────────────┐      ┌──────────────────┐                       │
│  │   ESP32 Device   │      │   ESP32 Device   │                       │
│  │  + INA219        │      │  + INA219        │  ... (N devices)      │
│  │  + Relay Module  │      │  + Relay Module  │                       │
│  └────────┬─────────┘      └────────┬─────────┘                       │
│           │                         │                                  │
│           │  Wi-Fi (MQTT over TLS)  │                                  │
└───────────┼─────────────────────────┼──────────────────────────────────┘
            │                         │
            └────────────┬────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────────────────────┐
│                       MESSAGE BROKER LAYER                              │
│                                                                         │
│                   ┌─────────────────────────┐                          │
│                   │   MQTT Broker           │                          │
│                   │   (Mosquitto)           │                          │
│                   │   - Port 1883 (MQTT)    │                          │
│                   │   - Port 8883 (MQTTS)   │                          │
│                   │   - Port 9001 (WS)      │                          │
│                   └──────────┬──────────────┘                          │
└──────────────────────────────┼──────────────────────────────────────────┘
                               │
              ┌────────────────┼────────────────┐
              │                │                │
              ▼                ▼                ▼
┌─────────────────────────────────────────────────────────────────────────┐
│                        BACKEND LAYER (Cloud)                            │
│                                                                         │
│  ┌────────────────┐  ┌────────────────┐  ┌─────────────────┐          │
│  │  MQTT          │  │  FastAPI       │  │  WebSocket      │          │
│  │  Subscriber    │  │  REST API      │  │  Server         │          │
│  │  (Ingestion)   │  │  (v1)          │  │  (Real-time)    │          │
│  └───────┬────────┘  └───────┬────────┘  └────────┬────────┘          │
│          │                   │                     │                   │
│          │    ┌──────────────┴──────────────┐      │                   │
│          │    │                             │      │                   │
│          ▼    ▼                             ▼      ▼                   │
│  ┌──────────────────┐              ┌──────────────────┐               │
│  │   InfluxDB       │              │   PostgreSQL     │               │
│  │   (Time-series)  │              │   (Metadata)     │               │
│  │   - Telemetry    │              │   - Users        │               │
│  │   - Metrics      │              │   - Devices      │               │
│  │   - Aggregates   │              │   - Schedules    │               │
│  └──────────────────┘              └──────────────────┘               │
│                                                                         │
│          ┌──────────────────┐              ┌──────────────────┐       │
│          │   Redis          │              │   S3 / Storage   │       │
│          │   (Cache)        │              │   (ML Data)      │       │
│          │   - Sessions     │              │   - Exports      │       │
│          │   - Rate Limit   │              │   - Backups      │       │
│          └──────────────────┘              └──────────────────┘       │
│                                                                         │
└─────────────────────────┬───────────────────────────────────────────────┘
                          │
          ┌───────────────┼───────────────┐
          │               │               │
          ▼               ▼               ▼
┌─────────────────────────────────────────────────────────────────────────┐
│                        CLIENT LAYER                                     │
│                                                                         │
│  ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐        │
│  │   Mobile App    │  │  Web Dashboard  │  │  ML Pipeline    │        │
│  │   (Flutter)     │  │  (React)        │  │  (Python)       │        │
│  │  - iOS          │  │  - Analytics    │  │  - Training     │        │
│  │  - Android      │  │  - Admin Panel  │  │  - Inference    │        │
│  └─────────────────┘  └─────────────────┘  └─────────────────┘        │
│                                                                         │
└─────────────────────────────────────────────────────────────────────────┘
```

## 🔧 Component Responsibilities

### Device Layer (ESP32)

**Responsibilities**:

- Read power metrics from INA219 sensor (voltage, current, power)
- Control relay module based on commands
- Publish telemetry data to MQTT broker (1-second intervals)
- Subscribe to control commands from MQTT broker
- Implement OTA firmware update mechanism
- Handle Wi-Fi connectivity and reconnection
- Maintain device status (online/offline)
- Execute fail-safe mechanisms (watchdog, brownout detection)

**Technology**:

- ESP32 microcontroller (dual-core, 240MHz)
- Arduino framework / ESP-IDF
- INA219 I2C driver
- MQTT client library (PubSubClient)
- ArduinoOTA for updates

### Message Broker Layer (MQTT)

**Responsibilities**:

- Route messages between devices and backend
- Maintain persistent connections with devices
- Handle message queuing and delivery (QoS 0, 1, 2)
- Support Last Will and Testament (LWT) for device offline detection
- Provide WebSocket support for browser clients
- Enforce authentication and authorization
- Scale to thousands of concurrent connections

**Technology**:

- Eclipse Mosquitto MQTT broker
- TLS/SSL for secure communication
- WebSocket bridge for browsers
- Access Control Lists (ACLs)

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

- Authenticate users (JWT tokens)
- Manage user accounts and profiles
- Provide device CRUD operations
- Query historical telemetry data
- Send control commands to devices via MQTT
- Manage device schedules and automation rules
- Generate reports and analytics
- Expose OpenAPI documentation

**Endpoints**:

- `/auth/*` - Authentication and authorization
- `/devices/*` - Device management and control
- `/telemetry/*` - Historical data queries
- `/users/*` - User management
- `/schedules/*` - Automation rules
- `/alerts/*` - Alert configuration

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

- **User Authentication**: JWT tokens with RS256 or HS256 signing
- **Token Expiry**: Short-lived access tokens (30 minutes)
- **Refresh Tokens**: Long-lived refresh tokens (7 days) stored in Redis
- **Device Authentication**: Unique device ID + secret key for MQTT
- **API Key**: Optional API keys for third-party integrations

### Data Encryption

- **In Transit**:
  - TLS 1.3 for HTTPS (API, WebSocket)
  - MQTT over TLS (MQTTS on port 8883)
  - Certificate pinning for mobile apps
  
- **At Rest**:
  - Encrypted database volumes (AES-256)
  - Encrypted S3 buckets for ML data
  - Encrypted secrets in Kubernetes Secrets or AWS Secrets Manager

### Network Security

- **Firewall Rules**: Restrict access to backend services
- **Rate Limiting**: Prevent abuse (100 req/min per user)
- **DDoS Protection**: Use Cloudflare or AWS Shield
- **IP Whitelisting**: Optional for admin endpoints

### Input Validation

- **API Endpoints**: Pydantic schemas for request validation
- **MQTT Messages**: JSON schema validation
- **SQL Injection**: Use parameterized queries (SQLAlchemy ORM)
- **XSS Prevention**: Sanitize outputs in web dashboard

### Device Security

- **OTA Security**: Verify firmware signature before flashing
- **Secure Boot**: Enable ESP32 secure boot in production
- **Flash Encryption**: Encrypt firmware in ESP32 flash memory
- **Credential Storage**: Use ESP32 NVS encryption for Wi-Fi credentials

### Access Control

- **Role-Based Access Control (RBAC)**:
  - Admin: Full system access
  - User: Access to owned devices
  - Guest: Read-only access to shared devices

- **Device Ownership**: Users can only control devices they own or are shared with
- **Multi-tenancy**: Logical separation of user data

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
