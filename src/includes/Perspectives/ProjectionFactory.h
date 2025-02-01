//
// Created by michal on 2/1/25.
//

#ifndef PROJECTIONFACTORY_H
#define PROJECTIONFACTORY_H

#include <memory>
#include "OrthogonalProjection.h"
#include "PerspectiveProjection.h"
#include "Projection.h"

class ProjectionFactory
{
    public:
    ProjectionFactory(
        const int windowWidth, const int windowHeight, const float zoom, const float zNear = 0.1f,
        const float zFar = 100.0f
    )
        : windowWidth(windowWidth), windowHeight(windowHeight), zoom(zoom), zNear(zNear), zFar(zFar)
    {
    }

    virtual ~ProjectionFactory()                           = default;
    virtual std::unique_ptr<Projection> createProjection() = 0;

    protected:
    // Window dimensions
    int windowWidth;
    int windowHeight;

    // Projection properties
    float zoom;
    float zNear;
    float zFar;
};

class OrthogonalProjectionFactory final : public ProjectionFactory
{
    public:
    OrthogonalProjectionFactory(
        const int windowWidth, const int windowHeight, const float zoom, const float zNear = 0.1f,
        const float zFar = 100.0f
    )
        : ProjectionFactory(windowWidth, windowHeight, zoom, zNear, zFar)
    {
    }

    std::unique_ptr<Projection> createProjection() override
    {
        return std::make_unique<OrthogonalProjection>(windowWidth, windowHeight, zoom, zNear, zFar);
    }
};

class PerspectiveProjectionFactory final : public ProjectionFactory
{
    public:
    PerspectiveProjectionFactory(
        const int windowWidth, const int windowHeight, const float zoom, const float zNear = 0.1f,
        const float zFar = 100.0f
    )
        : ProjectionFactory(windowWidth, windowHeight, zoom, zNear, zFar)
    {
    }

    std::unique_ptr<Projection> createProjection() override
    {
        return std::make_unique<PerspectiveProjection>(windowWidth, windowHeight, zoom, zNear, zFar);
    }
};

#endif  // PROJECTIONFACTORY_H
