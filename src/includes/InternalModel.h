//
// Created by michal on 1/31/25.
//

#ifndef INTER_MODEL_H
#define INTER_MODEL_H

#include <glm/glm.hpp>
#include <vector>

class InternalModel
{
    public:
    unsigned int VAO;
    unsigned int VBO;
    std::vector<float> vertices;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 color;
};

#endif  // INTER_MODEL_H
