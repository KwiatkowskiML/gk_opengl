//
// Created by michal on 1/30/25.
//

#include "includes/Renderer.h"

#include <includes/SkyBox.h>

#include <filesystem>

//-----------------------------------------------------------------------------------
// Constructor and destructor
//-----------------------------------------------------------------------------------
Renderer::~Renderer()
{
    for (const auto &model : models) {
        glDeleteVertexArrays(1, &model.VAO);
        glDeleteBuffers(1, &model.VBO);
    }
    glfwTerminate();
    delete flashlightModel;
}

Renderer::Renderer(unsigned int width, unsigned int height)
    : windowManager(std::make_unique<WindowManager>(width, height)),
      projectionManager(width, height, cameraManager.getZoom()),
      windowWidth(width),
      windowHeight(height),
      backpackModel(std::filesystem::path(BACKPACK_MODEL_PATH))
{
    setupLightSource();
    glfwSetWindowUserPointer(windowManager->getWindow(), this);
    flashlightModel =
        new Flashlight(std::filesystem::path(FLASHLIGHT_MODEL_PATH), aiProcess_FlipUVs | aiProcess_Triangulate);

    // Setup G-buffer
    setupGBuffer();

    addCube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.5f, 0.31f));
    addCube(glm::vec3(-10.0f, 0.0f, 0.0f), glm::vec3(0.31f, 1.0f, 0.5f));
    addSphere(glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.7f, 1.0f), 1.0f);
}

//-----------------------------------------------------------------------------------
// Getters
//-----------------------------------------------------------------------------------
GLFWwindow *Renderer::getWindow() const { return windowManager->getWindow(); }

Camera *Renderer::getCamera() const { return cameraManager.getActiveCamera(); }

//-----------------------------------------------------------------------------------
// Rendering and execution
//-----------------------------------------------------------------------------------
void Renderer::run()
{
    // Shader initialization
    Shader gShader(GBUFFER_VERTEX_SHADER_PATH, GBUFFER_FRAGMENT_SHADER_PATH);
    Shader lightningPassShader(LIGHTNING_PASS_VERTEX_SHADER_PATH, LIGHTNING_PASS_FRAGMENT_SHADER_PATH);
    Shader skyboxShader(SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH);

    // shader configuration
    lightningPassShader.use();
    lightningPassShader.setInt("gPosition", 0);
    lightningPassShader.setInt("gNormal", 1);
    lightningPassShader.setInt("gAlbedoSpec", 2);

    // skybox setup
    SkyBox skybox(DAY_SKYBOX_FACES, skyboxVertices);
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    // Time variables
    float currentFrame = 0.0f;
    float lastFrame    = 0.0f;
    float deltaTime    = 0.0f;

    // Render loop
    while (!glfwWindowShouldClose(windowManager->getWindow())) {
        // Calculate time difference
        currentFrame = glfwGetTime();
        deltaTime    = currentFrame - lastFrame;
        lastFrame    = currentFrame;

        processInput(deltaTime);
        render(gShader, lightningPassShader, skyboxShader, skybox);
        cameraManager.updateCamera(deltaTime);
    }
}

