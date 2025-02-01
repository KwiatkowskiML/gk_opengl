//
// Created by michal on 2/1/25.
//

#ifndef PERSPECTIVEMANAGER_H
#define PERSPECTIVEMANAGER_H

#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>
#include "Projection.h"
#include "ProjectionFactory.h"

enum ProjectionType { PERSPECTIVE, ORTHOGONAL };

class ProjectionManager
{
    public:
    //-----------------------------------------------------------------------------------
    // Constructor
    //-----------------------------------------------------------------------------------
    ProjectionManager(int windowWidth, int windowHeight, float zoom, float zNear = 0.1f, float zFar = 100.0f);

    //-----------------------------------------------------------------------------------
    // Projection management
    //-----------------------------------------------------------------------------------
    void switchProjection(ProjectionType type);

    //-----------------------------------------------------------------------------------
    // Getters
    //-----------------------------------------------------------------------------------
    glm::mat4 getProjectionMatrix() const;

    private:
    //-----------------------------------------------------------------------------------
    // Private members
    //-----------------------------------------------------------------------------------
    std::unique_ptr<Projection> currentProjection;
    std::unordered_map<ProjectionType, std::unique_ptr<ProjectionFactory>> projectionFactories;
    ProjectionType currentType;
};

#endif  // PERSPECTIVEMANAGER_H
