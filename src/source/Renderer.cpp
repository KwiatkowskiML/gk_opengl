//
// Created by michal on 1/30/25.
//

#include "includes/Renderer.h"

Renderer::Renderer(unsigned int width, unsigned int height)
    : windowManager(std::make_unique<WindowManager>(width, height)), windowWidth(width), windowHeight(height)
{
    setupLightSource();
    glfwSetWindowUserPointer(
        windowManager->getWindow(), this
    );  // Set the window user pointer to this Renderer instance
}

Renderer::~Renderer() { glfwTerminate(); }

void Renderer::run()
{
    // Shader initialization
    Shader lightningShader(LIGHTNING_VERTEX_SHADER_PATH, LIGHTNING_FRAGMENT_SHADER_PATH);

    // Vertex data setup
    setupVertexData();

    float currentFrame = glfwGetTime();             // Get current frame time
    float lastFrame    = 0.0f;                      // Track last frame time
    float deltaTime    = currentFrame - lastFrame;  // Calculate time difference between frames
    lastFrame          = currentFrame;              // Update last frame time

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

    // Cleanup resources after the loop ends
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}
GLFWwindow *Renderer::getWindow() const { return windowManager->getWindow(); }
Camera *Renderer::getCamera() const { return cameraManager.getActiveCamera(); }

void Renderer::setupVertexData()
{
    glGenVertexArrays(1, &VAO);  // Generate Vertex Array Object
    glGenBuffers(1, &VBO);       // Generate Vertex Buffer Object

    // Bind the Vertex Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Load vertex data into the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // Bind the Vertex Array Object
    glBindVertexArray(VAO);

    // Position attribute: 3 floats for the position of each vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}
void Renderer::processInput(float deltaTime)
{
    if (glfwGetKey(windowManager->getWindow(), GLFW_KEY_ESCAPE) ==
        GLFW_PRESS)  // Close the window if Escape key is pressed
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

    // constant cube
    glm::mat4 model1 = glm::mat4(1.0f);
    model1           = glm::translate(model1, glm::vec3(-10.0f, 0.0f, 0.0f));
    shader.setMat4("model", model1);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Create transformation matrices for model, view, and projection
    glm::mat4 model            = glm::mat4(1.0f);
    const glm::mat4 view       = cameraManager.getViewMatrix();
    const glm::mat4 projection = glm::perspective(
        glm::radians(cameraManager.getZoom()), static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.1f,
        100.0f
    );

    // Rotate the cube based on time
    model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));

    // Set the transformation matrices in the shader
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    // Draw the cube using the vertex data
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(windowManager->getWindow());
    glfwPollEvents();
}

void Renderer::setupLightSource()
{
    lightSource.position  = LIGHT_POS;
    lightSource.color     = glm::vec3(1.0f, 1.0f, 1.0f);
    lightSource.constant  = 1.0f;
    lightSource.linear    = 0.09f;
    lightSource.quadratic = 0.032f;
}
