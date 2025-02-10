//
// Created by michal on 2/10/25.
//

#ifndef IMGUIMENU_H
#define IMGUIMENU_H

#include "CameraManager.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class ImguiMenu
{
    public:
    ImguiMenu(GLFWwindow *window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        io = ImGui::GetIO();

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    ~ImguiMenu()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void DisplayMenu(CameraManager &cameraManager)
    {
        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Render ImGui menu if enabled
        if (showImGuiMenu) {
            ImGui::Begin("Renderer Menu", &showImGuiMenu, ImGuiWindowFlags_AlwaysAutoResize);

            // Camera type selection
            const char *cameraTypes[]    = {"FPS", "Constant", "Circular"};
            static int currentCameraType = 0;

            if (ImGui::Combo("Camera Type", &currentCameraType, cameraTypes, IM_ARRAYSIZE(cameraTypes))) {
                switch (currentCameraType) {
                    case 0:
                        cameraManager.switchCamera(FPS);
                        break;
                    case 1:
                        cameraManager.switchCamera(CONSTANT);
                        break;
                    case 2:
                        cameraManager.switchCamera(CIRCULAR);
                        break;
                }
            }

            // Additional menu options can be added here
            ImGui::Text("Press TAB to toggle this menu");
            ImGui::Text(
                "Current camera position: (%.2f, %.2f, %.2f)", cameraManager.getActiveCamera()->Position.x,
                cameraManager.getActiveCamera()->Position.y, cameraManager.getActiveCamera()->Position.z
            );

            ImGui::End();
        }

        // Render ImGui (with safety checks)
        ImGui::Render();
        ImDrawData *drawData = ImGui::GetDrawData();
        if (drawData) {
            ImGui_ImplOpenGL3_RenderDrawData(drawData);
        }
    }

    void ShowMenu() { showImGuiMenu = true; }

    void HideMenu() { showImGuiMenu = false; }

    bool ShouldShowMenu() const { return showImGuiMenu; }

    void toggleMenu() { showImGuiMenu = !showImGuiMenu; }

    private:
    ImGuiIO io;
    bool showImGuiMenu = false;
};

#endif  // IMGUIMENU_H
