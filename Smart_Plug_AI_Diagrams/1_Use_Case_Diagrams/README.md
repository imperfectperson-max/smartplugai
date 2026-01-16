# Use Case Diagrams

This folder contains use case diagrams that illustrate the interactions between users, systems, and the Smart Plug AI platform.

## Overview

Use case diagrams document the functional requirements from the user's perspective, showing:
- System actors (users, devices, services)
- Use cases (functional requirements)
- Relationships between actors and use cases
- System boundaries

## Recommended Tools

- **Primary**: Visual Paradigm Community Edition - Excellent UML support with auto-layout
- **Backup**: Diagrams.net - Web-based alternative for quick edits

## Diagrams in this Folder

### Main Diagrams
- **1.1_System_Use_Case.vpp** - Complete system overview with all actors and use cases
- **1.2_User_Use_Case.vpp** - User-focused interactions
- **1.3_Admin_Use_Case.vpp** - Administrator operations

### Additional Diagrams (Markdown)
- **authentication_security.md** - Authentication & Security use cases
- **device_management.md** - Device Management use cases
- **data_analytics.md** - Data Analytics & Reporting use cases
- **security_operations.md** - Security Operations Center use cases

## System Overview Use Case Diagram

The following Mermaid diagram provides a high-level overview of the Smart Plug AI system with all primary actors and use cases:

```mermaid
graph TB
    subgraph "Smart Plug AI System"
        %% Actors
        Homeowner[👤 Homeowner]
        Admin[🔧 System Admin]
        Auditor[🛡️ Security Auditor]
        Device[⚡ IoT Device]
        MobileApp[📱 Mobile App]
        WebDashboard[💻 Web Dashboard]


        %% Use Cases - Authentication & Security
        UC1["UC-1: Authenticate with 2FA/MFA"]
        UC2["UC-2: Manage User Profile & Settings"]
        UC3["UC-3: Review Security Audit Logs"]
        UC4["UC-4: Configure Security Policies"]


        %% Use Cases - Device Management
        UC5["UC-5: Pair New Device<br/>QR + Challenge-Response"]
        UC6["UC-6: Monitor Real-time Power Usage<br/>Encrypted Data"]
        UC7["UC-7: Control Device Remotely<br/>Signed Commands"]
        UC8["UC-8: View Device Status & Health"]


        %% Use Cases - Data & Analytics
        UC9["UC-9: View Historical Analytics"]
        UC10["UC-10: Generate Usage Reports"]
        UC11["UC-11: Export Data for Analysis"]
        UC12["UC-12: Set Energy Usage Alerts"]


        %% Use Cases - System Administration
        UC13["UC-13: Manage User Accounts<br/>RBAC"]
        UC14["UC-14: Configure System Settings"]
        UC15["UC-15: Monitor System Health"]
        UC16["UC-16: Update Firmware<br/>Secure OTA"]


        %% Use Cases - Security Operations
        UC17["UC-17: Respond to Tamper Alerts"]
        UC18["UC-18: Review Device Attestation"]
        UC19["UC-19: Manage Security Certificates"]
        UC20["UC-20: Investigate Security Incidents"]
    end


    %% Homeowner Relationships
    Homeowner --> UC1
    Homeowner --> UC2
    Homeowner --> UC5
    Homeowner --> UC6
    Homeowner --> UC7
    Homeowner --> UC8
    Homeowner --> UC9
    Homeowner --> UC10
    Homeowner --> UC12
    Homeowner -.->|"receives alerts"| UC17


    %% Admin Relationships
    Admin --> UC1
    Admin --> UC3
    Admin --> UC4
    Admin --> UC13
    Admin --> UC14
    Admin --> UC15
    Admin --> UC16
    Admin --> UC18
    Admin --> UC19
    Admin --> UC20


    %% Auditor Relationships
    Auditor --> UC1
    Auditor --> UC3
    Auditor --> UC4
    Auditor --> UC18
    Auditor --> UC19
    Auditor --> UC20


    %% System Actors
    MobileApp --> UC1
    MobileApp --> UC5
    MobileApp --> UC6
    MobileApp --> UC7
    MobileApp --> UC8
    MobileApp -.->|"push notification"| UC17


    WebDashboard --> UC1
    WebDashboard --> UC3
    WebDashboard --> UC9
    WebDashboard --> UC10
    WebDashboard --> UC13
    WebDashboard --> UC15
    WebDashboard --> UC18
    WebDashboard --> UC19
    WebDashboard --> UC20


    Device -.->|"publishes"| UC6
    Device -.->|"receives"| UC7
    Device -.->|"reports"| UC8
    Device -.->|"receives"| UC16
    Device -.->|"triggers"| UC17
    Device -.->|"provides"| UC18


    %% Include Relationships
    UC7 -.->|"<<include>>"| UC1
    UC13 -.->|"<<include>>"| UC1
    UC16 -.->|"<<include>>"| UC1
    UC20 -.->|"<<include>>"| UC3


    %% Extend Relationships
    UC17 -.->|"<<extend>>"| UC12
    UC18 -.->|"<<extend>>"| UC8


    %% Styling
    classDef actor fill:#bbdefb,stroke:#1565c0
    classDef useCase fill:#e8f5e8,stroke:#2e7d32
    classDef securityUseCase fill:#ffebee,stroke:#c62828
    classDef include fill:#fff3e0,stroke:#ff8f00,dashed
    classDef extend fill:#e1bee7,stroke:#7b1fa2,dashed


    class Homeowner,Admin,Auditor,Device,MobileApp,WebDashboard actor
    class UC1,UC2,UC5,UC6,UC7,UC8,UC9,UC10,UC11,UC12,UC13,UC14,UC15,UC16 useCase
    class UC3,UC4,UC17,UC18,UC19,UC20 securityUseCase
```

