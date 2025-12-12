# Smart Plug AI Backend

Cloud-based backend service for device management, data ingestion, real-time analytics, and API services.

## 🎯 Purpose

The backend provides:

- **MQTT Broker**: Secure message broker for device-to-cloud communication over TLS 1.3 with client certificates
- **REST API**: Device management, telemetry query, and control endpoints with signed command support
- **WebSocket**: Real-time encrypted data streaming to web and mobile clients
- **Data Ingestion**: Process and store encrypted telemetry from smart plug devices
- **Data Storage**: Time-series data (InfluxDB) with field-level encryption and metadata (PostgreSQL/Firestore)
- **Authentication**: User authentication and authorization with Auth0/Firebase Auth, 2FA, and RBAC
- **Analytics**: Aggregation, pattern recognition, and anomaly detection
- **ML Pipeline Integration**: Data export and hooks for machine learning workflows
- **Security**: Rate limiting, audit logging, ATECC608A device provisioning endpoints, SOC2 compliance readiness

## 🏗️ Tech Stack

### Recommended Stack (Security-First)

- **API Framework**: FastAPI (Python 3.10+)
  - Async support for high throughput
  - Automatic OpenAPI documentation
  - Type validation with Pydantic
  - Built-in security features

- **MQTT Broker**: Eclipse Mosquitto
  - Lightweight and reliable
  - TLS 1.3 support with client certificate authentication
  - WebSocket over TLS support for browsers
  - ACL-based access control

- **Time-Series Database**: InfluxDB 2.x
  - Optimized for time-series data
  - Built-in downsampling and retention policies
  - Flux query language
  - Field-level encryption support

- **Relational Database**: PostgreSQL 14+ or Firestore
  - Device metadata, user accounts, configurations
  - Strong ACID guarantees (PostgreSQL)
  - Field-level encryption for sensitive data
  - JSON support for flexible schemas
  - RBAC and audit logging

- **Authentication**: Auth0 or Firebase Auth
  - OAuth2/OpenID Connect support
  - 2FA and MFA support
  - Social login providers
  - RBAC and fine-grained permissions
  - Session management and token refresh

- **Cache**: Redis 7+
  - Session management
  - Real-time data caching
  - Rate limiting counters
  - Encrypted data cache

- **Message Queue**: Redis Streams or RabbitMQ (optional)
  - Async task processing
  - ML pipeline integration

- **Container Orchestration**: Docker + Docker Compose (development)
  - Kubernetes for production deployment

### Alternative Stacks

- **Node.js + Express + Mongoose + MongoDB** (JavaScript ecosystem)
- **Django + Celery + Redis + TimescaleDB** (Python, PostgreSQL-based time-series)
- **Go + Gin + NATS + PostgreSQL** (High performance, compiled)

## 🚀 Quick Start

### Prerequisites

- Docker and Docker Compose (recommended)
- Python 3.10+ (if running without Docker)
- Git

### Option 1: Using Docker Compose (Recommended)

1. **Clone the repository** (if not already done):
   ```bash
   cd smartplugai/backend
   ```

