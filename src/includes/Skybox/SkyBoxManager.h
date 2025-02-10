//
// Created by michal on 2/10/25.
//

#ifndef SKYBOXMANAGER_H
#define SKYBOXMANAGER_H

#include "SkyBox.h"

class SkyBoxManager
{
    public:
    std::unique_ptr<SkyBox> skybox;
    SkyBoxManager(
        const std::vector<std::string> &dayFaces, const std::vector<std::string> &nightFaces,
        const std::vector<float> &skyBoxVertices
    )
        : dayFaces(dayFaces), nightFaces(nightFaces), skyBoxVertices(skyBoxVertices)
    {
        setDaySkybox();
    }

    void setDaySkybox() { skybox = std::make_unique<SkyBox>(dayFaces, skyBoxVertices); }
    void setNightSkybox() { skybox = std::make_unique<SkyBox>(nightFaces, skyBoxVertices); }

    private:
    std::vector<std::string> dayFaces;
    std::vector<std::string> nightFaces;
    std::vector<float> skyBoxVertices;
};

#endif  // SKYBOXMANAGER_H
