# Authentication & Security Use Case Diagram

This diagram focuses on authentication and security-related use cases in the Smart Plug AI system.

## Overview

This use case diagram illustrates the authentication flow, session management, access control, and security features that protect the Smart Plug AI platform.

## Mermaid Diagram

```mermaid
graph TB
    subgraph "Authentication & Security System"
        %% Actors
        User[👤 User]
        Admin[🔧 Admin]
        System[⚡ System]


        %% Authentication Flow
        UC101["UC-101: Login with Credentials"]
        UC102["UC-102: Complete 2FA/MFA"]
        UC103["UC-103: Logout / Terminate Session"]
        UC104["UC-104: Recover Lost Credentials"]


        %% Session Management
        UC105["UC-105: Manage Active Sessions"]
        UC106["UC-106: Renew Expired Tokens"]
        UC107["UC-107: Revoke Compromised Sessions"]


        %% Access Control
        UC108["UC-108: View Role Permissions"]
        UC109["UC-109: Request Elevated Access"]
        UC110["UC-110: Audit Access Attempts"]


        %% Security Features
        UC111["UC-111: Configure Biometric Auth"]
        UC112["UC-112: Review Login History"]
        UC113["UC-113: Set Password Policies"]
        UC114["UC-114: Manage API Keys"]
    end


    %% User Relationships
    User --> UC101
    User --> UC102
    User --> UC103
    User --> UC104
    User --> UC105
    User --> UC106
    User --> UC111


    %% Admin Relationships
    Admin --> UC101
    Admin --> UC102
    Admin --> UC103
    Admin --> UC107
    Admin --> UC108
    Admin --> UC109
    Admin --> UC113
    Admin --> UC114


    %% System Relationships
    System -.->|"validates"| UC101
    System -.->|"verifies"| UC102
    System -.->|"auto-triggers"| UC106
    System -.->|"auto-detects"| UC107


    %% Include Relationships
    UC102 -.->|"<<include>>"| UC101
    UC109 -.->|"<<include>>"| UC102
    UC105 -.->|"<<include>>"| UC101


    %% Extend Relationships
    UC104 -.->|"<<extend>>"| UC102
    UC107 -.->|"<<extend>>"| UC110
```

## 2FA/MFA Options

- **TOTP** (Google Authenticator)
- **SMS Verification**
- **Push Notification**
- **Biometric Verification**

## RBAC Roles

- **Admin**: Full system access
- **User**: Device management
- **Viewer**: Read-only
- **Auditor**: Security logs only
- **Service**: API access

## Security Features

- TLS 1.3 for all communications
- Rate limiting: 5 attempts per 15 minutes
- JWT tokens with short expiry
- Refresh token rotation
- Biometric integration
- Session anomaly detection
- Audit logging of all authentication attempts