void Renderer::render(Shader &gShader, Shader &lightningPassShader, Shader &skyboxShader, SkyBox &skybox)
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // matrices setup
    glm::mat4 view             = cameraManager.getViewMatrix();
    const glm::mat4 projection = projectionManager.getProjectionMatrix();
    glm::mat4 model            = glm::mat4(1.0f);

    // Geometry pass
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set shader for model
    gShader.use();
    gShader.setMat4("projection", projection);
    gShader.setMat4("view", view);

    // Set texture loading
    gShader.setBool("useTexture", true);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 2.0f, -2.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    gShader.setMat4("model", model);

    // Draw backpack model
    backpackModel.Draw(gShader);

    // Draw other models
    gShader.setBool("useTexture", false);
    gShader.setFloat("specularIntensity", 0.5f);  // Adjust as needed
    for (const auto &geometricalModel : models) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, geometricalModel.position);

        gShader.setMat4("model", model);
        gShader.setVec3("objectColor", geometricalModel.color);

        glBindVertexArray(geometricalModel.VAO);
        glDrawArrays(GL_TRIANGLES, 0, geometricalModel.vertices.size() / 6);
    }

    // After finishing the geometry pass:
    glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);  // read from the G-buffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);        // draw to the default framebuffer
    glBlitFramebuffer(
        0, 0, windowWidth, windowHeight,  // source bounds
        0, 0, windowWidth, windowHeight,  // destination bounds
        GL_DEPTH_BUFFER_BIT,              // copy only depth buffer
        GL_NEAREST
    );

    // Switching back to default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Lightning pass
    glClear(GL_COLOR_BUFFER_BIT);
    lightningPassShader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gColorSpec);

    lightningPassShader.setupLightningUniforms(lightSource);
    lightningPassShader.setMat4("view", view);

    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

    renderQuad();

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    // Set depth function so skybox fragments pass if their depth is equal to or greater than existing depth.
    glDepthFunc(GL_LEQUAL);

    // Use the skybox shader and set uniforms:
    skyboxShader.use();
    glm::mat4 skyboxView = glm::mat4(glm::mat3(cameraManager.getViewMatrix()));  // remove translation
    skyboxShader.setMat4("view", skyboxView);
    skyboxShader.setMat4("projection", projectionManager.getProjectionMatrix());

    // Bind the skybox VAO and cubemap texture, then draw:
    glBindVertexArray(skybox.skyboxVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTextureID());
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Reset depth function if needed:
    glDepthFunc(GL_LESS);

    glfwSwapBuffers(windowManager->getWindow());
    glfwPollEvents();
}

