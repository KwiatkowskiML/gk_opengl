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
#include "Shader.h"

constexpr glm::vec3 INITIAL_FPS_CAMERA_POSITION(0.0f, 0.0f, 3.0f);
constexpr glm::vec3 INITIAL_CONSTANT_CAMERA_POSITION(0.0f, 5.0f, 10.0f);
constexpr glm::vec3 INITIAL_CIRCULAR_CAMERA_POSITION(0.0f, 5.0f, 10.0f);
constexpr glm::vec3 CAMERA_TARGET_POSITION(0.0f, 0.0f, 0.0f);
constexpr glm::vec3 LIGHT_POS(1.2f, 1.0f, 2.0f);

class Renderer
{
    public:
    // Constructor: Initializes the renderer with window dimensions, GLFW, and GLAD setup
    Renderer(unsigned int width, unsigned int height) : SCR_WIDTH(width), SCR_HEIGHT(height)
    {
        camera = new CameraFPS(INITIAL_FPS_CAMERA_POSITION);  // Create a new FPS camera

        initializeGLFW();                        // Initialize GLFW
        createWindow();                          // Create the window
        setupCallbacks();                        // Set up GLFW callbacks
        initializeGLAD();                        // Initialize GLAD for OpenGL function loading
        glfwSetWindowUserPointer(window, this);  // Set the window user pointer to this Renderer instance
    }

    // Destructor: Terminates GLFW when the Renderer object is destroyed
    ~Renderer()
    {
        glfwTerminate();
        delete camera;
    }

    // Method to change camera type
    void setCamera(Camera* newCamera)
    {
        delete camera;
        camera = newCamera;
    }

    void updateCamera()
    {
        float currentFrame     = glfwGetTime();             // Get current frame time
        static float lastFrame = 0.0f;                      // Track last frame time
        float deltaTime        = currentFrame - lastFrame;  // Calculate time difference between frames
        lastFrame              = currentFrame;              // Update last frame time

        if (cameraType == CameraType::CIRCULAR)
            camera->Update(deltaTime);
    }

    // Main render loop; starts the shader, sets up vertex data, and keeps the window open until closed
    void run()
    {
        // Shader initialization
        Shader lightningShader(LIGHTNING_VERTEX_SHADER_PATH, LIGHTNING_FRAGMENT_SHADER_PATH);

        // Vertex data setup
        setupVertexData();

        // Render loop
        while (!glfwWindowShouldClose(window)) {
            processInput();           // Handle input (e.g., keyboard)
            render(lightningShader);  // Render the scene
            updateCamera();
        }

        // Cleanup resources after the loop ends
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    // Getter for the window
    GLFWwindow* getWindow() const { return window; }

    // Getter for the camera
    Camera* getCamera() { return camera; }

    private:
    // The window for OpenGL rendering
    GLFWwindow* window;

    // The camera used for scene viewing
    Camera* camera;

    // Window dimensions
    unsigned int SCR_WIDTH, SCR_HEIGHT;

    // OpenGL buffer objects for the cube
    unsigned int VAO, VBO;

    // CameraType enum to keep track of the current camera type
    CameraType cameraType = CameraType::FPS;

    // GLFW setup: Initializes GLFW and sets OpenGL version
    void initializeGLFW()
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  // Set OpenGL version 3.x
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                  // Set OpenGL version 3.3
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // Use core profile
    }

