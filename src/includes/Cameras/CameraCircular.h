//
// Created by michal on 1/30/25.
//

#ifndef CAMERACIRCULAR_H
#define CAMERACIRCULAR_H

#include "Camera.h"

class CameraCircular : public Camera
{
    public:
    CameraCircular(
        glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f), float radius = 10.0f, float speed = 10.0f, float height = 10.0f,
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // Override GetViewMatrix to always look at the target
    glm::mat4 GetViewMatrix() override;

    // Update the camera's position based on time
    void Update(float deltaTime) override;

    private:
    void updatePosition();

    glm::vec3 Target;
    float Radius;
    float Speed;
    float Angle;
    float Height;
};

#endif  // CAMERACIRCULAR_H
