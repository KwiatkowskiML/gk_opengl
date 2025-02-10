//
// Created by michal on 2/10/25.
//

#include "includes/ImguiMenu.h"

#include <includes/Models/Flashlight.h>

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
void ImguiMenu::DisplayMenu(
    CameraManager &cameraManager, ProjectionManager &projectionManager, SkyBoxManager &skyBoxManager,
    Flashlight *flashlight
)
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

        const char *skyboxTypes[]    = {"DAY", "NIGHT"};
        static int currentSkyboxType = 0;

        if (ImGui::Combo("Skybox Type", &currentSkyboxType, skyboxTypes, IM_ARRAYSIZE(skyboxTypes))) {
            switch (currentSkyboxType) {
                case 0:
                    skyBoxManager.setDaySkybox();
                    break;
                case 1:
                    skyBoxManager.setNightSkybox();
                    break;
            }
        }

        // Flashlight Position Controls
        ImGui::Separator();
        ImGui::Text("Flashlight Position");

        // Create temporary variables to hold the position values
        glm::vec3 tempPosition = flashlight->position;

        // Add sliders for X, Y, and Z positions
        if (ImGui::SliderFloat("X Position", &tempPosition.x, -10.0f, 10.0f, "%.2f") ||
            ImGui::SliderFloat("Y Position", &tempPosition.y, -10.0f, 10.0f, "%.2f") ||
            ImGui::SliderFloat("Z Position", &tempPosition.z, -10.0f, 10.0f, "%.2f")) {
            // Update both the flashlight position and its spotlight position
            flashlight->position           = tempPosition;
            flashlight->spotLight.position = tempPosition;
        }

        // Display current position values
        ImGui::Text(
            "Current flashlight position: (%.2f, %.2f, %.2f)", flashlight->position.x, flashlight->position.y,
            flashlight->position.z
        );

        ImGui::Separator();

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
