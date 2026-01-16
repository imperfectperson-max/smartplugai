# Class & ER Diagrams

This folder contains class diagrams and entity-relationship diagrams for the Smart Plug AI system.

## Overview

These diagrams document:
- Database schema and relationships (ER Diagrams)
- Object-oriented class structures (Class Diagrams)
- Domain models and business logic
- Service layer architecture
- Security components

## Recommended Tools

- **Primary**: Visual Paradigm Community Edition - Superior database modeling tools
- **Backup**: Diagrams.net - Alternative for quick edits

## Diagrams in this Folder

### Main Diagrams
- **3.1_Class_Diagram_Backend.vpp** - Backend service layer classes
- **3.2_Class_Diagram_Firmware.vpp** - ESP32 firmware classes
- **3.3_ER_Diagram_Database.vpp** - Complete database schema
- **3.4_SQL_DDL_Generated.sql** - SQL Data Definition Language file (generated from ER diagram)

## Entity-Relationship Diagram

The following Mermaid ER diagram shows the complete database schema for Smart Plug AI:

<!-- Note: This diagram was copied from source_d24e744d.txt - includes all security fields and relationships -->

```mermaid
erDiagram
    USERS {
        uuid user_id PK
        string email UK
        string password_hash
        string full_name
        string role
        string two_fa_secret
        boolean two_fa_enabled
        timestamp created_at
        timestamp updated_at
        timestamp last_login
        string phone_number
        boolean is_active
    }


    DEVICES {
        string device_id PK
        uuid owner_id FK
        string name
        string location
        string status
        boolean relay_state
        float current_power
        string firmware_version
        timestamp last_seen
        timestamp created_at
        timestamp updated_at
        string certificate_id
        string hardware_version
        int tamper_count
        boolean attestation_valid
        float total_energy_kwh
        int uptime_seconds
        int wifi_rssi
        string mac_address
    }


    POWER_READINGS {
        uuid reading_id PK
        string device_id FK
        timestamp measured_at
        float voltage
        float current
        float power
        float energy_wh
        float frequency
        float power_factor
        float temperature
        string signature
        boolean is_encrypted
    }


    SECURITY_EVENTS {
        uuid event_id PK
        string device_id FK
        string event_type
        string severity
        json event_data
        timestamp occurred_at
        string ip_address
        uuid user_id FK
        boolean resolved
        timestamp resolved_at
        string resolution_notes
    }


    COMMANDS {
        uuid command_id PK
        string device_id FK
        uuid user_id FK
        string command_type
        json command_data
        string signature
        string status
        timestamp created_at
        timestamp sent_at
        timestamp executed_at
        string result_message
        string nonce
    }


    CERTIFICATES {
        uuid certificate_id PK
        string device_id FK
        string cert_type
        string cert_data
        string public_key
        string private_key_slot
        date issued_date
        date expiry_date
        string status
        date revoked_date
        string revocation_reason
        string issuer_cert_id
        string serial_number
        string thumbprint
    }


    ALERTS {
        uuid alert_id PK
        string device_id FK
        string alert_type
        string severity
        string message
        json alert_data
        timestamp triggered_at
        timestamp acknowledged_at
        uuid acknowledged_by FK
        boolean resolved
        timestamp resolved_at
        string resolution_notes
        boolean notification_sent
    }


    AUDIT_LOGS {
        uuid log_id PK
        uuid user_id FK
        string action_type
        string resource_type
        string resource_id
        json before_state
        json after_state
        string ip_address
        string user_agent
        timestamp created_at
        string session_id
        boolean success
        string error_message
    }


    DEVICE_SHARING {
        uuid share_id PK
        string device_id FK
        uuid user_id FK
        uuid shared_by FK
        string access_level
        timestamp shared_at
        timestamp expires_at
        boolean is_active
        string sharing_token
    }


    SCHEDULES {
        uuid schedule_id PK
        string device_id FK
        uuid user_id FK
        string schedule_type
        json schedule_data
        string command
        json command_data
        boolean enabled
        timestamp last_executed
        timestamp next_execution
        timestamp created_at
        timestamp updated_at
    }


    API_KEYS {
        uuid key_id PK
        uuid user_id FK
        string api_key
        string name
        string key_type
        json allowed_ips
        json rate_limits
        timestamp created_at
        timestamp expires_at
        timestamp last_used
        int usage_count
        boolean is_active
        string revoked_reason
    }


    %% Relationships
    USERS ||--o{ DEVICES : "owns"
    USERS ||--o{ COMMANDS : "issues"
    USERS ||--o{ SECURITY_EVENTS : "may trigger"
    USERS ||--o{ AUDIT_LOGS : "performs"
    USERS ||--o{ ALERTS : "acknowledges"
    USERS ||--o{ API_KEYS : "owns"
    USERS ||--o{ SCHEDULES : "creates"
    USERS ||--o{ DEVICE_SHARING : "shares"


    DEVICES ||--o{ POWER_READINGS : "generates"
    DEVICES ||--o{ COMMANDS : "receives"
    DEVICES ||--o{ SECURITY_EVENTS : "experiences"
    DEVICES ||--o{ ALERTS : "triggers"
    DEVICES ||--o{ CERTIFICATES : "has"
    DEVICES ||--o{ DEVICE_SHARING : "shared via"
    DEVICES ||--o{ SCHEDULES : "scheduled for"


    CERTIFICATES ||--o{ CERTIFICATES : "issued by"


    %% Many-to-Many through DEVICE_SHARING
    USERS }o--o{ DEVICES : "shares access to"
```

