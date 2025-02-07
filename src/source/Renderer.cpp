//
// Created by michal on 1/30/25.
//

#include "includes/Renderer.h"
#include <filesystem>
#include "includes/Models/Model.h"

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
}

Renderer::Renderer(unsigned int width, unsigned int height)
    : windowManager(std::make_unique<WindowManager>(width, height)),
      windowWidth(width),
      windowHeight(height),
      projectionManager(width, height, cameraManager.getZoom())
{
    setupLightSource();
    glfwSetWindowUserPointer(windowManager->getWindow(), this);

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
    Shader lightningShader(LIGHTNING_VERTEX_SHADER_PATH, LIGHTNING_FRAGMENT_SHADER_PATH);

    float currentFrame = 0.0f;
    float lastFrame    = 0.0f;
    float deltaTime    = 0.0f;

    // Model ourModel(FileSystem::getPath("resources/objects/backpack/backpack.obj"));

    auto path = std::filesystem::path("resources/backpack.obj");
    std::cout << path << std::endl;
    NewModel ourModel(path.string());

    // Render loop
    while (!glfwWindowShouldClose(windowManager->getWindow())) {
        // Calculate time difference
        currentFrame = glfwGetTime();
        deltaTime    = currentFrame - lastFrame;
        lastFrame    = currentFrame;

        processInput(deltaTime);
        render(lightningShader);
        cameraManager.updateCamera(deltaTime);
    }
}

void Renderer::render(Shader &shader) const
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Activate shader
    shader.use();

    // Setup uniforms
    shader.setVec3("light.position", lightSource.position);
    shader.setVec3("light.color", lightSource.color);
    shader.setFloat("light.constant", lightSource.constant);
    shader.setFloat("light.linear", lightSource.linear);
    shader.setFloat("light.quadratic", lightSource.quadratic);
    shader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    shader.setFloat("phongProperties.ambientStrength", 0.1f);
    shader.setFloat("phongProperties.shininess", 32.0f);
    shader.setFloat("phongProperties.specularStrength", 0.5f);

    // Get view and projection matrix
    const glm::mat4 view       = cameraManager.getViewMatrix();
    const glm::mat4 projection = projectionManager.getProjectionMatrix();

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    for (const auto &model : models) {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix           = glm::translate(modelMatrix, model.position);

        // Apply rotation only to the center cube
        if (model.position == glm::vec3(0.0f, 0.0f, 0.0f)) {
            modelMatrix = glm::rotate(modelMatrix, static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
        }

        shader.setMat4("model", modelMatrix);
        shader.setVec3("objectColor", model.color);

        glBindVertexArray(model.VAO);
        glDrawArrays(GL_TRIANGLES, 0, model.vertices.size() / 6);
    }

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
