![image](https://github.com/user-attachments/assets/dcf0e7a5-989d-4bb1-aa3d-532305710288)

# A custom Rendering Engine built on top of OpenGL
My implementation of a graphics engine to test the fundamentals of graphics programming and code c++ knowledge

## Core Features

- **Advanced Lighting Models**  
  - **Phong Shading**: Full per‐fragment lighting calculations with ambient, diffuse, and specular components.  
  - **Multi‐Light Support**:  
    - **Directional Lights** for simulating distant light sources (e.g., sun).  
    - **Point Lights** with linear and quadratic attenuation.  
    - **Spot Lights** with inner/outer cone angle falloff and configurable cutoff.  

- **Unidirectional Shadow Mapping**  
  - Standard depth‐map generation from a single light’s perspective.  
  - PCF (Percentage‐Closer Filtering) implemented in the fragment shader to smooth shadow edges.  
  - Configurable shadow bias to reduce “shadow acne.”

- **Omnidirectional Shadow Mapping**  
  - **Layered Cube‐Map Rendering**: One geometry‐shader pass writes depth to all six faces of a GL_TEXTURE_CUBE_MAP.  
  - **Depth Comparison** in the fragment shader using cube-map lookups for point‐light shadows.  
  - Dynamic resolution control for shadow maps (e.g., 512×512 to 2048×2048).

- **Skybox Renderer**  
  - Rendered by drawing a cube with inverted faces, centered on the camera.  
  - Depth writes disabled to ensure the skybox always renders behind all scene geometry.

    
demo video : https://www.linkedin.com/feed/update/urn:li:activity:7167542999357280256/
