//
// Created by michal on 2/9/25.
//

#ifndef FLASHLIGHT_H
#define FLASHLIGHT_H

#include "includes/Cameras/Camera.h"
#include "includes/Constants.h"
#include "includes/Models/Model.h"

class Flashlight : public NewModel
{
    public:
    //-----------------------------------------------------------------------------------
    // Constructor
    //-----------------------------------------------------------------------------------
    Flashlight(string const &path, unsigned int flags) : NewModel(path, flags) {}

    //-----------------------------------------------------------------------------------
    // Draw the flashlight model
    //-----------------------------------------------------------------------------------
    void Draw(Shader &shader) const { NewModel::Draw(shader); }

    glm::mat4 getModelMatrix(const Camera &camera) const
    {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix           = glm::translate(modelMatrix, camera.Position + FLASHLIGHT_SHIFT);
        modelMatrix           = glm::scale(modelMatrix, FLASHLIGHT_SCALE);
        modelMatrix           = glm::rotate(modelMatrix, glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
        return modelMatrix;
    }
};

#endif  // FLASHLIGHT_H
