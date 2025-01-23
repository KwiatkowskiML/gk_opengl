// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
// clang-format on

// Camera settings
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

// Mouse settings
float yaw       = -90.0f;  // Yaw initialized to -90 to make initial direction face negative z
float pitch     = 0.0f;
float lastX     = 800.0f / 2.0f;
float lastY     = 600.0f / 2.0f;
bool firstMouse = true;

// Camera options
float cameraSpeed      = 2.5f;
float mouseSensitivity = 0.07f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

const unsigned int SCR_WIDTH  = 800;
const unsigned int SCR_HEIGHT = 600;

// Updated vertex shader with transformation matrices
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    out vec3 ourColor;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    void main()
    {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
        ourColor = aColor;
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    in vec3 ourColor;
    out vec4 FragColor;
    void main()
    {
        FragColor = vec4(ourColor, 1.0);
    }
)";

extern int LibMain()
{
    // GLFW initialization code remains the same...
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Scene", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Set callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // Capture mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Shader compilation code remains the same...
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Vertex data for a colored cube
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

    unsigned int VBO, VAO;
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

    // render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Create transformation matrices
        glm::mat4 model      = glm::mat4(1.0f);
        glm::mat4 view       = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        // Rotate the cube
        model      = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        view       = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        // Get uniform locations
        unsigned int modelLoc      = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc       = glGetUniformLocation(shaderProgram, "view");
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

        // Pass them to the shaders
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

// Updated process input to handle camera movement
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float currentFrame     = glfwGetTime();
    static float lastFrame = 0.0f;
    float deltaTime        = currentFrame - lastFrame;
    lastFrame              = currentFrame;

    float velocity = cameraSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += velocity * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= velocity * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * velocity;
}

// New mouse callback function
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse) {
        lastX      = xpos;
        lastY      = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to top
    lastX         = xpos;
    lastY         = ypos;

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Constrain pitch to avoid flipping
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    // Update camera front vector
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }
