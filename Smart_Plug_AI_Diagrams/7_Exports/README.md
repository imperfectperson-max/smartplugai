# Exports

This folder contains exported versions of all diagrams for presentations, documentation, and archival.

## Expected Files

The following export files will be added to this directory:

- **All_Diagrams.pdf** - Complete compilation of all diagrams in a single PDF document
- **Presentation_Slides.pptx** - PowerPoint presentation with key diagrams for stakeholder presentations

## Additional Export Formats

Contributors may add exports in various formats:
- **PNG/JPG**: High-resolution raster images for web and documentation
- **SVG**: Scalable vector graphics for print and scalable displays
- **PDF**: Individual diagram PDFs for formal documentation
- **DOCX**: Word documents with embedded diagrams and descriptions

## Export Guidelines

### PDF Exports (All_Diagrams.pdf)

Structure:
1. Cover page with project title and date
2. Table of contents
3. Section 1: Use Case Diagrams
4. Section 2: Activity Diagrams
5. Section 3: Class & ER Diagrams
6. Section 4: Sequence Diagrams
7. Section 5: Component Diagrams
8. Section 6: Gantt Charts
9. Appendix: Tool recommendations and methodology

Settings:
- Page size: A4 or Letter
- Orientation: Landscape for wide diagrams, Portrait for text
- Resolution: 300 DPI minimum
- Color space: RGB for digital, CMYK for print
- Include page numbers and section headers

### PowerPoint Presentation (Presentation_Slides.pptx)

Recommended structure:
1. Title slide: Smart Plug AI Architecture Overview
2. Agenda: Diagram categories overview
3. System Use Case Overview (1 slide)
4. Key Activity Flows (2-3 slides): Pairing, Command, Tamper
5. Data Model Overview (1 slide): ER diagram
6. Security Architecture (2 slides): Class diagrams for security components
7. Critical Sequences (2-3 slides): Control, Tamper, OTA
8. System Architecture (1 slide): Component diagram
9. Project Timeline (1 slide): Gantt chart overview
10. Security Highlights (1 slide): Key security features
11. Next Steps (1 slide): Future work and roadmap

Presentation tips:
- Use consistent theme matching project branding
- Add speaker notes for each slide
- Include simplified versions of complex diagrams
- Highlight security features in red/orange
- Use animations sparingly for flow diagrams

## Exporting from Visual Paradigm

### To PDF:
1. File → Export → PDF
2. Select diagrams to include
3. Choose layout and page settings
4. Enable "Include diagram description"
5. Export

### To Images (PNG/SVG):
1. Right-click diagram → Export Diagram as Image
2. Choose format (PNG, SVG, JPG)
3. Set resolution (300 DPI for print, 150 DPI for web)
4. Export

### To PowerPoint:
1. File → Export → MS Office
2. Select PowerPoint format
3. Choose diagrams and layout
4. Enable "One diagram per slide"
5. Export

## Exporting from Diagrams.net

### To PDF:
1. File → Export as → PDF
2. Choose page range (All pages or Current page)
3. Set quality and border options
4. Download

### To Images:
1. File → Export as → PNG/SVG/JPEG
2. Set zoom level (100% for actual size, higher for HD)
3. Enable "Transparent background" for PNG if needed
4. Download

### To PowerPoint:
1. Export each diagram as PNG/SVG
2. Import images into PowerPoint template
3. Add titles and annotations
4. Or use File → Export as → VSDX, then import to PowerPoint

## Version Control

Export files should include version information:
- Filename format: `All_Diagrams_v1.0_2025-04-25.pdf`
- Changelog: Document what changed between versions
- Archival: Keep previous versions for reference

## Usage

Exported diagrams are used for:
- **Technical Documentation**: Included in system architecture docs
- **Stakeholder Presentations**: Shown to investors, partners, customers
- **Development Reference**: Printed for team collaboration
- **Compliance Submissions**: Provided to SABS, ICASA for certifications
- **Academic Purposes**: University project submissions and evaluations
- **Training Materials**: Onboarding new team members

## Quality Checklist

Before exporting, verify:
- [ ] All diagrams are up-to-date with latest system design
- [ ] No sensitive information (API keys, passwords) in diagrams
- [ ] Consistent notation and styling across all diagrams
- [ ] Clear labels and legends
- [ ] Readable text sizes (minimum 10pt font)
- [ ] High-resolution images (300 DPI for print)
- [ ] Proper attribution and copyright notices
- [ ] Version number and date on each diagram
