![image](https://github.com/user-attachments/assets/dcf0e7a5-989d-4bb1-aa3d-532305710288)

# A custom Rendering Engine built on top of OpenGL
My implementation of a graphics engine to test the fundamentals of graphics programming and code c++ knowledge

## Main Features

- **Lighting Models**  
  - Implemented Phong Lighing model to simluate Lighting in a 3-D environmend
  - Added multiple typed of lights such as directional light, point light and spot light  

- **Omnidirectional Shadow Mapping**  
  - Single-pass cubemap generation via a layered geometry shader  
  - Efficient depth rendering for six faces in one draw call  
  - Adjustable shadow map resolution

- **Skybox**
  - Implemented a Skybox by rendering a cube around the camera and disabling the depth buffer so that everything else is drawn on top of the skybox


demo video : https://www.linkedin.com/feed/update/urn:li:activity:7167542999357280256/
