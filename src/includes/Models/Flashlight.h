//
// Created by michal on 2/9/25.
//

#ifndef FLASHLIGHT_H
#define FLASHLIGHT_H

#include "includes/Cameras/Camera.h"
#include "includes/Constants.h"
#include "includes/Lights/SpotLight.h"
#include "includes/Models/Model.h"

class Flashlight : public NewModel
{
    public:
    // Flashlight properties
    SpotLight spotLight;
    glm::vec3 position;

    //-----------------------------------------------------------------------------------
    // Constructor
    //-----------------------------------------------------------------------------------
    Flashlight(string const &path, unsigned int flags) : NewModel(path, flags), position(INITIAL_FLASHLIGHT_POSITION)
    {
        spotLight.position = INITIAL_FLASHLIGHT_POSITION;
        spotLight.color    = glm::vec3(1.0f, 1.0f, 1.0f);

        spotLight.constant  = 1.0f;
        spotLight.linear    = 0.09f;
        spotLight.quadratic = 0.032f;

        spotLight.direction   = glm::vec3(0.0f, 0.0f, -1.0f);
        spotLight.cutOff      = glm::cos(glm::radians(12.5f));
        spotLight.outerCutOff = glm::cos(glm::radians(15.0f));
    }

    //-----------------------------------------------------------------------------------
    // Draw the flashlight model
    //-----------------------------------------------------------------------------------
    void Draw(Shader &shader) const { NewModel::Draw(shader); }

    // Get model matrix
    glm::mat4 getModelMatrix() const
    {
        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::translate(model, position);
        model           = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model           = glm::scale(model, FLASHLIGHT_SCALE);
        return model;
    }

    glm::mat4 getModelMatrix2(const Camera &camera) const
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
