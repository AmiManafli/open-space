#include "cg/ui/UserInterface.h"


UserInterface::UserInterface(EntityManager *entityManager, GLContext *context)
        : entityManager(entityManager), context(context) {
    views = { "Perspective", "Top", "Side" };
    currentView = const_cast<char *>(views[0]);

    cameraWindowSize = ImVec2(0, 0);
}

void UserInterface::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (context->displayCursor) {
        renderMainMenu();
    }

    if (context->displayGui) {
        renderSceneInfoWindow();
        renderCameraInfoWindow();
    }

    if (showDemoWindow) {
        ImGui::ShowDemoWindow();
    }

    ImGui::Render();
}

void UserInterface::renderMainMenu() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Quit")) {
                glfwSetWindowShouldClose(context->getWindow(), true);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools")) {
            ImGui::MenuItem("ImGui Demo", nullptr, &showDemoWindow);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void UserInterface::renderSceneInfoWindow() {
    ImGui::SetNextWindowPos(ImVec2(context->getWidth() - 170, 25));
    ImGui::Begin("Scene Information", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);

    ImGui::Text("Frame time: %.3f ms", context->getFrameTime());
    ImGui::Text("Triangles: %d\n", context->triangleCount);
    ImGui::End();
}

void UserInterface::renderCameraInfoWindow() {
    auto cameraEntity = context->getCamera();
    auto transform = entityManager->getTransformComponent(cameraEntity->id);
    auto camera = entityManager->getCameraComponent(cameraEntity->id);

    ImGui::SetNextWindowPos(ImVec2(context->getWidth() - 320, context->getHeight() - 160));
    ImGui::Begin("Camera Information", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);

    if (ImGui::BeginCombo("View", currentView)) {
        for (int n = 0; n < views.size(); n++) {
            bool isSelected = currentView == views[n];
            if (ImGui::Selectable(views[n], isSelected)) {
                currentView = const_cast<char *>(views[n]);
                if (n == 0) {
                    context->setActiveCamera(context->perspectiveCamera);
                } else if (n == 1) {
                    context->setActiveCamera(context->topCamera);
                } else if (n == 2) {
                    context->setActiveCamera(context->sideCamera);
                }
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    auto pos = transform->position;
    auto direction = camera->front;
    ImGui::Text("Position: (x=%.3f, y=%.3f, z=%.3f)", pos.x, pos.y, pos.z);
    ImGui::Text("Direction: (x=%.3f, y=%.3f, z=%.3f)", direction.x, direction.y, direction.z);
    ImGui::Text("Yaw: %.3f deg.", camera->yaw);
    ImGui::Text("Pitch: %.3f deg.", camera->pitch);
    ImGui::Text("Zoom: %.2f x", camera->zoom); ImGui::SameLine();
    if (ImGui::Button("Reset")) {
        camera->zoom = 1.0f;
	}
    ImGui::End();
}
