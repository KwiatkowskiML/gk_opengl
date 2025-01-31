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

#include <memory>
#include "CameraManager.h"
#include "Cameras/Camera.h"
#include "Cameras/CameraCircular.h"
#include "Cameras/CameraConstant.h"
#include "Cameras/CameraFPS.h"
#include "Constants.h"
#include "LightSource.h"
#include "Shader.h"
#include "WindowManager.h"

class Renderer
{
    public:
    //-----------------------------------------------------------------------------------
    // Constructor and Destructor
    //-----------------------------------------------------------------------------------
    Renderer(unsigned int width, unsigned int height);
    ~Renderer();

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
    CameraManager cameraManager;
    LightSource lightSource;
    unsigned int windowWidth, windowHeight;
    unsigned int VAO, VBO;

    //-----------------------------------------------------------------------------------
    // Data Setup
    //-----------------------------------------------------------------------------------
    void setupVertexData();

    //-----------------------------------------------------------------------------------
    // Input processing
    //-----------------------------------------------------------------------------------
    void processInput(float deltaTime);

    //-----------------------------------------------------------------------------------
    // Light source setup
    //-----------------------------------------------------------------------------------
    void setupLightSource();
};

#endif  // RENDERER_H
