# Smart Plug AI - Architecture Diagrams Collection

This directory contains comprehensive architecture diagrams and documentation for the Smart Plug AI project, South Africa's first bank-grade security IoT platform.

## 📋 Overview

This diagrams collection provides visual representations of the entire Smart Plug AI system architecture, including:

- **Use Case Diagrams**: System interactions and user scenarios
- **Activity Diagrams**: Process flows and state transitions
- **Class & ER Diagrams**: Data models and system structure
- **Sequence Diagrams**: Component interactions and message flows
- **Component Diagrams**: System architecture visualization
- **Gantt Charts**: Project timelines and milestones

## 🛠️ Recommended Tools

For creating and editing these diagrams, we recommend:

### Primary Tool: Visual Paradigm Community Edition
- **Best for**: Use Case, Activity, Class, ER, and Sequence Diagrams
- **License**: Free for students and non-commercial use
- **Features**: 
  - Professional UML notation support
  - Auto-layout capabilities
  - Database modeling tools
  - Built-in project management features
  - Excellent sequence diagram generation

### Backup Tool: Diagrams.net (draw.io)
- **Best for**: Component Diagrams and system architecture visualization
- **License**: Free and open-source
- **Features**:
  - Web-based and desktop versions
  - Great for system architecture
  - Easy collaboration
  - Export to multiple formats

### Mermaid Diagrams
Many diagrams in this collection use Mermaid syntax for version control-friendly diagram-as-code approach. These render directly in GitHub and many markdown viewers.

## 📁 Directory Structure

```
Smart_Plug_AI_Diagrams/
├── 1_Use_Case_Diagrams/       # System use cases and actor interactions
├── 2_Activity_Diagrams/        # Process flows and workflows
├── 3_Class_ER_Diagrams/        # Class diagrams and database models
├── 4_Sequence_Diagrams/        # Interaction sequences
├── 5_Component_Diagrams/       # System architecture components
├── 6_Gantt_Charts/             # Project timelines and schedules
├── 7_Exports/                  # Exported PDF and PPTX files
└── source_d24e744d.txt         # Original source documentation
```

## 📖 Source Documentation

The original source text from which these diagrams were generated is stored at:
`Smart_Plug_AI_Diagrams/diagram_specifications.txt`

This file contains:
- Complete diagram specifications
- Tool recommendations and rationale
- Suggested file naming conventions
- Detailed diagram content in Mermaid syntax
- Security architecture notes
- Implementation timelines

## 📤 Exporting Diagrams

### For Visual Paradigm Files (.vpp)

1. Open the diagram in Visual Paradigm
2. Go to **File → Export → Image**
3. Choose format: PDF, PNG, or SVG

### For Diagrams.net Files (.drawio)

1. Open the file in diagrams.net
2. Go to **File → Export as**
3. Choose: PDF, PNG, SVG, or VSDX
4. For high-quality prints, use PDF or SVG

### For Mermaid Diagrams

Mermaid diagrams are embedded in markdown files and render automatically in:
- GitHub
- GitLab
- Many markdown editors (VS Code with Mermaid extension)
- Documentation sites (MkDocs, Docusaurus, etc.)

To export Mermaid diagrams:
1. Use the Mermaid Live Editor (https://mermaid.live)
2. Copy diagram code from markdown files
3. Export to PNG or SVG

## 🎨 Export Collection

Pre-generated exports are available in the `7_Exports/` directory:
- `All_Diagrams.pdf` - Complete diagram collection in PDF format

**Note**: Large binary export (PDF) is a placeholder in this repository and needs to be generated locally using the tools above.

## 🔒 Security Features Documented

These diagrams illustrate the comprehensive security architecture:

- **Hardware Security**: ESP32-S3 with secure boot, ATECC608A secure element, tamper detection
- **Communication Security**: TLS 1.3, MQTT over TLS with mTLS, signed commands
- **Device Security**: Challenge-response authentication, device attestation, secure OTA updates
- **Backend Security**: 2FA/MFA, RBAC, field-level encryption, audit logging
- **Compliance**: SOC2, POPIA/GDPR readiness, SABS/ICASA compliance

## 📚 Related Documentation

For complete project documentation, see:
- [Main README](../README.md)
- [Architecture Documentation](../docs/ARCHITECTURE.md)
- [Security Documentation](../docs/SECURITY.md)
- [API Documentation](../docs/API.md)
- [Roadmap](../docs/ROADMAP.md)

## 🤝 Contributing

When adding new diagrams:
1. Follow the naming convention: `N.M_Diagram_Name.ext`
2. Update the respective folder's README.md
3. Include diagram source files (.vpp, .drawio)
4. Export to standard formats (PDF, PNG)
5. Add Mermaid versions where applicable for version control

## 📝 License

These diagrams are part of the Smart Plug AI project and are licensed under the MIT License - see the [LICENSE](../LICENSE) file for details.
