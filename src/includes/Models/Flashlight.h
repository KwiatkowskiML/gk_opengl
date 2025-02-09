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

        // Calculate camera orientation vectors
        glm::vec3 cameraRight = glm::normalize(glm::cross(camera.Front, camera.WorldUp));
        glm::vec3 cameraUp    = glm::normalize(glm::cross(cameraRight, camera.Front));

        // Create rotation matrix based on camera orientation
        glm::mat4 rotationMatrix = glm::mat4(
            glm::vec4(cameraRight, 0.0f), glm::vec4(cameraUp, 0.0f),
            glm::vec4(-camera.Front, 0.0f),  // Negative because OpenGL is right-handed
            glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
        );

        // Calculate the pivot point (the light end of the flashlight)
        // This should be adjusted based on your model's dimensions
        glm::vec3 pivotOffset = glm::vec3(0.0f, 0.0f, 100 * 0.5f);  // Adjust FLASHLIGHT_LENGTH as needed

        // 1. Translate to camera position + base offset
        modelMatrix = glm::translate(modelMatrix, camera.Position + FLASHLIGHT_SHIFT);

        // 2. Apply rotation
        modelMatrix = modelMatrix * rotationMatrix;

        // 3. Apply scale
        modelMatrix = glm::scale(modelMatrix, FLASHLIGHT_SCALE);

        // 4. Apply fixed model rotation
        modelMatrix = glm::rotate(modelMatrix, glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));

        // 5. Adjust position to maintain pivot point
        // Translate to pivot point, rotate, then translate back
        modelMatrix = glm::translate(modelMatrix, pivotOffset);
        modelMatrix = glm::translate(modelMatrix, -pivotOffset);

        return modelMatrix;
    }
};

#endif  // FLASHLIGHT_H