void Renderer::processInput(float deltaTime)
{
    if (glfwGetKey(windowManager->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(windowManager->getWindow(), true);

    // Handle movement input (WASD keys)
    if (glfwGetKey(windowManager->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
        cameraManager.processKeyboardInput(Camera::FORWARD, deltaTime);
    if (glfwGetKey(windowManager->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
        cameraManager.processKeyboardInput(Camera::BACKWARD, deltaTime);
    if (glfwGetKey(windowManager->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
        cameraManager.processKeyboardInput(Camera::LEFT, deltaTime);
    if (glfwGetKey(windowManager->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
        cameraManager.processKeyboardInput(Camera::RIGHT, deltaTime);

    // Handle menu input for camera type
    if (glfwGetKey(windowManager->getWindow(), GLFW_KEY_1) == GLFW_PRESS) {
        if (cameraManager.getCurrentType() != FPS) {
            std::cout << "Switching to FPS Camera\n";
            cameraManager.switchCamera(FPS);
        }
    }

    if (glfwGetKey(windowManager->getWindow(), GLFW_KEY_2) == GLFW_PRESS) {
        if (cameraManager.getCurrentType() != CONSTANT) {
            std::cout << "Switching to Constant Camera\n";
            cameraManager.switchCamera(CONSTANT);
        }
    }

    if (glfwGetKey(windowManager->getWindow(), GLFW_KEY_3) == GLFW_PRESS) {
        if (cameraManager.getCurrentType() != CIRCULAR) {
            std::cout << "Switching to Circular Camera\n";
            cameraManager.switchCamera(CIRCULAR);
        }
    }
}

//-----------------------------------------------------------------------------------
// G-buffer setup
//-----------------------------------------------------------------------------------
void Renderer::setupGBuffer()
{
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    // setup position buffer
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

    // setup normal buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

    // setup color and specular buffer
    glGenTextures(1, &gColorSpec);
    glBindTexture(GL_TEXTURE_2D, gColorSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowWidth, windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColorSpec, 0);

    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachments);

    // create and attach depth buffer (renderbuffer)
    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth, windowHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    // finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::renderQuad()
{
    if (quadVAO == 0) {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
        };

        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void *>(nullptr));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

//-----------------------------------------------------------------------------------
// Add model
//-----------------------------------------------------------------------------------
void Renderer::addCube(const glm::vec3 &position, const glm::vec3 &color)
{
    InternalModel cube;
    cube.vertices = std::vector<float>(std::begin(cubeVertices), std::end(cubeVertices));
    cube.position = position;
    cube.color    = color;
    cube.rotation = glm::vec3(0.0f);

    glGenVertexArrays(1, &cube.VAO);
    glGenBuffers(1, &cube.VBO);

    // Bind the Vertex Array Object
    glBindVertexArray(cube.VAO);

    // Bind the Vertex Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, cube.VBO);

    // Load vertex data into the buffer
    glBufferData(GL_ARRAY_BUFFER, cube.vertices.size() * sizeof(float), cube.vertices.data(), GL_STATIC_DRAW);

    // Position attribute: 3 floats for the position of each vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    models.push_back(cube);
}

void Renderer::addSphere(const glm::vec3 &position, const glm::vec3 &color, float radius = 1.0f)
{
    InternalModel sphere;
    sphere.position = position;
    sphere.color    = color;
    sphere.rotation = glm::vec3(0.0f);

    // Temporary vectors to store positions and normals
    std::vector<float> vertices;
    const float lengthInv = 1.0f / radius;  // For normal calculation

    float sectorStep = 2 * M_PI / SPHERE_SECTORS;
    float stackStep  = M_PI / SPHERE_STACKS;
    float sectorAngle, stackAngle;

    // Generate vertices for each stack and sector
    for (unsigned int i = 0; i <= SPHERE_STACKS; ++i) {
        stackAngle = M_PI / 2 - i * stackStep;   // Starting from pi/2 to -pi/2
        float xy   = radius * cosf(stackAngle);  // r * cos(u)
        float z    = radius * sinf(stackAngle);  // r * sin(u)

        // Add (sectorCount+1) vertices per stack
        // The first and last vertices have same position and normal, but different tex coords
        for (unsigned int j = 0; j <= SPHERE_SECTORS; ++j) {
            sectorAngle = j * sectorStep;  // Starting from 0 to 2pi

            // Vertex position
            float x = xy * cosf(sectorAngle);  // r * cos(u) * cos(v)
            float y = xy * sinf(sectorAngle);  // r * cos(u) * sin(v)

            // Normalized vertex normal
            float nx = x * lengthInv;
            float ny = y * lengthInv;
            float nz = z * lengthInv;

            // Add vertex position
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // Add vertex normal
            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);
        }
    }

    // Generate indices
    std::vector<unsigned int> indices;
    unsigned int k1, k2;
    for (unsigned int i = 0; i < SPHERE_STACKS; ++i) {
        k1 = i * (SPHERE_SECTORS + 1);  // Beginning of current stack
        k2 = k1 + SPHERE_SECTORS + 1;   // Beginning of next stack

        for (unsigned int j = 0; j < SPHERE_SECTORS; ++j, ++k1, ++k2) {
            // 2 triangles per sector excluding first and last stacks
            if (i != 0) {
                // k1 => k2 => k1+1
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (SPHERE_STACKS - 1)) {
                // k1+1 => k2 => k2+1
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    // Convert indices to vertices
    std::vector<float> finalVertices;
    for (unsigned int i = 0; i < indices.size(); i++) {
        unsigned int index = indices[i] * 6;  // 6 values per vertex (3 position + 3 normal)
        for (int j = 0; j < 6; j++) {
            finalVertices.push_back(vertices[index + j]);
        }
    }

    sphere.vertices = finalVertices;

    // Generate and bind VAO and VBO
    glGenVertexArrays(1, &sphere.VAO);
    glGenBuffers(1, &sphere.VBO);

    glBindVertexArray(sphere.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, sphere.VBO);
    glBufferData(GL_ARRAY_BUFFER, sphere.vertices.size() * sizeof(float), sphere.vertices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    models.push_back(sphere);
}

//-----------------------------------------------------------------------------------
// Light source setup
//-----------------------------------------------------------------------------------
void Renderer::setupLightSource()
{
    lightSource.position  = LIGHT_POS;
    lightSource.color     = glm::vec3(1.0f, 1.0f, 1.0f);
    lightSource.constant  = 1.0f;
    lightSource.linear    = 0.09f;
    lightSource.quadratic = 0.032f;
}
