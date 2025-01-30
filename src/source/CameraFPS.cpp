//
// Created by michal on 1/30/25.
//

#include "../includes/Cameras/CameraFPS.h"

void CameraFPS::ProcessKeyboard(Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    switch (direction) {
        case FORWARD:
            Position += Front * velocity;
            break;
        case BACKWARD:
            Position -= Front * velocity;
            break;
        case LEFT:
            Position -= Right * velocity;
            break;
        case RIGHT:
            Position += Right * velocity;
            break;
    }
}

void CameraFPS::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // Optional pitch constraining
    if (constrainPitch) {
        Pitch = glm::clamp(Pitch, -89.0f, 89.0f);
    }

    // Update camera vectors after movement
    updateCameraVectors();
}