2. **Create environment file**:
   ```bash
   cp .env.example .env
   ```

   Edit `.env` with your configuration:
   ```env
   # Application
   APP_NAME=SmartPlugAI
   ENVIRONMENT=development
   DEBUG=true
   SECRET_KEY=your-secret-key-change-in-production
   
   # API
   API_HOST=0.0.0.0
   API_PORT=8000
   
   # MQTT Broker (TLS enabled for production)
   MQTT_BROKER_HOST=mosquitto
   MQTT_BROKER_PORT=8883
   MQTT_USERNAME=YOUR_MQTT_USERNAME
   MQTT_PASSWORD=YOUR_MQTT_PASSWORD
   MQTT_TLS_ENABLED=true
   MQTT_CA_CERT=/certs/ca.crt
   MQTT_CLIENT_CERT=/certs/client.crt
   MQTT_CLIENT_KEY=/certs/client.key
   
   # InfluxDB (with field-level encryption)
   INFLUXDB_URL=http://influxdb:8086
   INFLUXDB_TOKEN=YOUR_INFLUXDB_TOKEN
   INFLUXDB_ORG=smartplugai
   INFLUXDB_BUCKET=telemetry
   INFLUXDB_ENCRYPTION_KEY=YOUR_FIELD_ENCRYPTION_KEY
   
   # PostgreSQL / Firestore
   POSTGRES_HOST=postgres
   POSTGRES_PORT=5432
   POSTGRES_DB=smartplugai
   POSTGRES_USER=smartplug
   POSTGRES_PASSWORD=YOUR_POSTGRES_PASSWORD
   POSTGRES_SSL_MODE=require
   
   # Firestore (alternative)
   FIRESTORE_PROJECT_ID=your-project-id
   FIRESTORE_CREDENTIALS=/path/to/service-account.json
   FIRESTORE_ENCRYPTION_KEY=YOUR_FIRESTORE_FIELD_ENCRYPTION_KEY
   
   # Redis
   REDIS_HOST=redis
   REDIS_PORT=6379
   REDIS_PASSWORD=YOUR_REDIS_PASSWORD
   REDIS_TLS_ENABLED=false
   
   # Auth0 / Firebase Auth
   AUTH_PROVIDER=auth0  # or firebase
   AUTH0_DOMAIN=your-tenant.auth0.com
   AUTH0_CLIENT_ID=YOUR_CLIENT_ID
   AUTH0_CLIENT_SECRET=YOUR_CLIENT_SECRET
   AUTH0_AUDIENCE=https://api.smartplugai.com
   
   # Firebase Auth (alternative)
   FIREBASE_PROJECT_ID=your-project-id
   FIREBASE_API_KEY=YOUR_API_KEY
   
   # JWT Authentication
   JWT_SECRET_KEY=your-jwt-secret-key
   JWT_ALGORITHM=HS256
   ACCESS_TOKEN_EXPIRE_MINUTES=30
   
   # ATECC608A Device Provisioning
   DEVICE_PROVISIONING_ENABLED=true
   DEVICE_CA_CERT=/certs/device-ca.crt
   DEVICE_CA_KEY=/certs/device-ca.key
   
   # Rate Limiting
   RATE_LIMIT_ENABLED=true
   RATE_LIMIT_PER_MINUTE=60
   
   # Audit Logging
   AUDIT_LOG_ENABLED=true
   AUDIT_LOG_PATH=/var/log/smartplugai/audit.log
   
   # AWS S3 (for ML data export - optional)
   AWS_ACCESS_KEY_ID=your-access-key
   AWS_SECRET_ACCESS_KEY=your-secret-key
   AWS_S3_BUCKET=smartplugai-ml-data
   AWS_REGION=us-east-1
   
   # Security & Compliance
   SABS_COMPLIANCE_MODE=true
   ICASA_COMPLIANCE_MODE=true
   SOC2_AUDIT_MODE=true
   ```

3. **Start all services**:
   ```bash
   docker-compose up -d
   ```

   This will start:
   - FastAPI application (port 8000)
   - Mosquitto MQTT broker (port 8883 TLS, WebSocket 9001)
   - InfluxDB (port 8086)
   - PostgreSQL (port 5432)
   - Redis (port 6379)
   
   **Note**: For TLS support, ensure certificate files are mounted:
   ```yaml
   # docker-compose.yml excerpt
   mosquitto:
     volumes:
       - ./certs/ca.crt:/mosquitto/certs/ca.crt
       - ./certs/server.crt:/mosquitto/certs/server.crt
       - ./certs/server.key:/mosquitto/certs/server.key
   ```

4. **Verify services are running**:
   ```bash
   docker-compose ps
   ```

5. **View logs**:
   ```bash
   docker-compose logs -f api
   ```

6. **Access API documentation**:
   - OpenAPI/Swagger: `http://localhost:8000/docs`
   - ReDoc: `http://localhost:8000/redoc`

7. **Run database migrations**:
   ```bash
   docker-compose exec api alembic upgrade head
   ```

### Option 2: Running Locally (Development)

1. **Install Python dependencies**:
   ```bash
   python -m venv venv
   source venv/bin/activate  # On Windows: venv\Scripts\activate
   pip install -r requirements.txt
   ```

2. **Start external services** (MQTT, InfluxDB, PostgreSQL, Redis):
   ```bash
   docker-compose up -d mosquitto influxdb postgres redis
   ```

