//
// Created by michal on 2/1/25.
//

#ifndef PROJECTIONORTHOGONAL_H
#define PROJECTIONORTHOGONAL_H

#include "Projection.h"

class OrthogonalProjection : public Projection
{
    public:
    // Constructor
    OrthogonalProjection(int windowWidth, int windowHeight, float zoom, float zNear = 0.1f, float zFar = 100.0f)
        : Projection(windowWidth, windowHeight, zoom, zNear, zFar)
    {
    }

    // Get the orthogonal projection matrix
    glm::mat4 getProjectionMatrix() const override
    {
        float aspectRatio = static_cast<float>(windowWidth) / windowHeight;

        // Compute the boundaries for the orthographic projection
        float left   = -orthoScale * aspectRatio;
        float right  = orthoScale * aspectRatio;
        float bottom = -orthoScale;
        float top    = orthoScale;

        // Create the orthographic projection matrix
        return glm::ortho(left, right, bottom, top, zNear, zFar);
    }

    private:
    // Scale factor for the orthographic view; adjust as needed
    float orthoScale = 1.0f;
};

#endif  // PROJECTIONORTHOGONAL_H
