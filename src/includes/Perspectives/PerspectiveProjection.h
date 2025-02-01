//
// Created by michal on 2/1/25.
//

#ifndef PERSPECTIVEPROJECTION_H
#define PERSPECTIVEPROJECTION_H

class PerspectiveProjection : public Projection
{
    public:
    // Constructor
    PerspectiveProjection(int windowWidth, int windowHeight, float zoom, float zNear = 0.1f, float zFar = 100.0f)
        : Projection(windowWidth, windowHeight, zoom, zNear, zFar)
    {
    }

    // Get the perspective projection matrix
    glm::mat4 getProjectionMatrix() const override
    {
        return glm::perspective(
            glm::radians(zoom), static_cast<float>(windowWidth) / static_cast<float>(windowHeight), zNear, zFar
        );
    }
};

#endif  // PERSPECTIVEPROJECTION_H
