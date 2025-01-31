//
// Created by michal on 1/31/25.
//

#ifndef MODEL_H
#define MODEL_H

#include <glm/glm.hpp>
#include <vector>

class Model
{
    public:
    unsigned int VAO;
    unsigned int VBO;
    std::vector<float> vertices;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 color;
};

#endif  // MODEL_H
