//
// Created by michal on 1/30/25.
//

#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <GLFW/glfw3.h>

class WindowManager
{
    public:
    WindowManager(unsigned int width, unsigned int height);
    ~WindowManager();
    GLFWwindow* getWindow() const { return window; }

    private:
    GLFWwindow* window;
    int width, height;

    // GLFW setup: Initializes GLFW and sets OpenGL version
    void initializeGLFW();

    // Create a window with the specified dimensions and check for errors
    void createWindow();

    // Set up GLFW callbacks for framebuffer size and mouse movement
    void setupCallbacks() const;

    // Initialize GLAD to load OpenGL function pointers
    void initializeGLAD();

    static void framebuffer_size_callback(GLFWwindow* /*window*/, const int width, const int height);
    static void mouse_callback(GLFWwindow* window, const double xpos, const double ypos);
};

#endif  // WINDOWMANAGER_H
