# Smart Plug AI Diagrams

This directory contains comprehensive architectural and design diagrams for the Smart Plug AI project, documenting the bank-grade security IoT platform.

## 📁 Directory Structure

```
Smart_Plug_AI_Diagrams/
├── 1_Use_Case_Diagrams/          # UML Use Case diagrams
├── 2_Activity_Diagrams/          # UML Activity diagrams  
├── 3_Class_ER_Diagrams/          # Class and ER diagrams
├── 4_Sequence_Diagrams/          # UML Sequence diagrams
├── 5_Component_Diagrams/         # System architecture diagrams
├── 6_Gantt_Charts/               # Project timeline charts
├── 7_Exports/                    # Exported PDFs and presentations
├── MERMAID_EXAMPLES.md           # Mermaid examples and usage guide
└── README.md                     # This file
```

## 📋 Overview

This directory serves as the central repository for all architectural diagrams, design documentation, and visual models for the Smart Plug AI system. The diagrams follow industry-standard UML 2.5 notation and incorporate security-first design principles.

## 🎯 Purpose

These diagrams serve multiple purposes:

1. **Development Reference**: Guide implementation across hardware, firmware, mobile, backend, and web teams
2. **Security Documentation**: Document security architecture and threat models for audits
3. **Stakeholder Communication**: Provide clear visualizations for investors, partners, and customers
4. **Compliance Requirements**: Support SABS, ICASA, SOC2, and GDPR certification submissions
5. **Academic Submission**: Document university project architecture and design decisions
6. **Team Onboarding**: Help new team members understand system architecture quickly

## 🛠️ Recommended Tools

Based on the comprehensive analysis in the original specification:

### Primary Tool: Visual Paradigm Community Edition
- **Best for**: UML diagrams (use cases, activities, sequences, classes, ER diagrams)
- **License**: Free for students and academic projects
- **Strengths**: 
  - Excellent UML 2.5 support with auto-layout
  - Superior database modeling and SQL generation
  - Built-in project management for Gantt charts
  - Code generation and reverse engineering
  - Team collaboration features
- **Download**: https://www.visual-paradigm.com/download/community.jsp

### Backup Tool: Diagrams.net (draw.io)
- **Best for**: Component diagrams, system architecture, deployment diagrams
- **License**: Free and open source
- **Strengths**:
  - Great for system architecture visualization
  - Rich icon libraries (AWS, Azure, network, IoT)
  - Cloud storage integration
  - No installation required (web-based)
  - Git-friendly XML format
- **Access**: https://app.diagrams.net/

## 📚 Diagram Categories

### 1. Use Case Diagrams
- **Location**: `1_Use_Case_Diagrams/`
- **Content**: Actor interactions, system boundaries, use case relationships
- **Coverage**: Authentication, device management, analytics, security operations
- **Primary Actors**: Homeowner, Admin, Security Auditor, IoT Device, Mobile App, Web Dashboard

### 2. Activity Diagrams  
- **Location**: `2_Activity_Diagrams/`
- **Content**: Workflows, process flows, decision points, parallel activities
- **Key Flows**: Secure device pairing, signed command execution, tamper detection response
- **Security Features**: Hardware crypto, ECDSA signatures, challenge-response, attestation

### 3. Class & ER Diagrams
- **Location**: `3_Class_ER_Diagrams/`
- **Content**: Domain models, service classes, database schema, relationships
- **Backend Classes**: User, Device, PowerReading, SecurityEvent, Command, Certificate, Alert
- **Security Classes**: ATECC608A interface, TLS manager, encryption service, key management
- **Database**: Complete ER diagram with entities, indexes, constraints, encryption requirements

### 4. Sequence Diagrams
- **Location**: `4_Sequence_Diagrams/`
- **Content**: Time-ordered interactions, message flows, security checkpoints
- **Critical Sequences**: Control commands, tamper events, OTA updates, telemetry, authentication
- **Participants**: Mobile app, backend API, MQTT broker, IoT device, ATECC608A, security services

### 5. Component Diagrams
- **Location**: `5_Component_Diagrams/`
- **Content**: System architecture, deployment view, infrastructure
- **Layers**: Device, mobile, web, backend, data, security, intelligence
- **Infrastructure**: Cloud services, networking, monitoring, security appliances

### 6. Gantt Charts
- **Location**: `6_Gantt_Charts/`
- **Content**: Project timeline, resource allocation, milestones, dependencies
- **Phases**: Phase 1 (MVP, 12 weeks), Phase 2 (Pilot, 4 months), Phase 3 (Commercial, 4 months)
- **Tracking**: Security milestones, risk management, deliverables

### 7. Exports
- **Location**: `7_Exports/`
- **Content**: PDF compilations, PowerPoint presentations, high-resolution images
- **Formats**: PDF (documentation), PPTX (presentations), PNG/SVG (web/print)

