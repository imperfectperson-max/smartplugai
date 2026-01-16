# Security Operations Use Case Diagram

This diagram covers security operations, threat detection, incident response, and compliance.

## Overview

This use case diagram illustrates the Security Operations Center (SOC) capabilities, including threat detection, incident response, forensics, compliance auditing, and device security management.

## Mermaid Diagram

<!-- Note: This diagram includes comprehensive security operations with tamper detection flow and device attestation -->

```mermaid
graph TB
    subgraph "Security Operations Center"
        %% Actors
        SecOfficer[🛡️ Security Officer]
        SysAdmin[🔧 System Admin]
        Auditor[📋 Compliance Auditor]
        Device[⚡ IoT Device]
        AlertSystem[🚨 Alert System]
        SIEM[📊 SIEM System]


        %% Threat Detection
        UC401["UC-401: Detect Physical Tampering"]
        UC402["UC-402: Identify Anomalous Activity"]
        UC403["UC-403: Monitor Network Intrusions"]
        UC404["UC-404: Track Authentication Failures"]


        %% Incident Response
        UC405["UC-405: Receive Security Alerts"]
        UC406["UC-406: Triage Security Incidents"]
        UC407["UC-407: Contain Security Breaches"]
        UC408["UC-408: Remediate Security Issues"]


        %% Forensics & Investigation
        UC409["UC-409: Collect Forensic Evidence"]
        UC410["UC-410: Analyze Attack Patterns"]
        UC411["UC-411: Trace Compromised Devices"]
        UC412["UC-412: Document Security Incidents"]


        %% Compliance & Auditing
        UC413["UC-413: Generate Compliance Reports"]
        UC414["UC-414: Audit Security Controls"]
        UC415["UC-415: Verify Certificate Validity"]
        UC416["UC-416: Review Access Logs"]


        %% Device Security
        UC417["UC-417: Verify Device Attestation"]
        UC418["UC-418: Manage Security Certificates"]
        UC419["UC-419: Revoke Compromised Devices"]
        UC420["UC-420: Enforce Security Policies"]


        %% Security Management
        UC421["UC-421: Update Security Configurations"]
        UC422["UC-422: Manage Encryption Keys"]
        UC423["UC-423: Conduct Security Drills"]
        UC424["UC-424: Train Security Personnel"]
    end


    %% Security Officer Relationships
    SecOfficer --> UC401
    SecOfficer --> UC405
    SecOfficer --> UC406
    SecOfficer --> UC407
    SecOfficer --> UC408
    SecOfficer --> UC409
    SecOfficer --> UC410
    SecOfficer --> UC411
    SecOfficer --> UC412
    SecOfficer --> UC421
    SecOfficer --> UC422
    SecOfficer --> UC423
    SecOfficer --> UC424


    %% System Admin Relationships
    SysAdmin --> UC401
    SysAdmin --> UC407
    SysAdmin --> UC408
    SysAdmin --> UC415
    SysAdmin --> UC417
    SysAdmin --> UC418
    SysAdmin --> UC419
    SysAdmin --> UC420


    %% Compliance Auditor Relationships
    Auditor --> UC413
    Auditor --> UC414
    Auditor --> UC416
    Auditor --> UC417


    %% IoT Device Relationships
    Device -.->|"triggers via MAX6316"| UC401
    Device -.->|"provides attestation data"| UC417
    Device -.->|"uses certificates"| UC418
    Device -.->|"gets revoked"| UC419


    %% Alert System Relationships
    AlertSystem -.->|"sends real-time alerts"| UC405
    AlertSystem -.->|"escalates tamper events"| UC401
    AlertSystem -.->|"notifies anomalies"| UC402
    AlertSystem -.->|"reports failed logins"| UC404


    %% SIEM System Relationships
    SIEM -.->|"correlates events"| UC402
    SIEM -.->|"analyzes traffic"| UC403
    SIEM -.->|"provides logs"| UC410
    SIEM -.->|"aggregates audit data"| UC416


    %% Include Relationships
    UC405 -.->|"<<include>>"| UC401
    UC405 -.->|"<<include>>"| UC402
    UC405 -.->|"<<include>>"| UC403
    UC405 -.->|"<<include>>"| UC404
    UC407 -.->|"<<include>>"| UC406
    UC408 -.->|"<<include>>"| UC407
    UC414 -.->|"<<include>>"| UC416
    UC414 -.->|"<<include>>"| UC417


    %% Extend Relationships
    UC419 -.->|"<<extend>>"| UC401
    UC419 -.->|"<<extend>>"| UC403
    UC419 -.->|"<<extend>>"| UC411
    UC423 -.->|"<<extend>>"| UC405
    UC423 -.->|"<<extend>>"| UC407
```

## Security Framework Compliance

- **NIST Cybersecurity Framework**
- **OWASP IoT Security Top 10**
- **SABS/ICASA Compliance**
- **SOC2 Type II Controls**
- **POPIA/GDPR Data Protection**
- **Certificates**: ECDSA P256, TLS 1.3

## Tamper Detection Flow

1. MAX6316 detects enclosure opening
2. Triggers ESP32 reset
3. Sends tamper alert via MQTT/TLS
4. Backend logs security event
5. User notified via push notification
6. Device marked as 'tampered'
7. Requires re-verification

## Device Attestation

- Firmware hash verification
- Secure boot status check
- Flash encryption status
- ATECC608A key validation
- Certificate chain verification
- Reported every 24h

## Security Features

- Hardware tamper detection
- Immediate response (<100ms)
- Secure erase capability
- Multi-channel notifications
- Forensic logging
- Manual security review
- Challenge-response re-verification
