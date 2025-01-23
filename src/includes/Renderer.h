//
// Created by michal on 1/23/25.
//

#ifndef RENDERER_H
#define RENDERER_H

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// clang-format on

#include "Camera.h"
#include "Shader.h"

class Renderer
{
    public:
    Renderer(unsigned int width, unsigned int height)
        : SCR_WIDTH(width), SCR_HEIGHT(height), camera(glm::vec3(0.0f, 0.0f, 3.0f))
    {
        initializeGLFW();
        createWindow();
        setupCallbacks();
        initializeGLAD();
        glfwSetWindowUserPointer(window, this);
    }

    ~Renderer() { glfwTerminate(); }

    void run()
    {
        // Shader initialization
        Shader shader(
            "/home/michal/studia/sem5/gk/gk_open_gl2/gk_opengl/src/shaders/vertex_shader.glsl",
            "/home/michal/studia/sem5/gk/gk_open_gl2/gk_opengl/src/shaders/fragment_shader.glsl"
        );

        // Vertex data setup
        setupVertexData();

        // Render loop
        while (!glfwWindowShouldClose(window)) {
            processInput();
            render(shader);
        }

        // Cleanup
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    GLFWwindow* getWindow() { return window; }
    Camera& getCamera() { return camera; }

    private:
    GLFWwindow* window;
    Camera camera;
    unsigned int SCR_WIDTH, SCR_HEIGHT;
    unsigned int VAO, VBO;

    void initializeGLFW()
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    void createWindow()
    {
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Scene", NULL, NULL);
        if (window == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            throw std::runtime_error("Window creation failed");
        }
        glfwMakeContextCurrent(window);
    }

    void setupCallbacks()
    {
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void initializeGLAD()
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            throw std::runtime_error("Failed to initialize GLAD");
        }
        glEnable(GL_DEPTH_TEST);
    }

    void setupVertexData()
    {
        float vertices[] = {// positions          // colors
                            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                            0.5f,  0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 0.0f, 0.0f, 1.0f,
                            -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, 0.0f, -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,

                            -0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  0.0f, 1.0f, 0.0f,
                            0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
                            -0.5f, 0.5f,  0.5f,  1.0f, 1.0f, 0.0f, -0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0f,

                            -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
                            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
                            -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

                            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
                            0.5f,  -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
                            0.5f,  -0.5f, 0.5f,  1.0f, 1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

                            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                            0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
                            -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 0.0f, -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,

                            -0.5f, 0.5f,  -0.5f, 1.0f, 0.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
                            0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
                            -0.5f, 0.5f,  0.5f,  1.0f, 1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 0.0f, 0.0f
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void processInput()
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        float currentFrame     = glfwGetTime();
        static float lastFrame = 0.0f;
        float deltaTime        = currentFrame - lastFrame;
        lastFrame              = currentFrame;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(Camera::FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(Camera::BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(Camera::LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(Camera::RIGHT, deltaTime);
    }

    void render(Shader& shader)
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        // Create transformation matrices
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view  = camera.GetViewMatrix();
        glm::mat4 projection =
            glm::perspective(glm::radians(camera.GetZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        // Rotate the cube
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

        // Set uniform matrices
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        // Draw cube
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        static bool firstMouse = true;
        static float lastX     = 800.0f / 2.0f;
        static float lastY     = 600.0f / 2.0f;

        if (firstMouse) {
            lastX      = xpos;
            lastY      = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX         = xpos;
        lastY         = ypos;

        // Retrieve the camera instance and process mouse movement
        Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
        if (renderer) {
            renderer->getCamera().ProcessMouseMovement(xoffset, yoffset);
        }
    }
};

#endif  // RENDERER_H
