//
// Created by michal on 1/30/25.
//

#include "../includes/Cameras/CameraConstant.h"

glm::mat4 CameraConstant::GetViewMatrix() { return glm::lookAt(Position, Target, Up); }
