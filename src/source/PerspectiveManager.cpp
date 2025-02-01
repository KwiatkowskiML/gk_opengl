//
// Created by michal on 2/1/25.
//

#include "includes/Perspectives/ProjectionFactory.h"
#include "includes/Perspectives/ProjectionManager.h"

ProjectionManager::ProjectionManager(int windowWidth, int windowHeight, float zoom, float zNear, float zFar)
{
    projectionFactories[ProjectionType::PERSPECTIVE] =
        std::make_unique<PerspectiveProjectionFactory>(windowWidth, windowHeight, zoom, zNear, zFar);

    projectionFactories[ProjectionType::ORTHOGONAL] =
        std::make_unique<OrthogonalProjectionFactory>(windowWidth, windowHeight, zoom, zNear, zFar);

    currentProjection = projectionFactories[ProjectionType::PERSPECTIVE]->createProjection();
    currentType       = ProjectionType::PERSPECTIVE;
}

void ProjectionManager::switchProjection(ProjectionType type)
{
    auto factoryIt = projectionFactories.find(type);
    if (factoryIt == projectionFactories.end()) {
        throw std::runtime_error("Invalid projection type");
    }

    if (type != currentType) {
        currentProjection = factoryIt->second->createProjection();
        currentType       = type;
    }
}

glm::mat4 ProjectionManager::getProjectionMatrix() const { return currentProjection->getProjectionMatrix(); }
