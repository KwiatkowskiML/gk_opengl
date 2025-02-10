//
// Created by michal on 2/10/25.
//

#include "includes/ImguiMenu.h"

ImguiMenu::ImguiMenu(GLFWwindow *window)
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

ImguiMenu::~ImguiMenu()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
void ImguiMenu::DisplayMenu(CameraManager &cameraManager, ProjectionManager &projectionManager)
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

        const char *projectionTypes[]    = {"PERSPECTIVE", "ORTHOGONAL"};
        static int currentProjectionType = 0;

        if (ImGui::Combo("Projection Type", &currentProjectionType, projectionTypes, IM_ARRAYSIZE(projectionTypes))) {
            switch (currentProjectionType) {
                case 0:
                    projectionManager.switchProjection(PERSPECTIVE);
                    break;
                case 1:
                    projectionManager.switchProjection(ORTHOGONAL);
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
