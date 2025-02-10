//
// Created by michal on 2/9/25.
//

#ifndef SKYBOX_H
#define SKYBOX_H

#include <glad/glad.h>
#include <stb_image.h>

#include <iostream>
#include <string>
#include <vector>

class SkyBox
{
    public:
    //-----------------------------------------------------------------------------------
    // VAO and VBO for the skybox
    //-----------------------------------------------------------------------------------
    unsigned int skyboxVAO, skyboxVBO;

    //-----------------------------------------------------------------------------------
    // Constructor and destructor
    //-----------------------------------------------------------------------------------
    SkyBox(const std::vector<std::string> &faces, const std::vector<float> &skyBoxVertices) : faces(faces)
    {
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, skyBoxVertices.size() * sizeof(float), skyBoxVertices.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(nullptr));
        setup();
    };
    ~SkyBox() = default;

    //-----------------------------------------------------------------------------------
    // Get the skybox texture
    //-----------------------------------------------------------------------------------
    unsigned int getTextureID() const { return textureID; }

    private:
    //-----------------------------------------------------------------------------------
    // Skybox details
    //-----------------------------------------------------------------------------------
    std::vector<std::string> faces;
    unsigned int textureID;

    //-----------------------------------------------------------------------------------
    // Setup the skybox
    //-----------------------------------------------------------------------------------
    void setup()
    {
        // Binding texture
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        // Generating the texture of each face
        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++) {
            if (unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0)) {
                GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
                glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data
                );
                stbi_image_free(data);
            } else {
                std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }

        // Setting each face's parameters
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }
};

#endif  // SKYBOX_H
