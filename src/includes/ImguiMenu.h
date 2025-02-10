//
// Created by michal on 2/10/25.
//

#ifndef IMGUIMENU_H
#define IMGUIMENU_H

#include "CameraManager.h"
#include "Models/Flashlight.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Perspectives/ProjectionManager.h"
#include "Skybox/SkyBoxManager.h"

class ImguiMenu
{
    public:
    //-------------------------------------------------------------------------
    // Constructor and destructor
    //-------------------------------------------------------------------------
    ImguiMenu(GLFWwindow *window);
    ~ImguiMenu();

    //-------------------------------------------------------------------------
    // Basic functionalities
    //-------------------------------------------------------------------------
    void DisplayMenu(
        CameraManager &cameraManager, ProjectionManager &projectionManager, SkyBoxManager &skyboxManager,
        Flashlight *flashlightModel
    );

    //-------------------------------------------------------------------------
    // Menu visibility
    //-------------------------------------------------------------------------
    void ShowMenu() { showImGuiMenu = true; }
    void HideMenu() { showImGuiMenu = false; }
    void toggleMenu() { showImGuiMenu = !showImGuiMenu; }
    bool ShouldShowMenu() const { return showImGuiMenu; }

    private:
    //-------------------------------------------------------------------------
    // Private members
    //-------------------------------------------------------------------------
    ImGuiIO io;
    bool showImGuiMenu = false;
};

#endif  // IMGUIMENU_H
