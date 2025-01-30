//
// Created by michal on 1/30/25.
//

#ifndef CAMERAFPS_H
#define CAMERAFPS_H

#include "Camera.h"

class CameraFPS : public Camera
{
    public:
    // Constructor with default values matching the base camera
    CameraFPS(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = -90.0f, float pitch = 0.0f
    )
        : Camera(position, up, yaw, pitch)
    {
    }

    // Override ProcessKeyboard to implement FPS-style movement
    void ProcessKeyboard(Movement direction, float deltaTime) override;

    // Override ProcessMouseMovement to implement mouse look
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) override;
};

#endif  // CAMERAFPS_H
