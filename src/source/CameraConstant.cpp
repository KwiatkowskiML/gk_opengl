//
// Created by michal on 1/30/25.
//

#include "../includes/Cameras/CameraConstant.h"

CameraConstant::CameraConstant(glm::vec3 position, glm::vec3 target) : Camera(position), Target(target)
{
    Front = glm::normalize(Target - Position);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up    = glm::normalize(glm::cross(Right, Front));
}

glm::mat4 CameraConstant::GetViewMatrix() { return glm::lookAt(Position, Target, Up); }