    // Create a window with the specified dimensions and check for errors
    void createWindow()
    {
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Scene", nullptr, nullptr);
        if (window == nullptr) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();  // Clean up GLFW
            throw std::runtime_error("Window creation failed");
        }
        glfwMakeContextCurrent(window);  // Make the context current for the window
    }

    // Set up GLFW callbacks for framebuffer size and mouse movement
    void setupCallbacks() const
    {
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // Set resize callback
        glfwSetCursorPosCallback(window, mouse_callback);                   // Set mouse position callback
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);        // Disable cursor
    }

    // Initialize GLAD to load OpenGL function pointers
    void initializeGLAD()
    {
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            throw std::runtime_error("Failed to initialize GLAD");
        }
        glEnable(GL_DEPTH_TEST);  // Enable depth testing for 3D rendering
    }

    // Setup the vertex array and buffer objects for rendering a cube
    void setupVertexData()
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
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    // Process user input (keyboard controls for camera movement)
    void processInput()
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  // Close the window if Escape key is pressed
            glfwSetWindowShouldClose(window, true);

        float currentFrame     = glfwGetTime();             // Get current frame time
        static float lastFrame = 0.0f;                      // Track last frame time
        float deltaTime        = currentFrame - lastFrame;  // Calculate time difference between frames
        lastFrame              = currentFrame;              // Update last frame time

        // Handle movement input (WASD keys)
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera->ProcessKeyboard(Camera::FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera->ProcessKeyboard(Camera::BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera->ProcessKeyboard(Camera::LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera->ProcessKeyboard(Camera::RIGHT, deltaTime);

        // Handle menu input for camera type
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            if (cameraType != CameraType::FPS) {
                std::cout << "Switching to FPS Camera\n";
                setCamera(new CameraFPS(INITIAL_FPS_CAMERA_POSITION));
                cameraType = CameraType::FPS;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
            if (cameraType != CameraType::CONSTANT) {
                std::cout << "Switching to Constant Camera\n";
                setCamera(new CameraConstant(INITIAL_CONSTANT_CAMERA_POSITION, CAMERA_TARGET_POSITION)
                );  // Replace with your constant camera class if defined
                cameraType = CameraType::CONSTANT;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
            if (cameraType != CameraType::CIRCULAR) {
                std::cout << "Switching to Circular Camera\n";
                setCamera(new CameraCircular(CAMERA_TARGET_POSITION)
                );  // Replace with your circular camera class if defined
                cameraType = CameraType::CIRCULAR;
            }
        }
    }

    // Render the scene (clear screen, set shader uniforms, and draw cube)
    void render(Shader& shader)
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);                // Set the background color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear the color and depth buffers

        shader.use();  // Use the shader program
        shader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
        shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setVec3("lightPos", LIGHT_POS);
        shader.setVec3("viewPos", camera->Position);

        // constant cube
        glm::mat4 model1 = glm::mat4(1.0f);
        model1           = glm::translate(model1, glm::vec3(2.0f, 0.0f, 0.0f));
        shader.setMat4("model", model1);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Create transformation matrices for model, view, and projection
        glm::mat4 model            = glm::mat4(1.0f);          // Identity matrix for model transformation
        const glm::mat4 view       = camera->GetViewMatrix();  // Camera view matrix
        const glm::mat4 projection = glm::perspective(
            glm::radians(camera->GetZoom()), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f,
            100.0f
        );  // Projection matrix for perspective

        // Rotate the cube based on time
        model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));

        // Set the transformation matrices in the shader
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        // Draw the cube using the vertex data
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);  // Draw 36 vertices (for the cube)

        glfwSwapBuffers(window);  // Swap the front and back buffers
        glfwPollEvents();         // Poll for events (like input)
    }

    // GLFW callback for resizing the window and adjusting the OpenGL viewport
    static void framebuffer_size_callback(GLFWwindow* /*window*/, const int width, const int height)
    {
        glViewport(0, 0, width, height);  // Set the OpenGL viewport to match the new window size
    }

    // GLFW callback for mouse movement (to update camera orientation)
    static void mouse_callback(GLFWwindow* window, const double xpos, const double ypos)
    {
        static bool firstMouse = true;  // Flag to handle the first mouse movement
        static float lastX     = WINDOW_WIDTH / 2.0f;
        static float lastY     = WINDOW_HEIGHT / 2.0f;

        if (firstMouse) {
            lastX      = xpos;  // Set initial mouse position
            lastY      = ypos;
            firstMouse = false;
        }

        // Calculate mouse movement offsets
        float const xoffset = xpos - lastX;
        float const yoffset = lastY - ypos;
        lastX               = xpos;
        lastY               = ypos;

        // Retrieve the camera instance and process mouse movement to update camera orientation
        if (const auto renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window))) {
            renderer->getCamera()->ProcessMouseMovement(xoffset, yoffset);
        }
    }
};

#endif  // RENDERER_H
