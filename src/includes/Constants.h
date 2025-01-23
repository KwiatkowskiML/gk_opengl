//
// Created by michal on 1/23/25.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

const std::string VERTEX_SHADER_PATH =
    "/home/michal/studia/sem5/gk/gk_open_gl2/gk_opengl/src/shaders/vertex_shader.glsl";
const std::string FRAGMENT_SHADER_PATH =
    "/home/michal/studia/sem5/gk/gk_open_gl2/gk_opengl/src/shaders/fragment_shader.glsl";

constexpr float cubeVertices[] = {  // positions          // colors
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

constexpr int WINDOW_WIDTH  = 800;
constexpr int WINDOW_HEIGHT = 600;

#endif  // CONSTANTS_H
