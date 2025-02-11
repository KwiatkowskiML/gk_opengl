//
// Created by michal on 2/9/25.
//

#ifndef FLASHLIGHT_H
#define FLASHLIGHT_H

#include "includes/Cameras/Camera.h"
#include "includes/Constants.h"
#include "includes/Lights/SpotLight.h"
#include "includes/Models/Model.h"

class Flashlight : public NewModel
{
    public:
    // Flashlight properties
    SpotLight spotLight;
    bool shouldMove = true;

    //-----------------------------------------------------------------------------------
    // Constructor
    //-----------------------------------------------------------------------------------
    Flashlight(string const &path, unsigned int flags);

    //-----------------------------------------------------------------------------------
    // Draw the flashlight model
    //-----------------------------------------------------------------------------------
    void Draw(Shader &shader) const { NewModel::Draw(shader); }

    //-----------------------------------------------------------------------------------
    // Getters
    //-----------------------------------------------------------------------------------
    glm::mat4 getModelMatrix() const;
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getRotation() const { return rotation; }

    //-----------------------------------------------------------------------------------
    // Setters
    //-----------------------------------------------------------------------------------
    void setPosition(const glm::vec3 &newPosition)
    {
        position           = newPosition;
        spotLight.position = newPosition;
    }

    void setRotation(const glm::vec3 &newRotation)
    {
        rotation = newRotation;
        updateSpotLightDirection();
    }

    //-----------------------------------------------------------------------------------
    // Update the flashlight position
    //-----------------------------------------------------------------------------------
    void updateFlashLightPosition(const float &deltaTime);

    private:
    //-----------------------------------------------------------------------------------
    // Private members
    //-----------------------------------------------------------------------------------
    glm::vec3 position;
    glm::vec3 rotation;
    float movementTime;
    glm::vec3 initialPosition;

    void updateSpotLightDirection();
};

#endif  // FLASHLIGHT_H
