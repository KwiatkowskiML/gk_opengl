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
    enum Movement { FORWARD, BACKWARD, LEFT, RIGHT };

    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = -90.0f, float pitch = 0.0f
    )
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(2.5f), MouseSensitivity(0.07f), Zoom(45.0f)
    {
        Position = position;
        WorldUp  = up;
        Yaw      = yaw;
        Pitch    = pitch;
        updateCameraVectors();
    }

    // Virtual destructor to ensure proper cleanup of derived classes
    virtual ~Camera() = default;

    // Core camera methods
    virtual glm::mat4 GetViewMatrix() { return glm::lookAt(Position, Position + Front, Up); }
    float GetZoom() const { return Zoom; }

    // Virtual methods that can be overridden by derived classes
    virtual void ProcessKeyboard(Movement /*direction*/, float /*deltaTime*/) {}
    virtual void ProcessMouseMovement(float /*xoffset*/, float /*yoffset*/, bool /*constrainPitch*/ = true) {}
    virtual void Update(float /*deltaTime*/) {}

    protected:
    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front   = glm::normalize(front);

        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up    = glm::normalize(glm::cross(Right, Front));
    }

    glm::vec3 Position;
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

// FPS Camera - inherits from base Camera
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
    void ProcessKeyboard(Movement direction, float deltaTime) override
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

    // Override ProcessMouseMovement to implement mouse look
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) override
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
};

// Constant Camera - inherits from base Camera
class CameraConstant : public Camera
{
    public:
    // Constructor with a fixed position looking at the target
    CameraConstant(
        glm::vec3 position = glm::vec3(0.0f, 5.0f, 10.0f),  // Elevated and pulled back position
        glm::vec3 target   = glm::vec3(0.0f, 0.0f, 0.0f)    // Looking at the origin of the scene
    )
        : Camera(position), Target(target)
    {
        Front = glm::normalize(Target - Position);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up    = glm::normalize(glm::cross(Right, Front));
    }

    // Override methods to do nothing
    void ProcessKeyboard(Movement /*direction*/, float /*deltaTime*/) override {}
    void ProcessMouseMovement(float /*xoffset*/, float /*yoffset*/, bool /*constrainPitch*/ = true) override {}

    // Override GetViewMatrix to always look at the target
    glm::mat4 GetViewMatrix() override { return glm::lookAt(Position, Target, Up); }

    private:
    glm::vec3 Target;  // The point the camera is always looking at
};

// Circular Camera - inherits from base Camera
class CameraCircular : public Camera
{
    public:
    CameraCircular(
        glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f),                   // Target to look at
        float radius     = 10.0f,                                         // Radius of the circular motion
        float speed      = 10.0f,                                         // Speed of rotation
        float height = 10.0f, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)  // Up vector
    )
        : Camera(glm::vec3(radius, 0.0f, 0.0f), up),
          Target(target),
          Radius(radius),
          Speed(speed),
          Angle(0.0f),
          Height(height)
    {
        updatePosition();
    }

    // Override GetViewMatrix to always look at the target
    glm::mat4 GetViewMatrix() override { return glm::lookAt(Position, Target, Up); }

    // Update the camera's position based on time
    void Update(float deltaTime)
    {
        Angle += Speed * deltaTime;  // Update angle based on speed and time
        if (Angle > 360.0f)
            Angle -= 360.0f;
        updatePosition();
    }

    private:
    void updatePosition()
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

    glm::vec3 Target;  // The point the camera always looks at
    float Radius;      // Radius of the circular path
    float Speed;       // Speed of rotation (degrees per second)
    float Angle;       // Current angle in degrees
    float Height;
};

#endif  // CAMERA_H