## Primary Actors

1. **Homeowner** - Primary user, residential customer
2. **Facility Manager** - Commercial/industrial user
3. **System Administrator** - Technical administrator
4. **Security Auditor** - Compliance and security reviewer
5. **IoT Device** - ESP32-S3 smart plug
6. **Mobile Application** - Flutter-based mobile interface
7. **Web Dashboard** - React-based admin interface

## Main Use Case Domains

### Authentication & Security Domain
- UC-1: Authenticate with 2FA/MFA
- UC-2: Manage User Profile & Settings
- UC-3: Review Security Audit Logs
- UC-4: Configure Security Policies

### Device Management Domain
- UC-5: Pair New Device (QR + Challenge-Response)
- UC-6: Monitor Real-time Power Usage
- UC-7: Control Device Remotely (Signed Commands)
- UC-8: View Device Status & Health

### Data & Analytics Domain
- UC-9: View Historical Analytics
- UC-10: Generate Usage Reports
- UC-11: Export Data for Analysis
- UC-12: Set Energy Usage Alerts

### System Administration Domain
- UC-13: Manage User Accounts (RBAC)
- UC-14: Configure System Settings
- UC-15: Monitor System Health
- UC-16: Update Firmware (Secure OTA)

### Security Operations Domain
- UC-17: Respond to Tamper Alerts
- UC-18: Review Device Attestation Status
- UC-19: Manage Security Certificates
- UC-20: Investigate Security Incidents

## Security Features

All use cases incorporate comprehensive security features:
- **Hardware-based security** with ATECC608A secure element
- **Challenge-response authentication** for device pairing
- **ECDSA P256 signatures** for command verification
- **Mutual TLS 1.3** for all communications
- **Role-based access control** (RBAC)
- **Comprehensive audit logging**
- **Tamper detection and response**

## Usage Notes

For creating or editing use case diagrams:
1. Open Visual Paradigm Community Edition
2. Create new Use Case Diagram
3. Add actors from the palette (stick figures)
4. Add use cases (ovals)
5. Connect with associations (solid lines)
6. Use <<include>> and <<extend>> for relationships
7. Define system boundary with a rectangle
8. Export to PDF/PNG for presentations

## References

See `diagram_specifications.txt` in the parent directory for complete diagram specifications and additional details.
