# Component Diagrams

This folder contains component diagrams showing the system architecture and deployment structure.

## Overview

Component diagrams illustrate:
- System architecture and components
- Component dependencies and interfaces
- Deployment topology
- Network boundaries and communication paths
- Infrastructure components

## Recommended Tools

- **Primary**: Diagrams.net (draw.io) - Great for system architecture visualization
- **Backup**: Visual Paradigm - Alternative for UML component diagrams

## Diagrams in this Folder

### Main Diagrams
- **5.1_System_Architecture.drawio** - High-level system components
- **5.2_Deployment_Architecture.drawio** - Deployment topology and infrastructure

## Creating Component Diagrams

### Using Diagrams.net

1. Open https://app.diagrams.net/ or desktop application
2. Choose "Blank Diagram" template
3. Use shapes from the left panel:
   - **Rectangles** for components
   - **Cylinders** for databases
   - **Cloud** shapes for cloud services
   - **Arrows** for dependencies
4. Add layers for logical grouping
5. Export to PDF, PNG, or SVG

### Component Types to Include

#### Edge Layer Components
- ESP32-S3 Microcontroller
- ATECC608A Secure Element
- MAX6316 Tamper Watchdog
- SCT-013 Current Sensor
- Relay Module
- 24C256 EEPROM

#### Network Layer
- MQTT Broker (TLS 1.3)
- Load Balancer
- API Gateway
- Firewall

#### Application Layer
- FastAPI Backend
- Authentication Service (Auth0/Firebase)
- Redis Cache
- WebSocket Server

#### Data Layer
- PostgreSQL Database
- InfluxDB Time-series
- Cloud Storage (S3)
- Key Management Service (KMS)

#### Presentation Layer
- Mobile App (Flutter)
- Web Dashboard (React)
- Public API

### Architecture Layers

```
┌─────────────────────────────────────────────────────────┐
│         PRESENTATION LAYER (TLS 1.3)                    │
│  Mobile App | Web Dashboard | Public API                │
└─────────────────────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────┐
│         APPLICATION LAYER (OAuth2/RBAC)                 │
│  FastAPI | Auth Service | Redis | MQTT Broker          │
└─────────────────────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────┐
│         DATA LAYER (Encrypted)                          │
│  PostgreSQL | InfluxDB | S3 Storage | KMS              │
└─────────────────────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────┐
│         DEVICE LAYER (mTLS, Hardware Security)          │
│  ESP32-S3 | ATECC608A | MAX6316 | Sensors | Relay      │
└─────────────────────────────────────────────────────────┘
```

## Security Zones

Component diagrams should clearly show security zones:

1. **Public Zone**: Load balancer, CDN
2. **DMZ**: API Gateway, Web servers
3. **Application Zone**: Backend services, authentication
4. **Data Zone**: Databases with encryption
5. **Device Zone**: IoT devices with hardware security

## Communication Protocols

Annotate component connections with protocols:
- **HTTPS** (TLS 1.3) - API calls
- **MQTTS** (TLS 1.3 + mTLS) - Device telemetry
- **WSS** (WebSocket over TLS) - Real-time updates
- **gRPC** - Internal service communication

## Export Instructions

### For Presentations
1. Export as PDF (high quality)
2. Export as PNG (transparent background)
3. Export as SVG (vector, scalable)

### For Documentation
1. Save as .drawio format (editable)
2. Export diagrams to `Smart_Plug_AI_Diagrams/7_Exports/`
3. Include in overall documentation

## References

See the main `ARCHITECTURE.md` document in `/docs` for detailed component descriptions and the text-based architecture diagram.

For export files and final presentations, see `Smart_Plug_AI_Diagrams/7_Exports/` folder.