3. **Run database migrations**:
   ```bash
   alembic upgrade head
   ```

4. **Start the API server**:
   ```bash
   uvicorn app.main:app --reload --host 0.0.0.0 --port 8000
   ```

5. **Start MQTT subscriber** (in separate terminal):
   ```bash
   python -m app.mqtt.subscriber
   ```

## 📁 Project Structure

```
backend/
├── app/
│   ├── __init__.py
│   ├── main.py                 # FastAPI application entry point
│   ├── config.py               # Configuration and environment variables
│   ├── dependencies.py         # Dependency injection
│   ├── api/                    # API endpoints
│   │   ├── __init__.py
│   │   ├── v1/
│   │   │   ├── __init__.py
│   │   │   ├── auth.py         # Authentication endpoints
│   │   │   ├── devices.py      # Device management
│   │   │   ├── telemetry.py    # Telemetry query endpoints
│   │   │   ├── control.py      # Device control endpoints
│   │   │   └── users.py        # User management
│   ├── models/                 # Database models (SQLAlchemy)
│   │   ├── __init__.py
│   │   ├── user.py
│   │   ├── device.py
│   │   └── telemetry.py
│   ├── schemas/                # Pydantic schemas (request/response)
│   │   ├── __init__.py
│   │   ├── auth.py
│   │   ├── device.py
│   │   └── telemetry.py
│   ├── services/               # Business logic
│   │   ├── __init__.py
│   │   ├── auth_service.py
│   │   ├── device_service.py
│   │   ├── telemetry_service.py
│   │   └── mqtt_service.py
│   ├── mqtt/                   # MQTT integration
│   │   ├── __init__.py
│   │   ├── client.py           # MQTT client setup
│   │   ├── subscriber.py       # Subscribe to device telemetry
│   │   └── publisher.py        # Publish control commands
│   ├── db/                     # Database utilities
│   │   ├── __init__.py
│   │   ├── session.py          # PostgreSQL session
│   │   └── influx.py           # InfluxDB client
│   ├── websocket/              # WebSocket handlers
│   │   ├── __init__.py
│   │   └── manager.py
│   ├── ml/                     # ML pipeline hooks
│   │   ├── __init__.py
│   │   ├── exporter.py         # Export data for ML
│   │   └── predictor.py        # Load and use ML models
│   └── utils/                  # Utility functions
│       ├── __init__.py
│       ├── security.py         # Password hashing, JWT
│       └── validators.py
├── alembic/                    # Database migrations
│   ├── versions/
│   └── env.py
├── tests/                      # Unit and integration tests
│   ├── __init__.py
│   ├── test_api/
│   ├── test_services/
│   └── conftest.py
├── docker-compose.yml          # Docker services configuration
├── Dockerfile                  # API container image
├── requirements.txt            # Python dependencies
├── alembic.ini                 # Alembic configuration
├── .env.example                # Environment variables template
└── README.md                   # This file
```

## 🌐 API Overview

### Authentication Endpoints

#### POST `/auth/register`

Register a new user account.

**Request**:

```json
{
  "email": "user@example.com",
  "password": "secure_password",
  "full_name": "John Doe"
}
```

**Response** (201 Created):

```json
{
  "id": "uuid-string",
  "email": "user@example.com",
  "full_name": "John Doe",
  "created_at": "2024-01-15T10:30:00Z"
}
```

#### POST `/auth/login`

Authenticate user and receive access token.

**Request**:

```json
{
  "email": "user@example.com",
  "password": "secure_password"
}
```

**Response** (200 OK):

```json
{
  "access_token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...",
  "token_type": "bearer",
  "expires_in": 1800
}
```

### Device Management Endpoints

#### GET `/devices`

List all devices for authenticated user.

**Headers**:

```
Authorization: Bearer <access_token>
```

**Response** (200 OK):

```json
{
  "devices": [
    {
      "id": "smartplug_001",
      "name": "Living Room Lamp",
      "status": "online",
      "last_seen": "2024-01-15T10:35:00Z",
      "firmware_version": "1.0.0",
      "created_at": "2024-01-10T08:00:00Z"
    }
  ],
  "total": 1
}
```

#### POST `/devices`

