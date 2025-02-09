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
#include <vector>
#include "CameraManager.h"
#include "Cameras/Camera.h"
#include "Cameras/CameraCircular.h"
#include "Cameras/CameraConstant.h"
#include "Cameras/CameraFPS.h"
#include "Constants.h"
#include "InternalModel.h"
#include "LightSource.h"
#include "Perspectives/ProjectionManager.h"
#include "Shader.h"
#include "WindowManager.h"
#include "includes/Models/Flashlight.h"
#include "includes/Models/Model.h"

class Renderer
{
    public:
    Flashlight* flashlightModel;

    //-----------------------------------------------------------------------------------
    // Constructor and Destructor
    //-----------------------------------------------------------------------------------
    Renderer(unsigned int width, unsigned int height);
    ~Renderer();

    //-----------------------------------------------------------------------------------
    // Rendering and execution
    //-----------------------------------------------------------------------------------
    void run();
    void render(Shader& lightningShader, Shader& modelShader, Shader& gShader) const;

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
    ProjectionManager projectionManager;
    LightSource lightSource;
    unsigned int windowWidth, windowHeight;
    std::vector<InternalModel> models;
    NewModel backpackModel;

    // G-buffer
    unsigned int gBuffer;
    unsigned int gPosition;
    unsigned int gNormal;
    unsigned int gColorSpec;

    //-----------------------------------------------------------------------------------
    // G-buffer setup
    //-----------------------------------------------------------------------------------
    void setupGBuffer();

    //-----------------------------------------------------------------------------------
    // Input processing
    //-----------------------------------------------------------------------------------
    void processInput(float deltaTime);

    //-----------------------------------------------------------------------------------
    // Light source setup
    //-----------------------------------------------------------------------------------
    void setupLightSource();

    //-----------------------------------------------------------------------------------
    // Add model
    //-----------------------------------------------------------------------------------
    void addCube(const glm::vec3& position, const glm::vec3& color);
    void addSphere(const glm::vec3& position, const glm::vec3& color, float radius);
};

#endif  // RENDERER_H
