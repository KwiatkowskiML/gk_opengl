//
// Created by michal on 1/30/25.
//

#include "includes/Shader.h"
#include <includes/Constants.h>
#include <cstdlib>
#include <ctime>

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
{
    // Read vertex/fragment shader source files
    const std::string vertexCode   = readShaderFile(vertexPath);
    const std::string fragmentCode = readShaderFile(fragmentPath);

    // Compile shaders
    const unsigned int vertex   = compileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
    const unsigned int fragment = compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);

    // Create shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    // Clean up
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() const { glUseProgram(ID); }

void Shader::setBool(const std::string &name, const bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void Shader::setInt(const std::string &name, const int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, const float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setupLightningUniforms(
    const LightSource &lightSource, const SpotLight &spot_light, const std::vector<LightSource> &pointLights
) const
{
    setVec3("light.position", lightSource.position);
    setVec3("light.color", lightSource.color);
    setFloat("light.constant", lightSource.constant);
    setFloat("light.linear", lightSource.linear);
    setFloat("light.quadratic", lightSource.quadratic);
    setFloat("phongProperties.ambientStrength", BASIC_AMBIENT_STRENGTH);
    setFloat("phongProperties.shininess", BASIC_SHININESS);
    setFloat("phongProperties.specularStrength", BASIC_SPECULAR_STRENGTH);

    setVec3("spotLight.position", spot_light.position);
    setVec3("spotLight.color", spot_light.color);
    setVec3("spotLight.direction", spot_light.direction);
    setFloat("spotLight.cutOff", spot_light.cutOff);
    setFloat("spotLight.outerCutOff", spot_light.outerCutOff);
    setFloat("spotLight.constant", spot_light.constant);
    setFloat("spotLight.linear", spot_light.linear);
    setFloat("spotLight.quadratic", spot_light.quadratic);

    setVec3("fogProperties.fogColor", FOG_COLOR);
    setFloat("fogProperties.fogStart", FOG_START);
    setFloat("fogProperties.fogEnd", FOG_END);

    for (int i = 0; i < NR_POINT_LIGHTS; ++i) {
        setVec3("pointLights[" + std::to_string(i) + "].position", pointLights[i].position);
        setVec3("pointLights[" + std::to_string(i) + "].color", pointLights[i].color);
        setFloat("pointLights[" + std::to_string(i) + "].constant", pointLights[i].constant);
        setFloat("pointLights[" + std::to_string(i) + "].linear", pointLights[i].linear);
        setFloat("pointLights[" + std::to_string(i) + "].quadratic", pointLights[i].quadratic);
    }
}

std::string Shader::readShaderFile(const std::string &filePath)
{
    std::string shaderCode;
    std::ifstream shaderFile;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shaderFile.open(filePath);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        shaderCode = shaderStream.str();
    } catch (std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << '\n';
    }
    return shaderCode;
}

unsigned int Shader::compileShader(const char *shaderCode, const GLenum shaderType)
{
    const unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, nullptr);
    glCompileShader(shader);
    checkCompileErrors(shader, shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
    return shader;
}

void Shader::checkCompileErrors(const unsigned int shader, std::string type)
{
    int success = 0;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
