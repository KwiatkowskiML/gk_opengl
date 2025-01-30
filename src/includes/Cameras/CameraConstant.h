//
// Created by michal on 1/30/25.
//

#ifndef CAMERACONSTANT_H
#define CAMERACONSTANT_H

#include "Camera.h"

class CameraConstant : public Camera
{
    public:
    // Constructor with a fixed position looking at the target
    CameraConstant(glm::vec3 position = glm::vec3(0.0f, 5.0f, 10.0f), glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f));

    // Override methods to do nothing
    void ProcessKeyboard(Movement /*direction*/, float /*deltaTime*/) override {}
    void ProcessMouseMovement(float /*xoffset*/, float /*yoffset*/, bool /*constrainPitch*/ = true) override {}

    // Override GetViewMatrix to always look at the target
    glm::mat4 GetViewMatrix() override;

    private:
    glm::vec3 Target;
};

#endif  // CAMERACONSTANT_H
