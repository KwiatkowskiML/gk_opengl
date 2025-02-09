//
// Created by michal on 1/23/25.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

// TODO: fix paths
const std::string LIGHTNING_VERTEX_SHADER_PATH =
    "/home/michal/studia/sem5/gk/gk_open_gl2/gk_opengl/src/shaders/lightning_vertex_shader.glsl";
const std::string LIGHTNING_FRAGMENT_SHADER_PATH =
    "/home/michal/studia/sem5/gk/gk_open_gl2/gk_opengl/src/shaders/lightning_fragment_shader.glsl";

const std::string MODEL_VERTEX_SHADER_PATH =
    "/home/michal/studia/sem5/gk/gk_open_gl2/gk_opengl/src/shaders/model_vertex_shader.glsl";
const std::string MODEL_FRAGMENT_SHADER_PATH =
    "/home/michal/studia/sem5/gk/gk_open_gl2/gk_opengl/src/shaders/model_fragment_shader.glsl";

const std::string GBUFFER_VERTEX_SHADER_PATH =
    "/home/michal/studia/sem5/gk/gk_open_gl2/gk_opengl/src/shaders/gbuffer_vertex_shader.glsl";
const std::string GBUFFER_FRAGMENT_SHADER_PATH =
    "/home/michal/studia/sem5/gk/gk_open_gl2/gk_opengl/src/shaders/gbuffer_fragment_shader.glsl";

const std::string BACKPACK_MODEL_PATH =
    "/home/michal/studia/sem5/gk/gk_open_gl2/gk_opengl/resources/backpack/backpack.obj";

const std::string FLASHLIGHT_MODEL_PATH =
    "/home/michal/studia/sem5/gk/gk_open_gl2/gk_opengl/resources/flashlight/Flashlight.obj";

constexpr float cubeVertices[] = {
    // positions          // normal vectors
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,
    0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,

    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
    -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,
    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
    0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,
    0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f
};

constexpr float pyramidVertices[] = {
    // Base (square face) - counterclockwise winding for correct normal direction
    // positions          // normal vectors
    -0.5f, 0.0f, -0.5f, 0.0f, -1.0f, 0.0f,  // bottom left
    0.5f, 0.0f, -0.5f, 0.0f, -1.0f, 0.0f,   // bottom right
    0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f,    // top right
    0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f,    // top right
    -0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f,   // top left
    -0.5f, 0.0f, -0.5f, 0.0f, -1.0f, 0.0f,  // bottom left

    // Side 1 (front)
    -0.5f, 0.0f, -0.5f, 0.0f, 0.707f, -0.707f,  // base left
    0.5f, 0.0f, -0.5f, 0.0f, 0.707f, -0.707f,   // base right
    0.0f, 0.75f, 0.0f, 0.0f, 0.707f, -0.707f,   // apex

    // Side 2 (right)
    0.5f, 0.0f, -0.5f, 0.707f, 0.707f, 0.0f,  // base front
    0.5f, 0.0f, 0.5f, 0.707f, 0.707f, 0.0f,   // base back
    0.0f, 0.75f, 0.0f, 0.707f, 0.707f, 0.0f,  // apex

    // Side 3 (back)
    0.5f, 0.0f, 0.5f, 0.0f, 0.707f, 0.707f,   // base right
    -0.5f, 0.0f, 0.5f, 0.0f, 0.707f, 0.707f,  // base left
    0.0f, 0.75f, 0.0f, 0.0f, 0.707f, 0.707f,  // apex

    // Side 4 (left)
    -0.5f, 0.0f, 0.5f, -0.707f, 0.707f, 0.0f,   // base back
    -0.5f, 0.0f, -0.5f, -0.707f, 0.707f, 0.0f,  // base front
    0.0f, 0.75f, 0.0f, -0.707f, 0.707f, 0.0f    // apex
};

constexpr int WINDOW_WIDTH  = 800;
constexpr int WINDOW_HEIGHT = 600;

constexpr unsigned int SPHERE_SECTORS = 36;  // longitude
constexpr unsigned int SPHERE_STACKS  = 18;  // latitude

constexpr float BASIC_AMBIENT_STRENGTH  = 0.1f;
constexpr float BASIC_SPECULAR_STRENGTH = 0.5f;
constexpr float BASIC_SHININESS         = 32.0f;

constexpr glm::vec3 INITIAL_FPS_CAMERA_POSITION(0.0f, 0.0f, 3.0f);
constexpr glm::vec3 INITIAL_CONSTANT_CAMERA_POSITION(0.0f, 5.0f, 10.0f);
constexpr glm::vec3 INITIAL_CIRCULAR_CAMERA_POSITION(0.0f, 5.0f, 10.0f);
constexpr glm::vec3 CAMERA_TARGET_POSITION(0.0f, 0.0f, 0.0f);
constexpr glm::vec3 LIGHT_POS(2.0f, 2.0f, 2.0f);

constexpr glm::vec3 FLASHLIGHT_SHIFT(0.09f, -0.1f, -0.3f);
constexpr glm::vec3 INITIAL_FLASHLIGHT_POSITION(0.09f, -0.1f, 2.7f);
constexpr glm::vec3 FLASHLIGHT_SCALE(0.08f, 0.08f, 0.08f);

constexpr glm::vec3 OBJECT_COLOR(1.0f, 0.5f, 0.31f);

#endif  // CONSTANTS_H
