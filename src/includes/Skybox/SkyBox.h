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
    SkyBox(const std::vector<std::string> &faces, const std::vector<float> &skyBoxVertices);
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
    void setup();
};

#endif  // SKYBOX_H
