//
// Created by michal on 2/7/25.
//

#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

#include "includes/Shader.h"

using namespace std;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh
{
    public:
    //-----------------------------------------------------------------------------------
    // Mesh Data
    //-----------------------------------------------------------------------------------
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    //-----------------------------------------------------------------------------------
    // Constructor
    //-----------------------------------------------------------------------------------
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

    //-----------------------------------------------------------------------------------
    // Render the mesh
    //-----------------------------------------------------------------------------------
    void Draw(Shader &shader) const;

    private:
    //-----------------------------------------------------------------------------------
    // Render data
    //-----------------------------------------------------------------------------------
    unsigned int VAO, VBO, EBO;

    //-----------------------------------------------------------------------------------
    // Mesh initialization
    //-----------------------------------------------------------------------------------
    void setupMesh();
};

#endif  // MESH_H