Register a new device (used during pairing).

**Request**:

```json
{
  "device_id": "smartplug_002",
  "name": "Kitchen Coffee Maker",
  "pairing_code": "ABC123"
}
```

**Response** (201 Created):

```json
{
  "id": "smartplug_002",
  "name": "Kitchen Coffee Maker",
  "status": "online",
  "created_at": "2024-01-15T10:40:00Z"
}
```

#### GET `/devices/{device_id}`

Get detailed information about a specific device.

**Response** (200 OK):

```json
{
  "id": "smartplug_001",
  "name": "Living Room Lamp",
  "status": "online",
  "relay_state": true,
  "current_power": 60.5,
  "firmware_version": "1.0.0",
  "last_seen": "2024-01-15T10:35:00Z",
  "total_energy_kwh": 45.2,
  "created_at": "2024-01-10T08:00:00Z"
}
```

### Device Control Endpoints

#### POST `/devices/{device_id}/command`

Send control command to device.

**Request**:

```json
{
  "command": "relay_on"
}
```

**Supported Commands**:

- `relay_on`: Turn device on
- `relay_off`: Turn device off
- `relay_toggle`: Toggle current state

**Response** (200 OK):

```json
{
  "device_id": "smartplug_001",
  "command": "relay_on",
  "status": "sent",
  "timestamp": "2024-01-15T10:45:00Z"
}
```

### Telemetry Endpoints

#### GET `/devices/{device_id}/telemetry`

Query historical telemetry data.

**Query Parameters**:

- `start`: Start timestamp (ISO 8601 format)
- `end`: End timestamp (ISO 8601 format)
- `interval`: Aggregation interval (e.g., "1m", "5m", "1h")
- `fields`: Comma-separated fields (voltage, current, power)

**Example**:

```
GET /devices/smartplug_001/telemetry?start=2024-01-15T00:00:00Z&end=2024-01-15T23:59:59Z&interval=1h&fields=power
```

**Response** (200 OK):

```json
{
  "device_id": "smartplug_001",
  "start": "2024-01-15T00:00:00Z",
  "end": "2024-01-15T23:59:59Z",
  "interval": "1h",
  "data": [
    {
      "timestamp": "2024-01-15T00:00:00Z",
      "power_avg": 45.2,
      "power_min": 0.0,
      "power_max": 120.5
    },
    {
      "timestamp": "2024-01-15T01:00:00Z",
      "power_avg": 52.8,
      "power_min": 0.0,
      "power_max": 115.0
    }
  ],
  "total_points": 24
}
```

#### GET `/devices/{device_id}/telemetry/latest`

Get most recent telemetry reading.

**Response** (200 OK):

```json
{
  "device_id": "smartplug_001",
  "timestamp": "2024-01-15T10:45:30Z",
  "voltage": 120.2,
  "current": 0.503,
  "power": 60.5,
  "relay_state": true
}
```

## 📡 MQTT Integration (TLS 1.3 with Client Certificates)

### Backend as MQTT Subscriber

The backend subscribes to device telemetry topics over TLS 1.3 and stores encrypted data in InfluxDB.

**Subscribed Topics**:

- `smartplug/+/telemetry` (all device telemetry - encrypted)
- `smartplug/+/status` (all device status updates)

**Handler Flow**:

1. MQTT message received on `smartplug/{device_id}/telemetry` over TLS 1.3
2. Verify client certificate and device identity
3. Parse JSON payload and decrypt encrypted fields
4. Validate data against schema and verify signature
5. Write to InfluxDB with field-level encryption for sensitive data
6. Update device last_seen in PostgreSQL/Firestore
7. Broadcast to WebSocket clients over TLS (real-time updates)
8. Trigger ML pipeline if anomaly detected
9. Log to audit trail

**Example MQTT Subscriber with TLS** (see `app/mqtt/subscriber.py`):

