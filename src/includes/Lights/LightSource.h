//
// Created by michal on 1/30/25.
//

#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <glm/glm.hpp>

class LightSource {
    public:
    glm::vec3 position;
    glm::vec3 color;

    float constant;
    float linear;
    float quadratic;
};

#endif  // LIGHTSOURCE_H
