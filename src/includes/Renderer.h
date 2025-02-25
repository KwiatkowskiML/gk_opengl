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
#include "ImguiMenu.h"
#include "InternalModel.h"
#include "Lights/LightSource.h"
#include "Perspectives/ProjectionManager.h"
#include "Shader.h"
#include "Skybox/SkyBox.h"
#include "WindowManager.h"
#include "includes/Models/Flashlight.h"
#include "includes/Models/Model.h"
#include "includes/Skybox/SkyBoxManager.h"

class Renderer
{
    public:
    std::unique_ptr<ImguiMenu> imguiMenu;

    //-----------------------------------------------------------------------------------
    // Constructor and Destructor
    //-----------------------------------------------------------------------------------
    Renderer(unsigned int width, unsigned int height);
    ~Renderer();

    //-----------------------------------------------------------------------------------
    // Rendering and execution
    //-----------------------------------------------------------------------------------
    void run();
    void render(Shader& gShader, Shader& lightningPassShader, Shader& skyboxShader, float deltaTime);

    //-----------------------------------------------------------------------------------
    // Getters
    //-----------------------------------------------------------------------------------
    GLFWwindow* getWindow() const;
    Camera* getCamera() const;

    private:
    //-----------------------------------------------------------------------------------
    // Private members
    //-----------------------------------------------------------------------------------
    unsigned int windowWidth, windowHeight;

    // Utility managers
    std::unique_ptr<WindowManager> windowManager;
    CameraManager cameraManager;
    ProjectionManager projectionManager;

    // Models
    std::vector<InternalModel> models;
    NewModel backpackModel;
    NewModel italyMap;
    SkyBoxManager skyboxManager;
    std::unique_ptr<Flashlight> flashlightModel;

    // Lights
    std::vector<LightSource> pointLights;
    LightSource lightSource;

    // G-buffer
    unsigned int gBuffer;
    unsigned int gPosition;
    unsigned int gNormal;
    unsigned int gColorSpec;

    // Lightning pass
    unsigned int quadVAO = 0;
    unsigned int quadVBO;

    //-----------------------------------------------------------------------------------
    // G-buffer
    //-----------------------------------------------------------------------------------
    void setupGBuffer();
    void renderQuad();
    void fillGBuffer(Shader& gShader, float deltaTime);
    void setupLightningPass(Shader& lightningPassShader);
    void dispalySkybox(const Shader& skyboxShader, const SkyBox& skybox);

    //-----------------------------------------------------------------------------------
    // Input processing
    //-----------------------------------------------------------------------------------
    void processInput(float deltaTime) const;

    //-----------------------------------------------------------------------------------
    // Light source setup
    //-----------------------------------------------------------------------------------
    void setupLightSource();
    void setupPointLights();

    //-----------------------------------------------------------------------------------
    // Add model
    //-----------------------------------------------------------------------------------
    void addCube(const glm::vec3& position, const glm::vec3& color);
    void addSphere(const glm::vec3& position, const glm::vec3& color, float radius);
};

#endif  // RENDERER_H