```python
import paho.mqtt.client as mqtt
import json
import ssl
from app.db.influx import write_telemetry_encrypted
from app.websocket.manager import broadcast_telemetry
from app.services.crypto import verify_signature, decrypt_telemetry

def on_message(client, userdata, msg):
    topic = msg.topic
    payload = json.loads(msg.payload.decode())
    
    # Extract device_id from topic: smartplug/{device_id}/telemetry
    device_id = topic.split('/')[1]
    
    # Verify signature (devices sign telemetry data)
    if not verify_signature(device_id, payload):
        logger.error(f"Invalid signature from device {device_id}")
        return
    
    # Decrypt sensitive fields
    decrypted_payload = decrypt_telemetry(payload)
    
    # Store in InfluxDB with field-level encryption
    write_telemetry_encrypted(device_id, decrypted_payload)
    
    # Broadcast to WebSocket clients (over TLS)
    broadcast_telemetry(device_id, decrypted_payload)

def start_mqtt_subscriber():
    client = mqtt.Client()
    
    # Configure TLS 1.3
    client.tls_set(
        ca_certs=MQTT_CA_CERT,
        certfile=MQTT_CLIENT_CERT,
        keyfile=MQTT_CLIENT_KEY,
        tls_version=ssl.PROTOCOL_TLS,
        cert_reqs=ssl.CERT_REQUIRED
    )
    client.tls_insecure_set(False)
    
    client.username_pw_set(MQTT_USERNAME, MQTT_PASSWORD)
    client.on_message = on_message
    client.connect(MQTT_BROKER_HOST, 8883)  # Port 8883 for TLS
    client.subscribe("smartplug/+/telemetry")
    client.subscribe("smartplug/+/status")
    client.loop_forever()
```

### Backend as MQTT Publisher

The backend publishes signed control commands to devices via MQTT over TLS 1.3.

**Published Topics**:

- `smartplug/{device_id}/control` (signed device commands)
- `smartplug/{device_id}/config` (configuration updates)

**Control Flow**:

1. API receives command via POST `/devices/{device_id}/command`
2. Validate command and user permissions (RBAC)
3. Sign command with server's private key (ECDSA)
4. Publish MQTT message to `smartplug/{device_id}/control` over TLS
5. Device receives command, verifies signature using server's public key
6. Device executes command if signature is valid
7. Device publishes status update
8. Backend receives status update and returns to API caller
9. Log to audit trail

**Example MQTT Publisher with Signed Commands** (see `app/mqtt/publisher.py`):

```python
import paho.mqtt.client as mqtt
import json
import ssl
from app.services.crypto import sign_command

def send_device_command(device_id: str, command: str):
    client = mqtt.Client()
    
    # Configure TLS 1.3
    client.tls_set(
        ca_certs=MQTT_CA_CERT,
        certfile=MQTT_CLIENT_CERT,
        keyfile=MQTT_CLIENT_KEY,
        tls_version=ssl.PROTOCOL_TLS
    )
    
    client.username_pw_set(MQTT_USERNAME, MQTT_PASSWORD)
    client.connect(MQTT_BROKER_HOST, 8883)  # Port 8883 for TLS
    
    # Sign the command
    signature = sign_command(command)
    
    topic = f"smartplug/{device_id}/control"
    payload = json.dumps({
        "command": command,
        "signature": signature,
        "timestamp": int(time.time())
    })
    
    client.publish(topic, payload, qos=1)
    client.disconnect()
```

## 💾 Data Storage & Retention

### InfluxDB (Time-Series Data)

**Bucket**: `telemetry`

**Measurement**: `power_metrics`

**Tags**:

- `device_id`: Unique device identifier
- `location`: Device location (optional)
- `device_type`: Type of device (plug, switch, etc.)

**Fields**:

- `voltage` (float): Voltage in Volts
- `current` (float): Current in Amps
- `power` (float): Power in Watts
- `relay_state` (boolean): Relay on/off state

**Retention Policies**:

- **Raw data**: 7 days (1-second resolution)
- **1-minute aggregates**: 30 days
- **1-hour aggregates**: 1 year
- **1-day aggregates**: 5 years

**Example InfluxDB Write** (using Flux):

```python
from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS

def write_telemetry(device_id: str, data: dict):
    with InfluxDBClient(url=INFLUXDB_URL, token=INFLUXDB_TOKEN, org=INFLUXDB_ORG) as client:
        write_api = client.write_api(write_options=SYNCHRONOUS)
        
        point = Point("power_metrics") \
            .tag("device_id", device_id) \
            .field("voltage", data["voltage"]) \
            .field("current", data["current"]) \
            .field("power", data["power"]) \
            .field("relay_state", data["relay_state"]) \
            .time(data["timestamp"])
        
        write_api.write(bucket=INFLUXDB_BUCKET, record=point)
```

