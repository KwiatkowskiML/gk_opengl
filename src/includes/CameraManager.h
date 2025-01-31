//
// Created by michal on 1/31/25.
//

#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <memory>
#include <unordered_map>
#include "Cameras/Camera.h"
#include "Cameras/CameraFactory.h"

class CameraManager
{
    public:
    //-----------------------------------------------------------------------------------
    // Constructor
    //-----------------------------------------------------------------------------------
    CameraManager();

    //-----------------------------------------------------------------------------------
    // Camera management
    //-----------------------------------------------------------------------------------
    void switchCamera(CameraType type);
    void updateCamera(float deltaTime) const;

    //-----------------------------------------------------------------------------------
    // Input processing
    //-----------------------------------------------------------------------------------
    void processKeyboardInput(Camera::Movement direction, float deltaTime) const;
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) const;

    //-----------------------------------------------------------------------------------
    // Getters
    //-----------------------------------------------------------------------------------
    Camera* getActiveCamera() const { return activeCamera.get(); }
    CameraType getCurrentType() const { return currentType; }
    glm::mat4 getViewMatrix() const;
    float getZoom() const;

    private:
    //-----------------------------------------------------------------------------------
    // Private members
    //-----------------------------------------------------------------------------------
    std::unique_ptr<Camera> activeCamera;
    std::unordered_map<CameraType, std::unique_ptr<CameraFactory>> cameraFactories;
    CameraType currentType;
};

#endif  // CAMERAMANAGER_H
