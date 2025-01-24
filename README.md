---

# 3D Graphics Application with Win32 and GDI

This project is a C++ Win32 application that demonstrates 3D graphical functionality using GDI (Graphics Device Interface) calls. The application features an automated, self-running 3D model showcasing various transformations, shading techniques, and lighting effects. It was developed as part of a university course project to explore the fundamentals of 3D graphics rendering, transformations, and lighting models.

## Features

### Basic Functionality
- **3D Wireframe Rendering**: Displays a 3D wireframe model loaded from an `.MD2` file.
- **Model Transformations**:
  - Rotation around X, Y, and Z axes.
  - Scaling and translation in world space.

### Additional Features (Grade Thresholds)
#### ≥50% Features:
- **Back Face Culling**: Implements an algorithm to remove polygons that should not be visible.
- **Polygon Sorting**: Uses the Painter’s Algorithm for proper rendering order.
- **Flat Shading**: Implements flat shading using Windows GDI calls.
- **Ambient Lighting**: Adds a global illumination effect to simulate ambient light.
- **Directional Lighting**: Implements diffuse lighting with a single directional light source.

#### ≥60% Features:
- **Custom Rasterization**: Implements flat shading with custom rasterization code.
- **Point Lighting**: Adds diffuse lighting using a point light source.

#### ≥70% Features:
- **Gouraud Shading**: Implements vertex-based lighting and color interpolation (implementation attempted but commented out due to runtime errors).

#### ≥80% and ≥90% Features:
- Spotlights and texturing (basic or perspective-correct) were not implemented in this project.

### Debugging and Fixes
- Extensive debugging was carried out for issues like memory leaks, lighting inaccuracies, and incorrect model transformations.
- Added custom rasterization for flat and Gouraud shading.
- Fixed normal vector calculations and vertex color assignments.

## Project Structure

The project consists of the following components:
1. **Vertex and Polygon Management**: Classes to handle vertex data, including 3D coordinates, normals, and colors.
2. **Camera System**: Allows for view matrix transformations.
3. **Lighting Models**:
   - Ambient lighting.
   - Directional and point light sources.
4. **Rendering Pipeline**:
   - Transformation matrices for perspective, screen, and model-space transformations.
   - Custom rasterization for flat shading.
5. **Error Handling**: All reported issues were fixed, except the commented-out Gouraud shading.

## How to Run
1. Clone the repository and extract the files.
2. Navigate to the `Standalone` folder.
3. Run the executable file (`*.exe`) on a Windows system (tested within University labs).

## Implementation Log
A detailed log of the implementation process, including debugging steps and challenges faced, is provided in the `WorkDiary.docx` file.

## Feature Checklist
The completed feature checklist is available in `FeatureChecklist.doc`. It summarizes the functionality implemented and any known limitations.

## Future Work
- Implementing specular lighting for directional and point light sources.
- Adding spotlights and texturing with and without perspective correction.
- Fixing the commented-out Gouraud shading functionality.

## Acknowledgments
This project was developed as part of the Graphics 1 (5CC509) module under the guidance of university staff.

---