### PostgreSQL (Metadata)

**Tables**:

- `users`: User accounts and profiles
- `devices`: Device metadata and configuration
- `device_users`: Many-to-many relationship (shared devices)
- `schedules`: Device automation schedules
- `alerts`: Alert configurations and history
- `audit_logs`: API access and changes

**Example Schema**:

```sql
CREATE TABLE devices (
    id VARCHAR(255) PRIMARY KEY,
    user_id UUID NOT NULL REFERENCES users(id),
    name VARCHAR(255) NOT NULL,
    firmware_version VARCHAR(50),
    status VARCHAR(50) DEFAULT 'offline',
    last_seen TIMESTAMP,
    created_at TIMESTAMP DEFAULT NOW(),
    updated_at TIMESTAMP DEFAULT NOW()
);

CREATE INDEX idx_devices_user_id ON devices(user_id);
CREATE INDEX idx_devices_status ON devices(status);
```

### Redis (Cache & Sessions)

**Use Cases**:

- User session storage (JWT token blacklist)
- Real-time device state cache
- Rate limiting counters
- WebSocket connection tracking
- Task queue for async processing

**Example Redis Usage**:

```python
import redis

# Cache latest telemetry for quick access
def cache_latest_telemetry(device_id: str, data: dict):
    r = redis.Redis(host=REDIS_HOST, port=REDIS_PORT)
    key = f"device:{device_id}:latest"
    r.setex(key, 300, json.dumps(data))  # Expire after 5 minutes
```

## 🤖 ML Pipeline Hooks

### Data Export for Training

**Batch Export to S3**:

```python
from app.ml.exporter import export_telemetry_to_s3

# Export last 30 days of data for ML training
export_telemetry_to_s3(
    start_date="2024-01-01",
    end_date="2024-01-31",
    format="parquet",
    s3_path="s3://smartplugai-ml-data/training/2024-01/"
)
```

**Export Format** (Parquet):

```
device_id | timestamp           | voltage | current | power | relay_state | day_of_week | hour_of_day
----------|---------------------|---------|---------|-------|-------------|-------------|------------
plug_001  | 2024-01-15 10:30:00 | 120.2   | 0.503   | 60.5  | true        | 1           | 10
plug_001  | 2024-01-15 10:30:01 | 120.1   | 0.501   | 60.2  | true        | 1           | 10
```

### Real-Time Inference

**Anomaly Detection**:

```python
from app.ml.predictor import detect_anomaly

def process_telemetry(device_id: str, data: dict):
    # Store data
    write_telemetry(device_id, data)
    
    # Check for anomalies
    is_anomaly, confidence = detect_anomaly(device_id, data)
    
    if is_anomaly and confidence > 0.8:
        send_alert(device_id, "Unusual power consumption detected")
```

**Pattern Recognition**:

```python
from app.ml.predictor import predict_usage_pattern

def get_usage_prediction(device_id: str):
    # Predict usage for next 24 hours
    predictions = predict_usage_pattern(device_id, hours=24)
    return predictions
```

## 🚀 Deployment

### Production Deployment Checklist

- [ ] Set strong passwords and secret keys in environment variables
- [ ] Enable TLS/SSL for MQTT (port 8883)
- [ ] Enable HTTPS for API (use reverse proxy like Nginx)
- [ ] Configure firewall rules (allow only necessary ports)
- [ ] Set up database backups (PostgreSQL and InfluxDB)
- [ ] Configure log aggregation (ELK stack or CloudWatch)
- [ ] Set up monitoring and alerting (Prometheus + Grafana)
- [ ] Implement rate limiting on API endpoints
- [ ] Enable CORS with appropriate origins
- [ ] Configure auto-scaling for API containers
- [ ] Set up CI/CD pipeline for automated deployments
- [ ] Perform security audit and penetration testing

### Recommended Hosting

**Option 1: AWS**

