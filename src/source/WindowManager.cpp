//
// Created by michal on 1/30/25.
//

#include <glad/glad.h>
#include <iostream>

#include "includes/WindowManager.h"

#include <includes/Renderer.h>

WindowManager::WindowManager(unsigned int width, unsigned int height) : width(width), height(height)
{
    initializeGLFW();                        // Initialize GLFW
    createWindow();                          // Create the window
    setupCallbacks();                        // Set up GLFW callbacks
    initializeGLAD();                        // Initialize GLAD for OpenGL function loading
    glfwSetWindowUserPointer(window, this);  // Set the window user pointer to this Renderer instance
}
WindowManager::~WindowManager() { glfwTerminate(); }

void WindowManager::initializeGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  // Set OpenGL version 3.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                  // Set OpenGL version 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // Use core profile
}
void WindowManager::createWindow()
{
    window = glfwCreateWindow(width, height, "3D Scene", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();  // Clean up GLFW
        throw std::runtime_error("Window creation failed");
    }
    glfwMakeContextCurrent(window);  // Make the context current for the window
}

void WindowManager::initializeGLAD()
{
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        throw std::runtime_error("Failed to initialize GLAD");
    }
    glEnable(GL_DEPTH_TEST);  // Enable depth testing for 3D rendering
}

void WindowManager::setupCallbacks() const
{
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // Set resize callback
    glfwSetCursorPosCallback(window, mouse_callback);                   // Set mouse position callback
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);        // Disable cursor
}

void WindowManager::framebuffer_size_callback(GLFWwindow *, const int width, const int height)
{
    glViewport(0, 0, width, height);
}
void WindowManager::mouse_callback(GLFWwindow *window, const double xpos, const double ypos)
{
    static bool firstMouse = true;
    static float lastX     = WINDOW_WIDTH / 2.0f;
    static float lastY     = WINDOW_HEIGHT / 2.0f;

    if (firstMouse) {
        lastX      = xpos;
        lastY      = ypos;
        firstMouse = false;
    }

    // Calculate mouse movement offsets
    float const xoffset = xpos - lastX;
    float const yoffset = lastY - ypos;
    lastX               = xpos;
    lastY               = ypos;

    // Retrieve the camera instance and process mouse movement to update camera orientation
    if (const auto renderer = static_cast<Renderer *>(glfwGetWindowUserPointer(window))) {
        renderer->getCamera()->ProcessMouseMovement(xoffset, yoffset);
    }
}
