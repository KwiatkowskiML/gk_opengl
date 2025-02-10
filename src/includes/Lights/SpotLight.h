//
// Created by michal on 2/10/25.
//

#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include <glm/glm.hpp>
#include "LightSource.h"

class SpotLight : public LightSource
{
    public:
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;
};

#endif  // SPOTLIGHT_H
