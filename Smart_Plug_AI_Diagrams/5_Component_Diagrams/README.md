# Component Diagrams

This folder contains system architecture and deployment component diagrams.

## Expected Files

The following diagram files will be added to this directory:

- **5.1_System_Architecture.drawio** - High-level system architecture showing all components and their interactions
- **5.2_Deployment_Architecture.drawio** - Deployment view with infrastructure, cloud services, and networking

## Diagram Contents

### System Architecture
Components:
- **Device Layer**: ESP32-S3, ATECC608A, MAX6316, SCT-013 sensor, relay module, 24C256 EEPROM
- **Mobile App Layer**: Flutter app, Firebase Auth, local storage, push notifications
- **Web Dashboard Layer**: React app, OAuth2/SSO, real-time charts, admin console
- **Backend Layer**: FastAPI services, Auth0/Firebase Auth, MQTT broker, WebSocket server
- **Data Layer**: PostgreSQL/Firestore, InfluxDB (time-series), Redis (cache), cloud storage
- **Security Layer**: Certificate Authority, HSM, KMS, SIEM system, audit logs
- **Intelligence Layer**: ML models, anomaly detection, pattern recognition, analytics

Connections:
- MQTT over TLS 1.3 (Device ↔ Backend)
- WebSocket over TLS (Backend ↔ Clients)
- REST API over HTTPS (App ↔ Backend)
- Encrypted database connections

### Deployment Architecture
Infrastructure:
- **Edge**: IoT devices in user homes/facilities
- **Network**: MQTT broker cluster with TLS termination
- **Application**: Containerized microservices (Docker/Kubernetes)
- **Database**: Managed PostgreSQL, InfluxDB Cloud, Redis cluster
- **Storage**: Encrypted blob storage for logs and exports
- **CDN**: Static assets and firmware distribution
- **Monitoring**: Prometheus, Grafana, ELK stack
- **Security**: WAF, DDoS protection, certificate management

Geographic distribution:
- Primary region: South Africa (Johannesburg data center)
- Backup region: Europe (GDPR compliance)
- CDN edge locations: Global

## Tools

- **Primary Tool**: Diagrams.net (draw.io) - Great for system architecture visualization
- **Backup Tool**: Visual Paradigm

## Drawing Notes

This directory includes a note file about using Diagrams.net for collaborative diagram creation:
- Cloud storage integration (Google Drive, OneDrive)
- Version control with Git
- Export formats (PNG, SVG, PDF)
- Embedding in documentation

## Architecture Principles

All component diagrams follow these principles:
- **Security by Design**: Bank-grade security at every layer
- **Scalability**: Horizontal scaling for all services
- **Resilience**: High availability and disaster recovery
- **Compliance**: GDPR, POPIA, SOC2 readiness
- **Observability**: Comprehensive monitoring and logging
