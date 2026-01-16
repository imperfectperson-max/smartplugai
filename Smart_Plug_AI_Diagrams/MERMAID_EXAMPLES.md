# Mermaid Diagram Examples

This document contains Mermaid code examples for various Smart Plug AI diagrams. These examples are provided for **reference and learning purposes only** and will be converted to formal UML diagrams using Visual Paradigm in future iterations.

## About These Examples

The Mermaid diagrams included in the original specification document serve as:
- **Conceptual references** for understanding system workflows
- **Quick prototyping tools** for visualizing interactions
- **Documentation supplements** that can be rendered in GitHub and markdown viewers
- **Templates** for converting to formal UML diagrams

## Important Notes

⚠️ **These are examples only** - The actual diagram files (.vpp and .drawio files) will be created separately using professional diagramming tools as specified in each subdirectory's README.

✅ **Not included in this PR** - The full Mermaid code from the specification document is not imported as active diagram files to keep this PR lightweight and focused on folder structure.

## Where to Find Mermaid Examples

The original specification document contains comprehensive Mermaid examples for:

1. **Use Case Diagrams** (5 diagrams)
   - System Overview Use Case
   - Authentication & Security Use Cases  
   - Device Management Use Cases
   - Data Analytics Use Cases
   - Security Operations Use Cases

2. **Activity Diagrams** (6 diagrams)
   - Secure Device Pairing Activity
   - Secure Command Execution Activity
   - Tamper Detection & Response Activity
   - Secure OTA Firmware Update Activity
   - Real-time Telemetry Data Flow Activity
   - User Authentication with 2FA Activity

3. **Class & ER Diagrams**
   - Entity-Relationship Diagram with detailed entities
   - Backend Service Layer Class Diagram
   - Security Components Class Diagram
   - Service Layer Class Diagram

4. **Sequence Diagrams** (6 diagrams)
   - Secure Device Pairing Sequence
   - Secure Command Execution Sequence
   - Tamper Detection & Response Sequence
   - Secure OTA Firmware Update Sequence
   - Real-time Telemetry Data Flow Sequence
   - User Authentication with 2FA Sequence

5. **Gantt Charts**
   - Detailed Phase 1 Gantt Chart (Weeks 1-12)
   - Security Milestone Timeline
   - Risk Management Timeline
   - Deliverable Timeline

## How to Use Mermaid Code

### Rendering in GitHub
GitHub automatically renders Mermaid diagrams in markdown files. Simply wrap the code in a mermaid code block:

\`\`\`mermaid
graph TD
    A[Start] --> B[Process]
    B --> C[End]
\`\`\`

### Using Mermaid Live Editor
Visit https://mermaid.live/ to:
- Paste and edit Mermaid code
- See live preview
- Export to PNG/SVG
- Share diagrams via URL

### Using Mermaid CLI
Install Mermaid CLI for command-line rendering:

```bash
npm install -g @mermaid-js/mermaid-cli
mmdc -i input.mmd -o output.png
```

### Using Pandoc with Mermaid Filter
For converting markdown with Mermaid to PDF:

```bash
pandoc --filter mermaid-filter input.md -o output.pdf
```

## Integration with Professional Tools

While Mermaid is excellent for quick visualization, professional UML tools offer:

### Visual Paradigm Advantages
- **Formal UML compliance** with UML 2.5 notation
- **Bidirectional code engineering** (generate code from diagrams, diagrams from code)
- **Advanced modeling** features (state machines, deployment diagrams)
- **Team collaboration** with model versioning
- **Database engineering** (generate SQL DDL from ER diagrams)
- **Documentation generation** (auto-generate specification documents)

### Diagrams.net Advantages
- **Cloud integration** (Google Drive, OneDrive, GitHub)
- **Rich icon libraries** (AWS, Azure, network devices)
- **No installation** required (web-based)
- **Git-friendly** XML format
- **Free and open source**

## Workflow Recommendation

For contributors creating diagrams:

1. **Prototype with Mermaid**: Use Mermaid for initial brainstorming and quick feedback
2. **Formalize with Visual Paradigm**: Convert approved Mermaid diagrams to UML in Visual Paradigm
3. **Component Diagrams with Diagrams.net**: Use draw.io for system architecture and deployment diagrams
4. **Export for Documentation**: Generate PNG/SVG/PDF for inclusion in documentation
5. **Version Control**: Store source files (.vpp, .drawio) in Git, export rendered images for README files

## Example: Converting Mermaid to Visual Paradigm

1. Review the Mermaid diagram structure
2. Open Visual Paradigm and create a new diagram of the appropriate type
3. Add actors/classes/components matching the Mermaid nodes
4. Add relationships matching the Mermaid edges
5. Apply UML stereotypes and formatting
6. Add notes and constraints
7. Validate against UML standards
8. Export to desired formats

## Resources

- **Mermaid Documentation**: https://mermaid.js.org/
- **Mermaid Live Editor**: https://mermaid.live/
- **Visual Paradigm Community**: https://www.visual-paradigm.com/download/community.jsp
- **Diagrams.net**: https://app.diagrams.net/
- **UML 2.5 Specification**: https://www.omg.org/spec/UML/

## Contributing

When adding Mermaid examples to documentation:
- Follow Mermaid best practices and syntax
- Include comments explaining complex sections
- Test rendering in GitHub before committing
- Provide both Mermaid code and rendered image for accessibility
- Link to the corresponding formal UML diagram when available

## Next Steps

In follow-up PRs, contributors will:
1. Create formal .vpp files in Visual Paradigm based on specifications
2. Create .drawio files for component diagrams
3. Export diagrams to the `7_Exports/` directory
4. Update README files with diagram completion status
5. Reference specific Mermaid examples in formal diagram documentation