- **API**: ECS Fargate or Lambda
- **MQTT**: EC2 with Mosquitto or AWS IoT Core
- **Databases**: RDS (PostgreSQL), InfluxDB Cloud, ElastiCache (Redis)
- **Storage**: S3 for ML data
- **Load Balancer**: Application Load Balancer

**Option 2: Google Cloud**

- **API**: Cloud Run or GKE
- **MQTT**: Compute Engine or IoT Core
- **Databases**: Cloud SQL, InfluxDB Cloud, Memorystore (Redis)
- **Storage**: Cloud Storage

**Option 3: Self-Hosted (VPS)**

- **Provider**: DigitalOcean, Linode, Vultr
- **Setup**: Docker Swarm or single-server Docker Compose
- **Reverse Proxy**: Nginx with Let's Encrypt SSL
- **Monitoring**: Self-hosted Grafana + Prometheus

### Environment Variables for Production

```env
# Application
ENVIRONMENT=production
DEBUG=false
SECRET_KEY=<generate-with-openssl-rand-hex-32>

# API
API_HOST=0.0.0.0
API_PORT=8000
ALLOWED_ORIGINS=https://app.smartplugai.com,https://dashboard.smartplugai.com

# MQTT Broker (use TLS in production)
MQTT_BROKER_HOST=mqtt.smartplugai.com
MQTT_BROKER_PORT=8883
MQTT_TLS_ENABLED=true
MQTT_CA_CERT=/path/to/ca.crt

# JWT
JWT_SECRET_KEY=<generate-strong-secret>
ACCESS_TOKEN_EXPIRE_MINUTES=30
REFRESH_TOKEN_EXPIRE_DAYS=7

# Monitoring
SENTRY_DSN=https://your-sentry-dsn
LOG_LEVEL=INFO
```

### Secrets Management

Use secure secret management:

- **AWS**: AWS Secrets Manager or Parameter Store
- **GCP**: Secret Manager
- **Kubernetes**: Kubernetes Secrets
- **Docker**: Docker Secrets (Swarm mode)
- **HashiCorp Vault**: For advanced secret rotation

## 📊 Monitoring

### Health Check Endpoint

```python
@app.get("/health")
async def health_check():
    return {
        "status": "healthy",
        "timestamp": datetime.utcnow().isoformat(),
        "services": {
            "database": check_postgres_connection(),
            "influxdb": check_influx_connection(),
            "redis": check_redis_connection(),
            "mqtt": check_mqtt_connection()
        }
    }
```

### Metrics to Monitor

- **API Metrics**: Request rate, latency, error rate
- **MQTT Metrics**: Messages per second, connection count
- **Database Metrics**: Query latency, connection pool usage
- **System Metrics**: CPU, memory, disk usage
- **Business Metrics**: Active devices, users, telemetry points per day

### Logging

Use structured logging (JSON format):

```python
import logging
import json

logger = logging.getLogger(__name__)

def log_telemetry_received(device_id: str, data: dict):
    logger.info(json.dumps({
        "event": "telemetry_received",
        "device_id": device_id,
        "power": data.get("power"),
        "timestamp": data.get("timestamp")
    }))
```

## 🧪 Testing

### Run Unit Tests

```bash
pytest tests/ -v
```

### Run Integration Tests

```bash
pytest tests/ -v -m integration
```

### Test Coverage

```bash
pytest --cov=app --cov-report=html tests/
open htmlcov/index.html
```

### Load Testing

Use tools like Apache JMeter, Locust, or k6:

```bash
# Example with k6
k6 run --vus 100 --duration 60s load_test.js
```

## 🤝 Contributing

Backend contributions are welcome! Please:

- Write unit tests for new features
- Follow Python PEP 8 style guide (use `black` formatter)
- Update API documentation
- Add database migrations for schema changes
- Update this README with new features

See [CONTRIBUTING.md](../CONTRIBUTING.md) for general guidelines.

---

**Ready to build a scalable IoT backend!** ☁️⚡

For comprehensive security architecture, ATECC608A device provisioning workflows, TLS certificate management, SABS/ICASA compliance guidelines, and SOC2 readiness roadmap, see [docs/SECURITY.md](../docs/SECURITY.md) and [docs/ROADMAP.md](../docs/ROADMAP.md) for Phase 1 (12 weeks) timeline with security milestones.
