//
// Created by michal on 1/23/25.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <vector>

//------------------------------------------------------------------------------
// Shader paths
//------------------------------------------------------------------------------
const std::string PATH_PREFIX = "../../";

const std::string GBUFFER_VERTEX_SHADER_PATH   = PATH_PREFIX + "src/shaders/gbuffer_vertex_shader.glsl";
const std::string GBUFFER_FRAGMENT_SHADER_PATH = PATH_PREFIX + "src/shaders/gbuffer_fragment_shader.glsl";

const std::string LIGHTNING_PASS_VERTEX_SHADER_PATH   = PATH_PREFIX + "src/shaders/lightning_pass_vertex_shader.glsl";
const std::string LIGHTNING_PASS_FRAGMENT_SHADER_PATH = PATH_PREFIX + "src/shaders/lightning_pass_fragment_shader.glsl";

const std::string SKYBOX_VERTEX_SHADER_PATH   = PATH_PREFIX + "src/shaders/skybox_vertex_shader.glsl";
const std::string SKYBOX_FRAGMENT_SHADER_PATH = PATH_PREFIX + "src/shaders/skybox_fragment_shader.glsl";

//------------------------------------------------------------------------------
// Model paths
//------------------------------------------------------------------------------
const std::string BACKPACK_MODEL_PATH   = PATH_PREFIX + "resources/backpack/backpack.obj";
const std::string FLASHLIGHT_MODEL_PATH = PATH_PREFIX + "resources/flashlight/Flashlight.obj";
const std::string ITALY_MODEL_PATH =
    PATH_PREFIX + "resources/map/counter-strike-italy-classic-cs-map-in-3d/source/cs_italy/cs_italy.obj";

//------------------------------------------------------------------------------
// Skybox maps
//------------------------------------------------------------------------------
const std::vector<std::string> DAY_SKYBOX_FACES = {
    PATH_PREFIX + "resources/cubemaps/skybox/right.jpg", PATH_PREFIX + "resources/cubemaps/skybox/left.jpg",
    PATH_PREFIX + "resources/cubemaps/skybox/top.jpg",   PATH_PREFIX + "resources/cubemaps/skybox/bottom.jpg",
    PATH_PREFIX + "resources/cubemaps/skybox/front.jpg", PATH_PREFIX + "resources/cubemaps/skybox/back.jpg"
};

const std::vector<std::string> NIGHT_SKYBOX_FACES = {
    PATH_PREFIX + "resources/cubemaps/night/nightRight.png", PATH_PREFIX + "resources/cubemaps/night/nightLeft.png",
    PATH_PREFIX + "resources/cubemaps/night/nightTop.png",   PATH_PREFIX + "resources/cubemaps/night/nightBottom.png",
    PATH_PREFIX + "resources/cubemaps/night/nightFront.png", PATH_PREFIX + "resources/cubemaps/night/nightBack.png"
};

//------------------------------------------------------------------------------
// Cube and pyramid vertices
//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
// Skybox vertices
//------------------------------------------------------------------------------
const std::vector<float> skyboxVertices = {
    // positions
    -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
    1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

    -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
    -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

    1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

    -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
    1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f
};

//------------------------------------------------------------------------------
// Window properties
//------------------------------------------------------------------------------
constexpr int WINDOW_WIDTH  = 1400;
constexpr int WINDOW_HEIGHT = 800;

//------------------------------------------------------------------------------
// Sphere properties
//------------------------------------------------------------------------------
constexpr unsigned int SPHERE_SECTORS = 36;  // longitude
constexpr unsigned int SPHERE_STACKS  = 18;  // latitude

//------------------------------------------------------------------------------
// Camera properties
//------------------------------------------------------------------------------
constexpr glm::vec3 INITIAL_FPS_CAMERA_POSITION(0.0f, 0.0f, 3.0f);
constexpr glm::vec3 INITIAL_CONSTANT_CAMERA_POSITION(0.0f, 5.0f, 10.0f);
constexpr glm::vec3 INITIAL_CIRCULAR_CAMERA_POSITION(0.0f, 5.0f, 10.0f);
constexpr glm::vec3 CAMERA_TARGET_POSITION(0.0f, 0.0f, 0.0f);

//------------------------------------------------------------------------------
// Light source properties
//------------------------------------------------------------------------------
constexpr glm::vec3 LIGHT_POS(2.0f, 2.0f, 2.0f);
constexpr glm::vec3 LIGHT_COLOR(1.0f, 1.0f, 1.0f);
constexpr float LIGHT_CONSTANT  = 1.0f;
constexpr float LIGHT_LINEAR    = 0.09f;
constexpr float LIGHT_QUADRATIC = 0.032f;

constexpr float BASIC_AMBIENT_STRENGTH  = 0.1f;
constexpr float BASIC_SPECULAR_STRENGTH = 0.5f;
constexpr float BASIC_SHININESS         = 32.0f;

constexpr float LIGHT_X_LOWER_BOUND = -20.0f;
constexpr float LIGHT_X_UPPER_BOUND = 20.0f;
constexpr float LIGHT_Y_LOWER_BOUND = -5.0f;
constexpr float LIGHT_Y_UPPER_BOUND = 10.0f;
constexpr float LIGHT_Z_LOWER_BOUND = -100.0f;
constexpr float LIGHT_Z_UPPER_BOUND = 10.0f;

constexpr glm::vec3 FLASHLIGHT_SHIFT(0.09f, -0.1f, -0.3f);
constexpr glm::vec3 INITIAL_FLASHLIGHT_POSITION(2.0f, -1.0f, 2.0f);
constexpr glm::vec3 FLASHLIGHT_SCALE(0.2f, 0.2f, 0.2f);
constexpr int NR_POINT_LIGHTS = 10;

//------------------------------------------------------------------------------
// Object properties
//------------------------------------------------------------------------------
constexpr glm::vec3 BACKPACK_POSITION(2.0f, -1.0f, -2.0f);
constexpr glm::vec3 OBJECT_COLOR(1.0f, 0.5f, 0.31f);

//------------------------------------------------------------------------------
// Fog properties
//------------------------------------------------------------------------------
constexpr glm::vec3 FOG_COLOR(0.7f, 0.7f, 0.7f);
constexpr float FOG_START = 10.0f;
constexpr float FOG_END   = 50.0f;

#endif  // CONSTANTS_H
