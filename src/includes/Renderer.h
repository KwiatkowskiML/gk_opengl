//
// Created by michal on 1/23/25.
//

#ifndef RENDERER_H
#define RENDERER_H

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
// clang-format on

#include "Cameras/Camera.h"
#include "Cameras/CameraCircular.h"
#include "Cameras/CameraConstant.h"
#include "Cameras/CameraFPS.h"
#include "Constants.h"
#include "LightSource.h"
#include "Shader.h"
#include "WindowManager.h"

#include <memory>

// Initial camera positions for different camera types
constexpr glm::vec3 INITIAL_FPS_CAMERA_POSITION(0.0f, 0.0f, 3.0f);
constexpr glm::vec3 INITIAL_CONSTANT_CAMERA_POSITION(0.0f, 5.0f, 10.0f);
constexpr glm::vec3 INITIAL_CIRCULAR_CAMERA_POSITION(0.0f, 5.0f, 10.0f);
constexpr glm::vec3 CAMERA_TARGET_POSITION(0.0f, 0.0f, 0.0f);
constexpr glm::vec3 LIGHT_POS(2.0f, 2.0f, 2.0f);

class Renderer
{
    public:
    //-----------------------------------------------------------------------------------
    // Constructor and Destructor
    //-----------------------------------------------------------------------------------
    Renderer(unsigned int width, unsigned int height);
    ~Renderer();

    //-----------------------------------------------------------------------------------
    // GLFW callback functions
    //-----------------------------------------------------------------------------------
    // static void framebuffer_size_callback(GLFWwindow* /*window*/, const int width, const int height);
    // static void mouse_callback(GLFWwindow* window, const double xpos, const double ypos);

    //-----------------------------------------------------------------------------------
    // Camera management
    //-----------------------------------------------------------------------------------
    void setCamera(Camera* newCamera);
    void updateCamera() const;

    //-----------------------------------------------------------------------------------
    // Rendering and execution
    //-----------------------------------------------------------------------------------
    void run();
    void render(Shader& shader) const;

    //-----------------------------------------------------------------------------------
    // Getters
    //-----------------------------------------------------------------------------------
    GLFWwindow* getWindow() const;
    Camera* getCamera() const;

    private:
    //-----------------------------------------------------------------------------------
    // Private members
    //-----------------------------------------------------------------------------------
    std::unique_ptr<WindowManager> windowManager;
    // GLFWwindow* window;
    Camera* camera;
    LightSource lightSource;
    unsigned int SCR_WIDTH, SCR_HEIGHT;
    unsigned int VAO, VBO;
    CameraType cameraType = CameraType::FPS;

    //-----------------------------------------------------------------------------------
    // OpenGL and GLFW initialization functions
    //-----------------------------------------------------------------------------------

    // GLFW setup: Initializes GLFW and sets OpenGL version
    // void initializeGLFW();
    //
    // // Create a window with the specified dimensions and check for errors
    // void createWindow();
    //
    // // Set up GLFW callbacks for framebuffer size and mouse movement
    void setupCallbacks() const;
    //
    // // Initialize GLAD to load OpenGL function pointers
    // void initializeGLAD();

    //-----------------------------------------------------------------------------------
    // Data Setup
    //-----------------------------------------------------------------------------------
    void setupVertexData();

    //-----------------------------------------------------------------------------------
    // Input processing
    //-----------------------------------------------------------------------------------
    void processInput();

    //-----------------------------------------------------------------------------------
    // Light source setup
    //-----------------------------------------------------------------------------------
    void setupLightSource();
};

#endif  // RENDERER_H
