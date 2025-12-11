# Smart Plug AI API Documentation

Comprehensive API reference for REST endpoints, WebSocket connections, and MQTT topics.

## 📚 Table of Contents

- [REST API Reference](#rest-api-reference)
- [WebSocket API](#websocket-api)
- [MQTT Topics Reference](#mqtt-topics-reference)
- [Rate Limits](#rate-limits)
- [Error Codes](#error-codes)

## 🌐 REST API Reference

**Base URL**: `https://api.smartplugai.com/v1`

**Development**: `http://localhost:8000/v1`

### Authentication

All authenticated endpoints require a Bearer token in the Authorization header:

```
Authorization: Bearer <access_token>
```

#### POST `/auth/register`

Register a new user account.

**Request**:

```json
{
  "email": "user@example.com",
  "password": "SecurePassword123!",
  "full_name": "John Doe"
}
```

**Validation**:

- Email: Must be valid email format
- Password: Minimum 8 characters, must contain uppercase, lowercase, number, and special character
- Full name: 2-100 characters

**Response** (201 Created):

```json
{
  "id": "550e8400-e29b-41d4-a716-446655440000",
  "email": "user@example.com",
  "full_name": "John Doe",
  "created_at": "2024-01-15T10:30:00Z",
  "updated_at": "2024-01-15T10:30:00Z"
}
```

**Errors**:

- `400 Bad Request`: Invalid email or password format
- `409 Conflict`: Email already registered

#### POST `/auth/login`

Authenticate user and receive access token.

**Request**:

```json
{
  "email": "user@example.com",
  "password": "SecurePassword123!"
}
```

**Response** (200 OK):

```json
{
  "access_token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiI1NTBlODQwMC1lMjliLTQxZDQtYTcxNi00NDY2NTU0NDAwMDAiLCJleHAiOjE3MDUzMjE4MDB9.SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c",
  "refresh_token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...",
  "token_type": "bearer",
  "expires_in": 1800
}
```

**Errors**:

- `400 Bad Request`: Missing email or password
- `401 Unauthorized`: Invalid credentials
- `429 Too Many Requests`: Rate limit exceeded (see [Rate Limits](#rate-limits))

#### POST `/auth/refresh`

Refresh access token using refresh token.

**Request**:

```json
{
  "refresh_token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9..."
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

**Errors**:

- `401 Unauthorized`: Invalid or expired refresh token

#### POST `/auth/logout`

Invalidate current access token (add to blacklist).

**Headers**:

```
Authorization: Bearer <access_token>
```

**Response** (200 OK):

```json
{
  "message": "Successfully logged out"
}
```

### Device Management

#### GET `/devices`

List all devices for authenticated user.

**Headers**:

```
Authorization: Bearer <access_token>
```

**Query Parameters**:

- `page` (optional): Page number for pagination (default: 1)
- `per_page` (optional): Items per page (default: 20, max: 100)
- `status` (optional): Filter by status (`online`, `offline`, `all`)
- `sort` (optional): Sort by field (`name`, `created_at`, `last_seen`)
- `order` (optional): Sort order (`asc`, `desc`)

**Example**:

```
GET /devices?page=1&per_page=10&status=online&sort=last_seen&order=desc
```

**Response** (200 OK):

```json
{
  "devices": [
    {
      "id": "smartplug_001",
      "name": "Living Room Lamp",
      "status": "online",
      "relay_state": true,
      "current_power": 60.5,
      "firmware_version": "1.0.0",
      "last_seen": "2024-01-15T10:35:00Z",
      "created_at": "2024-01-10T08:00:00Z",
      "updated_at": "2024-01-15T10:35:00Z"
    },
    {
      "id": "smartplug_002",
      "name": "Kitchen Coffee Maker",
      "status": "online",
      "relay_state": false,
      "current_power": 0.0,
      "firmware_version": "1.0.0",
      "last_seen": "2024-01-15T10:34:58Z",
      "created_at": "2024-01-12T09:15:00Z",
      "updated_at": "2024-01-15T10:34:58Z"
    }
  ],
  "pagination": {
    "page": 1,
    "per_page": 10,
    "total": 2,
    "total_pages": 1
  }
}
```

#### POST `/devices`

Register a new device (pairing).

**Headers**:

```
Authorization: Bearer <access_token>
```

**Request**:

```json
{
  "device_id": "smartplug_003",
  "name": "Bedroom Heater",
  "pairing_code": "ABC123"
}
```

**Response** (201 Created):

```json
{
  "id": "smartplug_003",
  "name": "Bedroom Heater",
  "status": "online",
  "relay_state": false,
  "firmware_version": "1.0.0",
  "created_at": "2024-01-15T10:40:00Z",
  "updated_at": "2024-01-15T10:40:00Z"
}
```

**Errors**:

- `400 Bad Request`: Invalid pairing code or device_id
- `409 Conflict`: Device already registered

#### GET `/devices/{device_id}`

Get detailed information about a specific device.

**Headers**:

```
Authorization: Bearer <access_token>
```

**Response** (200 OK):

```json
{
  "id": "smartplug_001",
  "name": "Living Room Lamp",
  "status": "online",
  "relay_state": true,
  "current_power": 60.5,
  "current_voltage": 120.2,
  "current_current": 0.503,
  "firmware_version": "1.0.0",
  "hardware_version": "1.0",
  "last_seen": "2024-01-15T10:35:00Z",
  "total_energy_kwh": 45.234,
  "uptime_seconds": 86400,
  "wifi_rssi": -45,
  "location": "Living Room",
  "created_at": "2024-01-10T08:00:00Z",
  "updated_at": "2024-01-15T10:35:00Z"
}
```

**Errors**:

- `404 Not Found`: Device not found or user doesn't have access

#### PUT `/devices/{device_id}`

Update device information.

**Headers**:

```
Authorization: Bearer <access_token>
```

**Request**:

```json
{
  "name": "Living Room Floor Lamp",
  "location": "Living Room - South Corner"
}
```

**Response** (200 OK):

```json
{
  "id": "smartplug_001",
  "name": "Living Room Floor Lamp",
  "location": "Living Room - South Corner",
  "updated_at": "2024-01-15T10:45:00Z"
}
```

#### DELETE `/devices/{device_id}`

Remove device from user account (unpair).

**Headers**:

```
Authorization: Bearer <access_token>
```

**Response** (204 No Content)

**Errors**:

- `404 Not Found`: Device not found or user doesn't have access

### Device Control

#### POST `/devices/{device_id}/command`

Send control command to device.

**Headers**:

```
Authorization: Bearer <access_token>
```

**Request**:

```json
{
  "command": "relay_on"
}
```

**Supported Commands**:

- `relay_on`: Turn relay ON
- `relay_off`: Turn relay OFF
- `relay_toggle`: Toggle relay state
- `restart`: Restart device (reboot ESP32)
- `update_interval`: Change telemetry interval (include `interval_ms` field)

**Example with parameters**:

```json
{
  "command": "update_interval",
  "interval_ms": 5000
}
```

**Response** (200 OK):

```json
{
  "device_id": "smartplug_001",
  "command": "relay_on",
  "status": "sent",
  "timestamp": "2024-01-15T10:45:00Z"
}
```

**Errors**:

- `400 Bad Request`: Invalid command
- `404 Not Found`: Device not found
- `503 Service Unavailable`: Device offline or unreachable

### Telemetry

#### GET `/devices/{device_id}/telemetry`

Query historical telemetry data.

**Headers**:

```
Authorization: Bearer <access_token>
```

**Query Parameters**:

- `start` (required): Start timestamp (ISO 8601 format)
- `end` (required): End timestamp (ISO 8601 format)
- `interval` (optional): Aggregation interval (`1m`, `5m`, `15m`, `1h`, `1d`)
- `fields` (optional): Comma-separated fields (`voltage`, `current`, `power`, `relay_state`)

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
  "fields": ["power"],
  "data": [
    {
      "timestamp": "2024-01-15T00:00:00Z",
      "power_mean": 45.2,
      "power_min": 0.0,
      "power_max": 120.5,
      "power_sum": 162720.0
    },
    {
      "timestamp": "2024-01-15T01:00:00Z",
      "power_mean": 52.8,
      "power_min": 0.0,
      "power_max": 115.0,
      "power_sum": 190080.0
    }
  ],
  "total_points": 24
}
```

**Errors**:

- `400 Bad Request`: Invalid date range or interval
- `404 Not Found`: Device not found

#### GET `/devices/{device_id}/telemetry/latest`

Get most recent telemetry reading.

**Headers**:

```
Authorization: Bearer <access_token>
```

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

#### GET `/devices/{device_id}/energy`

Get energy consumption summary.

**Headers**:

```
Authorization: Bearer <access_token>
```

**Query Parameters**:

- `period` (optional): Time period (`today`, `week`, `month`, `year`, `all`)
- `start` (optional): Custom start date (ISO 8601)
- `end` (optional): Custom end date (ISO 8601)

**Example**:

```
GET /devices/smartplug_001/energy?period=month
```

**Response** (200 OK):

```json
{
  "device_id": "smartplug_001",
  "period": "month",
  "start": "2024-01-01T00:00:00Z",
  "end": "2024-01-31T23:59:59Z",
  "total_energy_kwh": 45.234,
  "estimated_cost_usd": 5.43,
  "average_power_w": 60.8,
  "peak_power_w": 125.0,
  "on_time_hours": 744.5
}
```

### User Management

#### GET `/users/me`

Get current user profile.

**Headers**:

```
Authorization: Bearer <access_token>
```

**Response** (200 OK):

```json
{
  "id": "550e8400-e29b-41d4-a716-446655440000",
  "email": "user@example.com",
  "full_name": "John Doe",
  "created_at": "2024-01-01T10:00:00Z",
  "updated_at": "2024-01-15T10:45:00Z"
}
```

#### PUT `/users/me`

Update current user profile.

**Headers**:

```
Authorization: Bearer <access_token>
```

**Request**:

```json
{
  "full_name": "John A. Doe",
  "email": "john.doe@example.com"
}
```

**Response** (200 OK):

```json
{
  "id": "550e8400-e29b-41d4-a716-446655440000",
  "email": "john.doe@example.com",
  "full_name": "John A. Doe",
  "updated_at": "2024-01-15T10:50:00Z"
}
```

#### PUT `/users/me/password`

Change user password.

**Headers**:

```
Authorization: Bearer <access_token>
```

**Request**:

```json
{
  "current_password": "OldPassword123!",
  "new_password": "NewSecurePassword456!"
}
```

**Response** (200 OK):

```json
{
  "message": "Password updated successfully"
}
```

**Errors**:

- `400 Bad Request`: Invalid password format
- `401 Unauthorized`: Current password incorrect

## 🔌 WebSocket API

**URL**: `wss://api.smartplugai.com/ws`

**Development**: `ws://localhost:8000/ws`

### Connection

Establish WebSocket connection with authentication:

```javascript
const ws = new WebSocket('wss://api.smartplugai.com/ws');

// Send authentication message after connection
ws.onopen = () => {
  ws.send(JSON.stringify({
    type: 'auth',
    token: '<access_token>'
  }));
};
```

### Subscribe to Device Updates

Subscribe to real-time updates for specific devices:

```javascript
ws.send(JSON.stringify({
  type: 'subscribe',
  devices: ['smartplug_001', 'smartplug_002']
}));
```

### Receive Telemetry Updates

Server broadcasts telemetry updates for subscribed devices:

```javascript
ws.onmessage = (event) => {
  const data = JSON.parse(event.data);
  
  if (data.type === 'telemetry') {
    console.log('Device:', data.device_id);
    console.log('Power:', data.power);
    // Handle update in UI
  }
};
```

**Telemetry Message Format**:

```json
{
  "type": "telemetry",
  "device_id": "smartplug_001",
  "timestamp": "2024-01-15T10:45:30Z",
  "voltage": 120.2,
  "current": 0.503,
  "power": 60.5,
  "relay_state": true
}
```

### Receive Status Updates

Device status changes (online/offline, relay state):

```json
{
  "type": "status",
  "device_id": "smartplug_001",
  "status": "online",
  "relay_state": true,
  "timestamp": "2024-01-15T10:45:30Z"
}
```

### Unsubscribe

```javascript
ws.send(JSON.stringify({
  type: 'unsubscribe',
  devices: ['smartplug_002']
}));
```

### Heartbeat

Client should send periodic ping to keep connection alive:

```javascript
setInterval(() => {
  ws.send(JSON.stringify({ type: 'ping' }));
}, 30000); // Every 30 seconds
```

Server responds with pong:

```json
{
  "type": "pong",
  "timestamp": "2024-01-15T10:45:30Z"
}
```

## 📡 MQTT Topics Reference

### Device → Cloud Topics

#### `smartplug/{device_id}/telemetry`

Device publishes power telemetry data.

**QoS**: 0 (at most once)

**Retained**: No

**Frequency**: 1 second (default, configurable)

**Payload Schema**:

```json
{
  "device_id": "smartplug_001",
  "timestamp": 1705321530000,
  "voltage": 120.2,
  "current": 0.503,
  "power": 60.5,
  "relay_state": true
}
```

**JSON Schema**:

```json
{
  "$schema": "http://json-schema.org/draft-07/schema#",
  "type": "object",
  "required": ["device_id", "timestamp", "voltage", "current", "power", "relay_state"],
  "properties": {
    "device_id": {
      "type": "string",
      "pattern": "^smartplug_[a-zA-Z0-9_-]+$",
      "description": "Unique device identifier"
    },
    "timestamp": {
      "type": "integer",
      "description": "Unix timestamp in milliseconds"
    },
    "voltage": {
      "type": "number",
      "minimum": 0,
      "maximum": 300
    },
    "current": {
      "type": "number",
      "minimum": 0,
      "maximum": 20
    },
    "power": {
      "type": "number",
      "minimum": 0,
      "maximum": 5000
    },
    "relay_state": {
      "type": "boolean"
    }
  }
}
```

#### `smartplug/{device_id}/status`

Device publishes status updates (online/offline).

**QoS**: 1 (at least once)

**Retained**: Yes (Last Will and Testament)

**Payload Schema**:

```json
{
  "device_id": "smartplug_001",
  "status": "online",
  "timestamp": 1705321530000,
  "firmware_version": "1.0.0",
  "ip_address": "192.168.1.100"
}
```

**Status Values**: `online`, `offline`

### Cloud → Device Topics

#### `smartplug/{device_id}/control`

Backend publishes control commands to device.

**QoS**: 1 (at least once)

**Retained**: No

**Payload Schema**:

```json
{
  "command": "relay_on",
  "timestamp": 1705321530000
}
```

**Supported Commands**:

- `relay_on`: Turn relay ON
- `relay_off`: Turn relay OFF
- `relay_toggle`: Toggle relay state
- `restart`: Restart device
- `update_interval`: Change telemetry interval

**Command with Parameters**:

```json
{
  "command": "update_interval",
  "interval_ms": 5000,
  "timestamp": 1705321530000
}
```

#### `smartplug/{device_id}/config`

Backend publishes configuration updates.

**QoS**: 1 (at least once)

**Retained**: Yes

**Payload Schema**:

```json
{
  "telemetry_interval": 1000,
  "calibration": {
    "voltage_offset": 0.0,
    "current_offset": 0.0,
    "power_factor": 1.0
  },
  "ota_url": "https://firmware.smartplugai.com/v1.1.0.bin",
  "timestamp": 1705321530000
}
```

#### `smartplug/{device_id}/ota`

Backend publishes OTA update commands.

**QoS**: 1 (at least once)

**Retained**: No

**Payload Schema**:

```json
{
  "firmware_url": "https://firmware.smartplugai.com/v1.1.0.bin",
  "version": "1.1.0",
  "checksum": "sha256:abcdef1234567890...",
  "timestamp": 1705321530000
}
```

### Wildcard Subscriptions

Backend subscribes to all devices using wildcards:

- `smartplug/+/telemetry` - All device telemetry
- `smartplug/+/status` - All device status updates

## ⏱️ Rate Limits

Rate limits are enforced per user account:

| Endpoint | Limit | Window |
|----------|-------|--------|
| `POST /auth/login` | 5 requests | 15 minutes |
| `POST /auth/register` | 3 requests | 1 hour |
| `GET /devices` | 100 requests | 1 minute |
| `GET /devices/{id}/telemetry` | 60 requests | 1 minute |
| `POST /devices/{id}/command` | 30 requests | 1 minute |
| `PUT /devices/{id}` | 10 requests | 1 minute |
| WebSocket connections | 10 connections | Per user |
| MQTT connections | 50 connections | Per user |

**Rate Limit Headers**:

```
X-RateLimit-Limit: 100
X-RateLimit-Remaining: 95
X-RateLimit-Reset: 1705321800
```

**Rate Limit Exceeded Response** (429 Too Many Requests):

```json
{
  "error": "rate_limit_exceeded",
  "message": "Too many requests. Please try again in 45 seconds.",
  "retry_after": 45
}
```

## ❌ Error Codes

All error responses follow this format:

```json
{
  "error": "error_code",
  "message": "Human-readable error message",
  "details": {
    "field": "Additional error details"
  }
}
```

### HTTP Status Codes

| Code | Name | Description |
|------|------|-------------|
| 200 | OK | Successful request |
| 201 | Created | Resource created successfully |
| 204 | No Content | Successful request with no response body |
| 400 | Bad Request | Invalid request parameters or body |
| 401 | Unauthorized | Missing or invalid authentication |
| 403 | Forbidden | Authenticated but not authorized |
| 404 | Not Found | Resource not found |
| 409 | Conflict | Resource already exists |
| 422 | Unprocessable Entity | Validation error |
| 429 | Too Many Requests | Rate limit exceeded |
| 500 | Internal Server Error | Server error |
| 503 | Service Unavailable | Service temporarily unavailable |

### Error Codes

| Code | HTTP Status | Description |
|------|-------------|-------------|
| `invalid_credentials` | 401 | Invalid email or password |
| `token_expired` | 401 | Access token has expired |
| `invalid_token` | 401 | Malformed or invalid token |
| `email_already_exists` | 409 | Email already registered |
| `device_not_found` | 404 | Device not found or no access |
| `invalid_pairing_code` | 400 | Incorrect pairing code |
| `device_offline` | 503 | Device is offline |
| `invalid_command` | 400 | Unsupported command |
| `validation_error` | 422 | Request validation failed |
| `rate_limit_exceeded` | 429 | Too many requests |
| `internal_error` | 500 | Unexpected server error |

### Example Error Responses

**Validation Error**:

```json
{
  "error": "validation_error",
  "message": "Request validation failed",
  "details": {
    "email": "Invalid email format",
    "password": "Password must be at least 8 characters"
  }
}
```

**Device Offline**:

```json
{
  "error": "device_offline",
  "message": "Device smartplug_001 is currently offline",
  "details": {
    "device_id": "smartplug_001",
    "last_seen": "2024-01-15T09:30:00Z"
  }
}
```

## 📖 Additional Resources

- [OpenAPI Specification](https://api.smartplugai.com/docs) - Interactive API documentation
- [Postman Collection](https://www.postman.com/smartplugai/workspace) - Pre-built API requests
- [Backend README](../backend/README.md) - Backend setup and development
- [Architecture Documentation](ARCHITECTURE.md) - System architecture overview

---

**API Version**: v1.0.0 | **Last Updated**: January 2025
