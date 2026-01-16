# Exports

This folder contains exported diagram collections in presentation-ready formats.

## Overview

This directory holds:
- **PDF exports**: Complete diagram collection for printing and sharing
- **PowerPoint presentations**: Slides with key diagrams for presentations
- **High-resolution images**: Individual diagrams in PNG/SVG format

## Files in this Folder

### Main Export Files

- **All_Diagrams.pdf** - Complete collection of all diagrams in a single PDF document
- **Presentation_Slides.pptx** - PowerPoint presentation with key diagrams and annotations

**Note**: These are placeholder files. The actual exports need to be generated locally using the tools described below.

## Generating Exports

### Creating All_Diagrams.pdf

#### From Visual Paradigm
1. Open each .vpp file in Visual Paradigm
2. Go to **File → Export → Image**
3. Select PDF format
4. Choose "Export All Diagrams" option
5. Combine PDFs using:
   - Adobe Acrobat (paid)
   - PDFtk (free, command-line)
   - Online PDF merge tools

#### From Diagrams.net
1. Open each .drawio file
2. Go to **File → Export as → PDF**
3. Select "All Pages" option
4. Combine all PDFs into one document

### Creating Presentation_Slides.pptx

#### Method 1: Visual Paradigm Direct Export
1. Select diagrams to include in presentation
2. Go to **File → Export → MS PowerPoint**
3. Choose layout and format options
4. Add titles and annotations in PowerPoint

#### Method 2: Manual Creation
1. Export each diagram as high-res PNG (300 DPI)
2. Create new PowerPoint presentation
3. Insert diagrams one per slide
4. Add titles, descriptions, and notes
5. Apply consistent theme and branding

### Recommended Export Settings

#### For PDF
- **Resolution**: 300 DPI minimum
- **Color Space**: RGB for digital, CMYK for print
- **Compression**: Medium (balance quality/size)
- **Fonts**: Embed all fonts
- **Page Size**: A4 or Letter

#### For PowerPoint
- **Slide Size**: Widescreen (16:9)
- **Image Format**: PNG with transparency
- **Resolution**: 1920x1080 minimum
- **Fonts**: Use standard fonts or embed
- **File Size**: Compress images if needed

#### For Individual Images
- **Format**: PNG for raster, SVG for vector
- **Resolution**: 300 DPI for print, 96 DPI for web
- **Background**: Transparent for flexibility
- **Naming**: Descriptive names (e.g., `use_case_system_overview.png`)

## Export Checklist

Before finalizing exports:
- [ ] All diagrams are up to date
- [ ] Consistent styling across diagrams
- [ ] Legends and labels are clear
- [ ] High enough resolution for intended use
- [ ] File sizes are reasonable (<50MB for PDF, <20MB for PPTX)
- [ ] All fonts are embedded or converted to shapes
- [ ] Security-sensitive information is redacted if needed
- [ ] Version numbers or dates are included
- [ ] Copyright and licensing information added

## Distribution

### Internal Use
- Share via company file server or SharePoint
- Include in project documentation
- Distribute to development team

### External Presentations
- Remove internal notes and draft labels
- Add company branding and logos
- Include disclaimer slides if needed
- Test on presentation computer beforehand

### Academic Submission
- Follow university formatting guidelines
- Include required cover pages
- Add bibliography and references
- Check file size limits for submission portals

## Folder Structure for Exports

```
7_Exports/
├── All_Diagrams.pdf              # Complete collection
├── Presentation_Slides.pptx       # Key diagrams presentation
├── individual/                    # Individual diagram exports
│   ├── 01_use_case_overview.png
│   ├── 02_activity_pairing.png
│   ├── 03_er_diagram.png
│   ├── 04_sequence_pairing.png
│   ├── 05_architecture.png
│   └── 06_gantt_phase1.png
└── high_res/                      # High-resolution versions
    ├── use_case_overview_300dpi.png
    └── ...
```

## Version Control

When updating diagrams:
1. Update source files (.vpp, .drawio, .md)
2. Re-export affected diagrams
3. Update version number in footer
4. Document changes in commit message
5. Regenerate All_Diagrams.pdf
6. Archive previous versions if significant changes

## Large Binary Files

**Important**: PDF and PPTX files can be quite large (10-50MB+). Consider:
- Using Git LFS (Large File Storage) for version control
- Hosting exports on cloud storage with links in README
- Excluding from repository and generating on-demand
- Compressing images within documents

For this repository, large binary exports are **placeholders only** and should be generated locally when needed.

## Tools for Batch Export

### PDFtk (PDF Toolkit)
```bash
# Combine multiple PDFs
pdftk diagram1.pdf diagram2.pdf diagram3.pdf cat output All_Diagrams.pdf
```

### ImageMagick
```bash
# Convert and resize images
convert input.png -resize 1920x1080 output.png
```

### Python Script Example
```python
# Combine diagrams into PDF
from PyPDF2 import PdfMerger

merger = PdfMerger()
for pdf in sorted(glob.glob("*.pdf")):
    merger.append(pdf)
merger.write("All_Diagrams.pdf")
merger.close()
```

## References

For source diagrams and editing, see the respective folders:
- `1_Use_Case_Diagrams/`
- `2_Activity_Diagrams/`
- `3_Class_ER_Diagrams/`
- `4_Sequence_Diagrams/`
- `5_Component_Diagrams/`
- `6_Gantt_Charts/`

For export instructions specific to each tool, see the README files in those folders.
