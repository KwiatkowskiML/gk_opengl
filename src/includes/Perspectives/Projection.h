//
// Created by michal on 2/1/25.
//

#ifndef PROJECTION_H
#define PROJECTION_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Projection
{
    public:
    // Constructor
    Projection(int windowWidth, int windowHeight, float zoom, float zNear = 0.1f, float zFar = 100.0f)
        : windowWidth(windowWidth), windowHeight(windowHeight), zoom(zoom), zNear(zNear), zFar(zFar)
    {
    }

    // Virtual destructor
    virtual ~Projection() = default;

    // Get the projection matrix
    virtual glm::mat4 getProjectionMatrix() const = 0;

    protected:
    // Window dimensions
    int windowWidth;
    int windowHeight;

    // Projection properties
    float zoom;
    float zNear;
    float zFar;
};

#endif  // PROJECTION_H
