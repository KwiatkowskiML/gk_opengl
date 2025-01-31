//
// Created by michal on 1/30/25.
//

#include "includes/Renderer.h"

Renderer::~Renderer()
{
    for (const auto &model : models) {
        glDeleteVertexArrays(1, &model.VAO);
        glDeleteBuffers(1, &model.VBO);
    }
    glfwTerminate();
}

GLFWwindow *Renderer::getWindow() const { return windowManager->getWindow(); }

Camera *Renderer::getCamera() const { return cameraManager.getActiveCamera(); }

Renderer::Renderer(unsigned int width, unsigned int height)
    : windowManager(std::make_unique<WindowManager>(width, height)), windowWidth(width), windowHeight(height)
{
    setupLightSource();
    glfwSetWindowUserPointer(windowManager->getWindow(), this);

    addCube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.5f, 0.31f));
    addCube(glm::vec3(-10.0f, 0.0f, 0.0f), glm::vec3(0.31f, 1.0f, 0.5f));
}

void Renderer::run()
{
    // Shader initialization
    Shader lightningShader(LIGHTNING_VERTEX_SHADER_PATH, LIGHTNING_FRAGMENT_SHADER_PATH);

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
        render(lightningShader);
        cameraManager.updateCamera(deltaTime);
    }
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

void Renderer::setupLightSource()
{
    lightSource.position  = LIGHT_POS;
    lightSource.color     = glm::vec3(1.0f, 1.0f, 1.0f);
    lightSource.constant  = 1.0f;
    lightSource.linear    = 0.09f;
    lightSource.quadratic = 0.032f;
}

void Renderer::addCube(const glm::vec3 &position, const glm::vec3 &color)
{
    Model cube;
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

void Renderer::render(Shader &shader) const
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    shader.setVec3("light.position", lightSource.position);
    shader.setVec3("light.color", lightSource.color);
    shader.setFloat("light.constant", lightSource.constant);
    shader.setFloat("light.linear", lightSource.linear);
    shader.setFloat("light.quadratic", lightSource.quadratic);

    shader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    shader.setFloat("phongProperties.ambientStrength", 0.1f);
    shader.setFloat("phongProperties.shininess", 32.0f);
    shader.setFloat("phongProperties.specularStrength", 0.5f);

    const glm::mat4 view       = cameraManager.getViewMatrix();
    const glm::mat4 projection = glm::perspective(
        glm::radians(cameraManager.getZoom()), static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.1f,
        100.0f
    );

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
