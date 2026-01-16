# Device Management Use Case Diagram

This diagram details device management operations including pairing, control, monitoring, and security.

## Overview

This use case diagram shows how users interact with IoT devices through the Smart Plug AI platform, including secure pairing, real-time monitoring, remote control, and security verification.

## Mermaid Diagram

<!-- Note: This diagram was copied from source_d24e744d.txt and contains the complete device management use case specification -->

```mermaid
graph TB
    subgraph "Device Management System"
        %% Actors
        User[👤 User]
        Admin[🔧 Admin]
        Mobile[📱 Mobile App]
        Device[⚡ IoT Device]
        Backend[☁️ Cloud Backend]


        %% Device Provisioning
        UC201["UC-201: Pair New Device<br/>QR Scan"]
        UC202["UC-202: Verify Device Identity"]
        UC203["UC-203: Complete Challenge-Response"]
        UC204["UC-204: Store Device Certificate"]


        %% Device Control
        UC205["UC-205: Turn Device On/Off"]
        UC206["UC-206: Schedule Device Operations"]
        UC207["UC-207: Toggle Device State"]
        UC208["UC-208: Send Signed Command"]


        %% Device Monitoring
        UC209["UC-209: View Real-time Telemetry"]
        UC210["UC-210: Monitor Device Health"]
        UC211["UC-211: Check Power Consumption"]
        UC212["UC-212: Receive Device Alerts"]


        %% Device Configuration
        UC213["UC-213: Rename/Label Device"]
        UC214["UC-214: Set Device Location"]
        UC215["UC-215: Configure Alert Thresholds"]
        UC216["UC-216: Update Device Settings"]


        %% Device Security
        UC217["UC-217: Verify Device Attestation"]
        UC218["UC-218: Review Tamper Status"]
        UC219["UC-219: Revoke Device Access"]
        UC220["UC-220: Perform Security Audit"]
    end


    %% User Relationships
    User --> UC201
    User --> UC205
    User --> UC206
    User --> UC209
    User --> UC211
    User --> UC212
    User --> UC213
    User --> UC214
    User --> UC215


    %% Admin Relationships
    Admin --> UC201
    Admin --> UC202
    Admin --> UC203
    Admin --> UC204
    Admin --> UC217
    Admin --> UC218
    Admin --> UC219
    Admin --> UC220


    %% Mobile App Relationships
    Mobile -->|"scanner"| UC201
    Mobile -->|"interface"| UC205
    Mobile -->|"display"| UC209
    Mobile -.->|"notifications"| UC212


    %% IoT Device Relationships
    Device -.->|"provides device ID"| UC202
    Device -.->|"responds to challenge"| UC203
    Device -.->|"receives commands"| UC208
    Device -.->|"publishes telemetry"| UC209
    Device -.->|"triggers alerts"| UC212
    Device -.->|"provides attestation"| UC217
    Device -.->|"detects tampering"| UC218


    %% Backend Relationships
    Backend -.->|"validates"| UC202
    Backend -.->|"generates challenge"| UC203
    Backend -.->|"stores certificate"| UC204
    Backend -.->|"signs commands"| UC208
    Backend -.->|"verifies attestation"| UC217


    %% Include Relationships
    UC201 -.->|"<<include>>"| UC202
    UC201 -.->|"<<include>>"| UC203
    UC201 -.->|"<<include>>"| UC204
    UC208 -.->|"<<include>>"| UC217


    %% Extend Relationships
    UC212 -.->|"<<extend>>"| UC209
    UC220 -.->|"<<extend>>"| UC217
    UC220 -.->|"<<extend>>"| UC218
```

## Secure Pairing Flow

1. Device shows QR with ID + challenge
2. Mobile app scans QR
3. App sends to backend for verification
4. Backend signs challenge response
5. Device verifies with ATECC608A
6. Certificate exchange completes

## Signed Command Process

1. User initiates command in app
2. Backend signs with ECDSA private key
3. Command sent via MQTT over TLS 1.3
4. Device verifies with server public key
5. ATECC608A validates signature
6. Command executed if valid

## Security Features

- Hardware-based key storage (ATECC608A)
- Challenge-response anti-replay
- ECDSA P256 signatures
- Mutual authentication
- Device attestation
- TLS 1.3 encryption
