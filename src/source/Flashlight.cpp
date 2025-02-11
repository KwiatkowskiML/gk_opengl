//
// Created by michal on 2/9/25.
//

#include "includes/Models/Flashlight.h"

Flashlight::Flashlight(string const &path, unsigned int flags)
    : NewModel(path, flags),
      position(INITIAL_FLASHLIGHT_POSITION),
      rotation(glm::vec3(0.0f, 0.0f, 0.0f)),
      initialPosition(INITIAL_FLASHLIGHT_POSITION)
{
    movementTime = 0.0f;

    spotLight.position = INITIAL_FLASHLIGHT_POSITION;
    spotLight.color    = glm::vec3(1.0f, 1.0f, 1.0f);

    spotLight.constant  = 1.0f;
    spotLight.linear    = 0.09f;
    spotLight.quadratic = 0.032f;

    spotLight.direction   = glm::vec3(0.0f, 0.0f, -1.0f);
    spotLight.cutOff      = glm::cos(glm::radians(12.5f));
    spotLight.outerCutOff = glm::cos(glm::radians(15.0f));
}

glm::mat4 Flashlight::getModelMatrix() const
{
    glm::mat4 model = glm::mat4(1.0f);

    // Translate the model to the flashlight position
    model = glm::translate(model, position);

    // Rotate the model to face the same direction as the camera
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // Apply y and x rotations
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

    // scale the model
    model = glm::scale(model, FLASHLIGHT_SCALE);
    return model;
}

void Flashlight::updateFlashLightPosition(const float &deltaTime)
{
    if (!shouldMove)
        return;

    // Update movement time
    movementTime += deltaTime;

    // Calculate the new z offset
    float speedFactor       = 0.5f;
    float movementAmplitude = 2.0f;
    float zOffset           = sin(movementTime * speedFactor) * movementAmplitude;

    // Calculate the rotation
    float rotationAmplitude = 25.0f;
    float yRotation         = rotationAmplitude * sin(movementTime);
    float xRotation         = rotationAmplitude * cos(movementTime);

    // Setup new position and rotation
    glm::vec3 newPosition = initialPosition;
    newPosition.z += zOffset;
    setPosition(newPosition);

    glm::vec3 newRotation = rotation;
    newRotation.y         = yRotation;
    newRotation.x         = xRotation;
    setRotation(newRotation);
}

void Flashlight::updateSpotLightDirection()
{
    // Start with base direction
    glm::vec3 baseDirection = glm::vec3(0.0f, 0.0f, -1.0f);

    // Create rotation matrices
    glm::mat4 rotationMatrix = glm::mat4(1.0f);

    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

    // Transform the base direction by the rotation matrix
    glm::vec4 rotatedDirection = rotationMatrix * glm::vec4(baseDirection, 0.0f);

    // Update spotlight direction
    spotLight.direction = glm::normalize(glm::vec3(rotatedDirection));
}
