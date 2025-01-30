//
// Created by michal on 1/23/25.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraType { FPS, CONSTANT, CIRCULAR };

class Camera
{
    public:
    glm::vec3 Position;
    enum Movement { FORWARD, BACKWARD, LEFT, RIGHT };

    //-------------------------------------------------------------------------
    // Constructor and virtual destructor
    //-------------------------------------------------------------------------
    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = -90.0f, float pitch = 0.0f
    );

    virtual ~Camera() = default;

    //-------------------------------------------------------------------------
    // Core camera methods
    //-------------------------------------------------------------------------
    virtual glm::mat4 GetViewMatrix();
    float GetZoom() const;

    //-------------------------------------------------------------------------
    // Methods to be implemented by derived classes
    //-------------------------------------------------------------------------
    virtual void ProcessKeyboard(Movement /*direction*/, float /*deltaTime*/) {}
    virtual void ProcessMouseMovement(float /*xoffset*/, float /*yoffset*/, bool /*constrainPitch*/ = true) {}
    virtual void Update(float /*deltaTime*/) {}

    protected:
    void updateCameraVectors();

    //-------------------------------------------------------------------------
    // Camera Attributes
    //-------------------------------------------------------------------------
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
};

#endif  // CAMERA_H
