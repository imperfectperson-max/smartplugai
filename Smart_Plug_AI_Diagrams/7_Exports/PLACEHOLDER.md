# Export Placeholder

This directory will contain exported diagram files once the diagrams are created.

## Recommended Export Formats

### For Documentation
- **PDF**: Best for formal documentation and archival
  - Resolution: 300 DPI
  - Color space: RGB for digital, CMYK for print
  - Include: Table of contents, page numbers

### For Presentations
- **PowerPoint (PPTX)**: Ideal for stakeholder presentations
  - One diagram per slide with speaker notes
  - Simplified versions of complex diagrams
  - Consistent theme and branding

### For Web and Digital Documentation
- **PNG**: For embedding in README files and wikis
  - Resolution: 150-300 DPI
  - Transparent background option
  - Optimized file size

- **SVG**: For scalable web graphics
  - Vector format, scales without quality loss
  - Editable in web browsers
  - Smaller file size than raster formats

### For Print Materials
- **PDF**: High-resolution for printing
  - 300 DPI minimum
  - CMYK color space
  - Embedded fonts

### For Collaboration and Editing
- **Native Formats**: Keep source files accessible
  - `.vpp` files for Visual Paradigm
  - `.drawio` files for Diagrams.net
  - Version control with Git

## Exporting Best Practices

1. **Naming Convention**: Use descriptive names with version and date
   - Example: `Smart_Plug_AI_Architecture_v1.0_2025-04-25.pdf`

2. **Quality Settings**: Always export at highest quality
   - Minimum 300 DPI for print
   - Minimum 150 DPI for digital/web

3. **File Organization**: Group exports by type and purpose
   - `/exports/pdf/` - PDF versions
   - `/exports/images/` - PNG/SVG versions  
   - `/exports/presentations/` - PowerPoint files

4. **Version Control**: Keep track of changes
   - Include version number in filename
   - Maintain changelog for major updates
   - Archive old versions for reference

5. **Accessibility**: Ensure exports are accessible
   - Include alt text in images
   - Use high contrast colors
   - Readable font sizes (minimum 10pt)
   - Proper heading structure in PDFs

## Coming Soon

Once diagrams are created in Visual Paradigm and Diagrams.net, this directory will be populated with:
- `All_Diagrams.pdf` - Complete compilation of all diagrams
- `Presentation_Slides.pptx` - Presentation deck for stakeholders
- Individual diagram exports in PNG/SVG formats
- High-resolution versions for printing and archival

## Tools for Exporting

### From Visual Paradigm
- File → Export → PDF (for complete document)
- File → Export → Image (for individual diagrams)
- File → Export → MS Office (for PowerPoint)

### From Diagrams.net
- File → Export as → PDF/PNG/SVG/JPEG
- File → Print → Save as PDF (for multi-page exports)

### Batch Export Scripts
For automated export of multiple diagrams, scripts can be created using:
- Visual Paradigm automation API
- Diagrams.net command-line tools
- ImageMagick for format conversion and optimization