## 🔒 Security Architecture Highlights

All diagrams incorporate these security principles:

- **Hardware Security**: ESP32-S3 secure boot, ATECC608A cryptographic co-processor, MAX6316 tamper watchdog
- **Communication Security**: TLS 1.3, mutual authentication, certificate pinning, ECDSA signatures
- **Device Security**: QR-based pairing, challenge-response authentication, device attestation, tamper detection
- **Backend Security**: OAuth2/2FA, RBAC, field-level encryption, signed commands, audit logging
- **Data Security**: AES-256-GCM encryption, end-to-end encryption, secure key management

## 📖 Reference Documents

This diagram collection is based on the comprehensive specification document that includes:

- **Tool Selection Recommendations**: Comparative analysis of diagramming tools
- **File Organization**: Structured approach to diagram management
- **Detailed Specifications**: Complete requirements for each diagram type
- **Mermaid Examples**: Quick-reference Mermaid code for all diagrams
- **Security Features**: Comprehensive security architecture documentation
- **Project Timeline**: Week-by-week Phase 1 implementation plan

See `MERMAID_EXAMPLES.md` for information about Mermaid diagram examples and how they relate to the formal UML diagrams.

## 🚀 Getting Started

### For Diagram Creators

1. **Review Specifications**: Check the README in each subdirectory for expected content
2. **Choose Tool**: Use Visual Paradigm for UML, Diagrams.net for component diagrams
3. **Follow Conventions**: Maintain consistent notation, colors, and styling
4. **Include Security**: Highlight security features in diagrams
5. **Document**: Add notes explaining complex interactions
6. **Export**: Generate PNG/SVG for documentation, PDF for formal submissions
7. **Version Control**: Store source files (.vpp, .drawio) in Git

### For Diagram Consumers

1. **Navigate**: Use the directory structure to find specific diagram types
2. **Read READMEs**: Each subdirectory explains its contents and expected files
3. **View Exports**: Check `7_Exports/` for compiled PDFs and presentations
4. **Check Mermaid**: See `MERMAID_EXAMPLES.md` for quick-reference examples
5. **Verify Currency**: Check file dates and version numbers for latest versions

## 📝 Diagram Status

Currently, this directory contains:
- ✅ Folder structure (complete)
- ✅ README files with specifications (complete)
- ✅ .gitkeep files for empty directories (complete)
- ✅ Placeholder files (complete)
- ⏳ Actual .vpp diagram files (to be added in follow-up PRs)
- ⏳ Actual .drawio diagram files (to be added in follow-up PRs)
- ⏳ Exported PDFs and presentations (to be generated after diagrams are complete)

## 🤝 Contributing

When adding diagrams to this repository:

1. **Follow Structure**: Place files in the appropriate subdirectory
2. **Use Naming Convention**: Match the expected filenames listed in README files
3. **Include Source Files**: Commit both source (.vpp, .drawio) and exports (PNG, PDF)
4. **Update Documentation**: Mark files as complete in README files
5. **Quality Check**: Ensure diagrams are readable, accurate, and follow UML standards
6. **Security Review**: Verify no sensitive information (keys, passwords) in diagrams
7. **Version Control**: Include version numbers and dates in exports

## 📞 Questions and Support

For questions about:
- **Diagram Content**: Refer to the specification documents and README files in each subdirectory
- **Tool Usage**: See tool documentation links in `MERMAID_EXAMPLES.md`
- **Security Architecture**: Review `docs/SECURITY.md` in the project root
- **Project Timeline**: See Gantt charts in `6_Gantt_Charts/`
- **General Project**: Check main project README at repository root

## 🎓 Academic Context

This diagram collection supports a university project for Smart Plug AI, South Africa's first bank-grade security IoT platform. The diagrams document:

- Phase 1 secure MVP development (12 weeks, R5,000 budget)
- Hardware security implementation (ESP32-S3 + ATECC608A + MAX6316)
- Mobile app with 2FA and QR-based secure pairing
- Cloud backend with signed commands and device attestation
- Comprehensive security architecture for SOC2 compliance readiness

## 📅 Timeline

- **Week 1-2**: Hardware & firmware foundation
- **Week 3-4**: Mobile app security features
- **Week 5-6**: Cloud backend security
- **Week 7-8**: Safety & enclosure
- **Week 9-10**: Rule engine & automation
- **Week 11-12**: Testing & security audit

## 📄 License

Diagrams and documentation are part of the Smart Plug AI project, licensed under MIT License. See the LICENSE file in the project root for details.

---

**Last Updated**: January 2025  
**Status**: Folder structure complete, diagram files to be added in follow-up PRs  
**Version**: 1.0
