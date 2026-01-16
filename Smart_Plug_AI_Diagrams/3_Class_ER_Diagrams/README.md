# Class and ER Diagrams

This folder contains UML Class Diagrams and Entity-Relationship (ER) diagrams for the Smart Plug AI system.

## Expected Files

The following diagram files will be added to this directory:

- **3.1_Class_Diagram_Backend.vpp** - Backend service layer class diagram with domain models
- **3.2_Class_Diagram_Firmware.vpp** - ESP32 firmware class structure and hardware interfaces
- **3.3_ER_Diagram_Database.vpp** - Complete entity-relationship diagram for PostgreSQL/Firestore database

## Additional Files

- **3.4_SQL_DDL_Generated.sql** - SQL Data Definition Language generated from ER diagram (includes table schemas, indexes, constraints)

## Diagram Contents

### Backend Class Diagram
- **Core Domain Entities**: User, Device, PowerReading, SecurityEvent, Command, Certificate, Alert
- **Service Layer**: DeviceService, TelemetryService, CommandService, AlertService, UserService, MQTTService, NotificationService, AuditService
- **Security Components**: SecurityService, KeyManagementService, CertificateService, SignatureService, EncryptionService
- **Repository Interfaces**: DeviceRepository, UserRepository, CommandRepository
- **Data Transfer Objects**: DeviceDTO, PowerReadingDTO, CommandDTO, AlertDTO

### Firmware Class Diagram
- **Hardware Interfaces**: ATECC608AInterface, MAX6316Interface, SecureEEPROM, ESP32Security
- **Security Managers**: TLSManager, KeyManager, CertificateManager
- **Device Controllers**: RelayController, SensorController, TelemetryController
- **Communication**: MQTTClient, HTTPClient, BluetoothProvisioning

### ER Diagram
- **Primary Entities**: USERS, DEVICES, POWER_READINGS, SECURITY_EVENTS, COMMANDS, CERTIFICATES, ALERTS, AUDIT_LOGS
- **Supporting Entities**: DEVICE_SHARING, SCHEDULES, API_KEYS
- **Relationships**: One-to-many, many-to-many with junction tables
- **Indexes**: Performance optimization indexes for time-series queries
- **Constraints**: Foreign key constraints with appropriate cascade rules

## Tools

- **Primary Tool**: Visual Paradigm Community Edition (excellent database modeling tools)
- **Backup Tool**: Diagrams.net

## Database Design Principles

- Field-level encryption for sensitive data (AES-256-GCM)
- Comprehensive indexing for performance
- Time-series optimization for telemetry data (InfluxDB + PostgreSQL)
- Audit logging with immutable records
- GDPR/POPIA compliance with data retention policies
