# 3D Renderer

This project demonstrates the usage of a programmable rendering pipeline utilizing OpenGL for rendering a 3D scene. The
scene includes basic geometric objects such as spheres and cubes, as well as
complex models that can be loaded into the application. The project includes a set of camera views, a dynamic moving
object, and several light sources interacting with the environment, showcasing various advanced rendering techniques.

## Features

- **Camera System:**
    - Three distinct, switchable camera perspectives:
        1. **Stationary camera:** Observes the scene from a fixed position.
        2. **Circular camera:** A stationary camera that moves in circles.
        3. **First person camera** 

- **Lighting:**
    - **Spotlight on moving object:** Simulates flashlight's light effects with the ability to manually change
      the spotlight's direction.
    - **Multiple fixed point light source**
    - **Light fading:** A smooth transition of light intensity as the distance from the light source increases.
    - **Day/Night modes:** changing the skybox according to selected mode.
    - **Phong shading model:** Smooth shading using normal vector interpolation to simulate realistic lighting.

- **Rendering Effects:**
    - **Fog Effect**
    - **Deferred shading**
    - **Projection Switching:** The ability to switch between perspective and orthogonal projections.

## Requirements

Before running the project, ensure that the following tools and libraries are available:

- **CMake** (version 3.28 or later)
- **OpenGL**.
- **GLFW**
- **GLAD**
- **GLM** 
- **Assimp**
- **STB** 
- **Imgui**

These dependencies are automatically handled and fetched by CMake, so there is no need for manual installation.


## Installation

### Step 1: Clone the repository

```bash
git clone https://github.com/KwiatkowskiML/gk_opengl.git
cd gk_opengl
```

### Step 2: Build the Project

This project uses CMake for building and fetching dependencies.

#### On Linux:
```bash
mkdir build
cd build
cmake ..
make
```

#### On Windows:
Build the project from within Visual Studio with CMake.


### Step 3: Run the Project
After building the project, run the generated executable from the build directory:

```bash
./GK_P4   # On Linux/macOS
GK_P4.exe # On Windows
```

## Usage

- **Movement**: Use `WASD` to move around and the mouse for look control.
- **Menu**: Press `Tab` to toggle the on-screen menu for additional options.

