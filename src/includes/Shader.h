//
// Created by michal on 1/23/25.
//

#ifndef SHADER_H
#define SHADER_H

#include "Lights/LightSource.h"
#include "Lights/SpotLight.h"

#include <glad/glad.h>
#include <fstream>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Shader
{
    public:
    // Shader program ID
    unsigned int ID;

    // Constructor
    Shader(const std::string &vertexPath, const std::string &fragmentPath);

    // Use the shader
    void use() const;

    //-----------------------------------------------------------------------------------
    // Set uniform values
    //-----------------------------------------------------------------------------------
    void setBool(const std::string &name, const bool value) const;
    void setInt(const std::string &name, const int value) const;
    void setFloat(const std::string &name, const float value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;

    //-----------------------------------------------------------------------------------
    // Setup uniform values
    //-----------------------------------------------------------------------------------
    void setupLightningUniforms(
        const LightSource &lightSource, const SpotLight &spot_light, const std::vector<LightSource> &pointLights
    ) const;

    private:
    //-----------------------------------------------------------------------------------
    // Private functions
    //-----------------------------------------------------------------------------------
    std::string readShaderFile(const std::string &filePath);
    unsigned int compileShader(const char *shaderCode, const GLenum shaderType);
    void checkCompileErrors(const unsigned int shader, std::string type);
};

#endif  // SHADER_H
