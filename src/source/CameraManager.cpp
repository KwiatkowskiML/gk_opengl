//
// Created by michal on 1/31/25.
//

#include "includes/CameraManager.h"
#include <stdexcept>

CameraManager::CameraManager()
{
    cameraFactories[CameraType::FPS]      = std::make_unique<FPSCameraFactory>();
    cameraFactories[CameraType::CONSTANT] = std::make_unique<ConstantCameraFactory>();
    cameraFactories[CameraType::CIRCULAR] = std::make_unique<CircularCameraFactory>();

    activeCamera = cameraFactories[CameraType::FPS]->createCamera();
    currentType  = CameraType::FPS;
}

void CameraManager::switchCamera(CameraType type)
{
    auto factoryIt = cameraFactories.find(type);
    if (factoryIt == cameraFactories.end()) {
        throw std::runtime_error("Invalid camera type");
    }

    if (type != currentType) {
        activeCamera = factoryIt->second->createCamera();
        currentType  = type;
    }
}

void CameraManager::updateCamera(float deltaTime) const
{
    if (activeCamera) {
        if (currentType == CameraType::CIRCULAR) {
            activeCamera->Update(deltaTime);
        }
    }
}

void CameraManager::processKeyboardInput(Camera::Movement direction, float deltaTime) const
{
    if (activeCamera) {
        activeCamera->ProcessKeyboard(direction, deltaTime);
    }
}

void CameraManager::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) const
{
    if (activeCamera) {
        activeCamera->ProcessMouseMovement(xoffset, yoffset, constrainPitch);
    }
}

glm::mat4 CameraManager::getViewMatrix() const
{
    if (activeCamera) {
        return activeCamera->GetViewMatrix();
    }
    return glm::mat4(1.0f);
}

float CameraManager::getZoom() const
{
    if (activeCamera) {
        return activeCamera->GetZoom();
    }
    return 45.0f;
}