## SQL DDL File Reference

The complete SQL Data Definition Language (DDL) file is available at:
`3.4_SQL_DDL_Generated.sql`

This file includes:
- Table creation statements with all fields
- Primary key and foreign key constraints
- Indexes for performance optimization
- Check constraints for data validation
- Default values and auto-increment settings
- Comments and documentation

**Note**: The SQL DDL file is a placeholder that should be generated from the ER diagram using Visual Paradigm's database generation features.

## Key Entities

### Core Entities

1. **USERS** - User accounts with authentication and RBAC
2. **DEVICES** - IoT smart plug device registry
3. **POWER_READINGS** - Time-series telemetry data
4. **SECURITY_EVENTS** - Security incidents and tamper events
5. **COMMANDS** - Device control commands with signatures

### Security Entities

6. **CERTIFICATES** - X.509 certificates for device authentication
7. **AUDIT_LOGS** - Comprehensive audit trail
8. **API_KEYS** - API access tokens with rate limits

### Feature Entities

9. **ALERTS** - User-configured alerts and notifications
10. **SCHEDULES** - Automation rules and time-based commands
11. **DEVICE_SHARING** - Multi-user device access control

## Security Features in Database Design

- **Field-level Encryption**: Sensitive fields encrypted at rest
- **Audit Logging**: Complete trail of all data modifications
- **Tamper Tracking**: tamper_count and security event logging
- **Certificate Management**: Full PKI certificate lifecycle
- **RBAC Integration**: Role-based access control in user table
- **2FA Support**: two_fa_secret and two_fa_enabled fields
- **Signature Verification**: Signature fields for data integrity

## Indexes and Performance

Key indexes for performance (see SQL DDL file for complete list):
- `idx_devices_status` - Fast device status queries
- `idx_power_readings_device_time` - Efficient telemetry queries
- `idx_security_events_device_time` - Security event monitoring
- `idx_certificates_expiry` - Certificate expiration checks
- `idx_alerts_device_active` - Active alert queries

## Relationships

### One-to-Many Relationships
- USERS → DEVICES (owner_id)
- DEVICES → POWER_READINGS
- DEVICES → SECURITY_EVENTS
- DEVICES → COMMANDS
- USERS → AUDIT_LOGS

### Many-to-Many Relationships
- USERS ↔ DEVICES (via DEVICE_SHARING for shared access)

### Self-Referencing
- CERTIFICATES → CERTIFICATES (certificate chain: issuer_cert_id)

## References

For complete class diagram specifications and service layer architecture, see `diagram_specifications.txt` in the parent directory, which includes:
- Backend service layer class diagrams
- Security components class diagram
- Firmware class structures
- Data transfer objects (DTOs)
- Repository patterns
