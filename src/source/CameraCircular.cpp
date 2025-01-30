//
// Created by michal on 1/30/25.
//

#include "../includes/Cameras/CameraCircular.h"

CameraCircular::CameraCircular(glm::vec3 target, float radius, float speed, float height, glm::vec3 up)
    : Camera(glm::vec3(radius, 0.0f, 0.0f), up),
      Target(target),
      Radius(radius),
      Speed(speed),
      Angle(0.0f),
      Height(height)
{
    updatePosition();
}

glm::mat4 CameraCircular::GetViewMatrix() { return glm::lookAt(Position, Target, Up); }

void CameraCircular::Update(float deltaTime)
{
    Angle += Speed * deltaTime;  // Update angle based on speed and time
    if (Angle > 360.0f)
        Angle -= 360.0f;
    updatePosition();
}

void CameraCircular::updatePosition()
{
    // Calculate the new position in the circular trajectory
    Position.x = Target.x + Radius * cos(glm::radians(Angle));
    Position.z = Target.z + Radius * sin(glm::radians(Angle));
    Position.y = Target.y + Height;  // Keep the height constant

    // Update the Front vector to always look at the target
    Front = glm::normalize(Target - Position);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up    = glm::normalize(glm::cross(Right, Front));
}
