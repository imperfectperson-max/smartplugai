# Gantt Charts

This folder contains project timeline and scheduling diagrams for Smart Plug AI development.

## Overview

Gantt charts provide:
- Project timelines and milestones
- Task dependencies and sequencing
- Resource allocation over time
- Phase-based development planning
- Security milestone tracking

## Recommended Tools

- **Primary**: Visual Paradigm Community Edition - Built-in project management features
- **Backup**: Diagrams.net - Alternative for timeline visualization

## Diagrams in this Folder

### Main Diagrams
- **6.1_Project_Timeline.vpp** - Complete project timeline (Phases 1-3)
- **6.2_Phase1_Detailed.vpp** - Detailed Phase 1 breakdown (Weeks 1-12)

## Phase 1: Detailed Gantt Chart (Weeks 1-12)

The following Mermaid Gantt chart shows the detailed week-by-week timeline for Phase 1 (Secure MVP Development):

<!-- Note: This diagram was copied from source_d24e744d.txt - shows complete Phase 1 security milestones -->

```mermaid
gantt
    title Phase 1: Secure MVP Development (Detailed Week-by-Week)
    dateFormat YYYY-MM-DD
    axisFormat %W


    section Week 1-2: Hardware & Firmware Foundation
    Component Testing & Verification :w1_1, 2025-01-06, 4d
    ATECC608A Provisioning          :w1_2, 2025-01-08, 3d
    Secure Boot + Flash Encryption  :w1_3, 2025-01-11, 2d
    MQTT over TLS 1.3 Setup         :w1_4, 2025-01-13, 3d
    Tamper Detection Integration    :w1_5, 2025-01-16, 2d
    Relay Control System            :w1_6, 2025-01-18, 2d
    Power Measurement Calibration   :w1_7, 2025-01-20, 2d


    section Week 3-4: Mobile App Security Features
    Flutter App Setup with 2FA      :w3_1, 2025-01-20, 3d
    Real-time Power Monitoring UI   :w3_2, 2025-01-23, 3d
    Secure QR Code Pairing          :w3_3, 2025-01-26, 4d
    Device Control with Signatures  :w3_4, 2025-01-30, 2d
    Historical Data Display         :w3_5, 2025-02-01, 2d
    Security Status Dashboard       :w3_6, 2025-02-03, 2d
    Push Notifications System       :w3_7, 2025-02-05, 2d


    section Week 5-6: Cloud Backend Security
    Firestore with Field Encryption :w5_1, 2025-02-05, 3d
    Cloud Functions with RBAC       :w5_2, 2025-02-08, 3d
    WebSocket over TLS              :w5_3, 2025-02-11, 3d
    MQTT Broker with mTLS           :w5_4, 2025-02-14, 3d
    Signed Command System           :w5_5, 2025-02-17, 3d
    Device Attestation Endpoints    :w5_6, 2025-02-20, 2d
    Audit Logging System            :w5_7, 2025-02-22, 2d


    section Week 7-8: Safety & Enclosure
    PCB Design with Security Zones  :w7_1, 2025-02-24, 4d
    3D-printed Enclosure v1         :w7_2, 2025-02-28, 3d
    Mains Wiring with Safety        :w7_3, 2025-03-03, 4d
    Thermal Monitoring System       :w7_4, 2025-03-07, 2d
    Overload Protection Circuit     :w7_5, 2025-03-09, 2d
    Physical Tamper Mechanisms      :w7_6, 2025-03-11, 2d
    Assembly Documentation          :w7_7, 2025-03-13, 3d


    section Week 9-10: Rule Engine & Automation
    Time-based Scheduling System    :w9_1, 2025-03-17, 3d
    Manual Rules with 2FA           :w9_2, 2025-03-20, 3d
    Encrypted Alert System          :w9_3, 2025-03-23, 2d
    Load-shedding Detection         :w9_4, 2025-03-25, 2d
    Rate Limiting Implementation    :w9_5, 2025-03-27, 2d
    Anti-replay Protection          :w9_6, 2025-03-29, 3d
    Rule Engine Testing             :w9_7, 2025-04-01, 3d


    section Week 11-12: Testing & Security Audit
    10-device Pilot Deployment      :w11_1, 2025-04-03, 3d
    User Testing & Feedback         :w11_2, 2025-04-06, 5d
    Security Penetration Testing    :w11_3, 2025-04-11, 4d
    Bug Fixing & Improvements       :w11_4, 2025-04-15, 3d
    UI/UX Refinement                :w11_5, 2025-04-18, 3d
    ML Data Collection Setup        :w11_6, 2025-04-21, 2d
    Security Audit Report           :w11_7, 2025-04-23, 2d


    %% Milestones
    Hardware Ready              :milestone, milestone_hw, 2025-01-20, 0d
    Mobile App Ready            :milestone, milestone_app, 2025-02-05, 0d
    Backend Ready               :milestone, milestone_backend, 2025-02-24, 0d
    Enclosure Ready             :milestone, milestone_enclosure, 2025-03-17, 0d
    Phase 1 Complete            :milestone, milestone_phase1, 2025-04-25, 0d
```

## Phase 1 Overview

**Duration**: 12 weeks (January - April 2025)  
**Budget**: R5,000  
**Goal**: Secure MVP with 3 functional prototypes

### Key Milestones

1. **Week 2**: Hardware components tested and ATECC608A provisioned
2. **Week 4**: Mobile app with secure pairing functional
3. **Week 6**: Cloud backend with full security stack operational
4. **Week 8**: Physical enclosure with tamper detection complete
5. **Week 10**: Automation and rule engine tested
6. **Week 12**: Security audit passed, MVP ready for Phase 2

## Security Milestones

Critical security implementations by week:
- **Week 1**: Secure boot, flash encryption, ATECC608A
- **Week 2**: MQTT over TLS 1.3, tamper detection
- **Week 4**: 2FA, QR pairing, signed commands
- **Week 6**: mTLS, device attestation, audit logging
- **Week 10**: Rate limiting, anti-replay protection
- **Week 12**: Penetration testing, security audit

## Phases 2 & 3

### Phase 2: Pilot & Refinement (Months 5-8)
- Production-ready PCB design
- 500-device pilot deployment
- SABS/ICASA certification
- Advanced analytics features
- Budget: R3,000,000

### Phase 3: Commercial Launch (Months 9-12)
- First 10,000 units manufactured
- Retail partnerships established
- Enterprise features
- SOC2 Type I certification
- Budget: R6,000,000

## Risk Management

Key risks tracked in timeline:
- **Technical**: Hardware failures, software bugs
- **Security**: Vulnerabilities, penetration test failures
- **Regulatory**: SABS/ICASA approval delays
- **Supply Chain**: Component availability
- **Market**: Customer adoption, competitor response

## Usage Notes

For creating Gantt charts in Visual Paradigm:
1. Create New Diagram → Gantt Chart
2. Add tasks with start dates and durations
3. Define dependencies between tasks
4. Add milestones for key dates
5. Assign resources to tasks
6. Export to PDF or image formats

## References

See `diagram_specifications.txt` for additional timeline diagrams including:
- Security milestone timeline
- Risk management timeline
- Deliverable timeline
- Complete Phase 2 & 3 schedules
