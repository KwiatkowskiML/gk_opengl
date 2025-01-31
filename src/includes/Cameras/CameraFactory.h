//
// Created by michal on 1/31/25.
//

#ifndef CAMERAFACTORY_H
#define CAMERAFACTORY_H

#include <memory>
#include "Camera.h"
#include "CameraCircular.h"
#include "CameraConstant.h"
#include "CameraFPS.h"
#include "includes/Constants.h"

class CameraFactory
{
    public:
    virtual ~CameraFactory()                       = default;
    virtual std::unique_ptr<Camera> createCamera() = 0;
};

class FPSCameraFactory : public CameraFactory
{
    public:
    std::unique_ptr<Camera> createCamera() override { return std::make_unique<CameraFPS>(INITIAL_FPS_CAMERA_POSITION); }
};

class ConstantCameraFactory : public CameraFactory
{
    public:
    std::unique_ptr<Camera> createCamera() override
    {
        return std::make_unique<CameraConstant>(INITIAL_CONSTANT_CAMERA_POSITION, CAMERA_TARGET_POSITION);
    }
};

class CircularCameraFactory : public CameraFactory
{
    public:
    std::unique_ptr<Camera> createCamera() override { return std::make_unique<CameraCircular>(CAMERA_TARGET_POSITION); }
};

#endif  // CAMERAFACTORY_H
