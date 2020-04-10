#include "cg/ui/UserInterface.h"


UserInterface::UserInterface(EntityManager *entityManager, GLContext *context)
        : entityManager(entityManager), context(context) {
    views = { "Perspective", "Top", "Side" };
    noiseFunctions = { "Open Simplex", "Perlin" };

    currentView = const_cast<char *>(views[0]);
    currentNoise = const_cast<char *>(noiseFunctions[0]);

    cameraWindowSize = ImVec2(0, 0);

    generateTerrainButtonText = "Generate";

    settings.width = 10;
    settings.height = 10;
    settings.subdivisionWidth = 1;
    settings.subdivisionHeight = 1;
    settings.maxAmplitude = 1.0;
    settings.octaves = 0;
    settings.frequency = 1.0;
    settings.persistence = 1.0;
    settings.seed = 1;
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

    if (showTerrainGeneratorWindow) {
        renderTerrainGeneratorWindow();
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
            ImGui::MenuItem("Terrain Generator", nullptr, &showTerrainGeneratorWindow);
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

void UserInterface::renderTerrainGeneratorWindow() {
    ImGui::Begin("Terrain Generator");

    if (ImGui::BeginCombo("Noise", currentNoise)) {
        for (int n = 0; n < noiseFunctions.size(); n++) {
            bool isSelected = currentNoise == noiseFunctions[n];
            if (ImGui::Selectable(noiseFunctions[n], isSelected)) {
                currentNoise = const_cast<char *>(noiseFunctions[n]);
                settings.noiseType = static_cast<NoiseType>(n);
                updateTerrain(terrain, settings);
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    int minSize = 0;
    int maxSize = 200;

    if (ImGui::SliderScalar("Size", ImGuiDataType_S32, &settings.width, &minSize, &maxSize,"%d")) {
        settings.height = settings.width;
        updateTerrain(terrain, settings);
    }

    int minSubdivisions = 1;
    int maxSubdivisions = 400;

    if (ImGui::SliderScalar("Subdivisions", ImGuiDataType_S32, &settings.subdivisionWidth, &minSubdivisions, &maxSubdivisions,"%d")) {
        settings.subdivisionHeight = settings.subdivisionWidth;
        updateTerrain(terrain, settings);
    }

    int minOctaves = 0;
    int maxOctaves = 32;

    if (ImGui::SliderScalar("Octaves", ImGuiDataType_S32, &settings.octaves, &minOctaves, &maxOctaves,"%d")) {
        updateTerrain(terrain, settings);
    }

    int minSeed = 0;
    int maxSeed = 100000;
    if (ImGui::SliderScalar("Seed", ImGuiDataType_S32, &settings.seed, &minSeed, &maxSeed,"%d")) {
        settings.subdivisionHeight = settings.subdivisionWidth;
        updateTerrain(terrain, settings);
    }
    double minFrequency = 0.0;
    double maxFrequency = 2.0;
    if (ImGui::SliderScalar("Frequency", ImGuiDataType_Double, &settings.frequency, &minFrequency, &maxFrequency,"%f", 1.0f)) {
        updateTerrain(terrain, settings);
    }
    double minPersistence = 0.0;
    double maxPersistence = 2.0;
    if (ImGui::SliderScalar("Persistence", ImGuiDataType_Double, &settings.persistence, &minPersistence, &maxPersistence,"%f", 1.0f)) {
        updateTerrain(terrain, settings);
    }

    double minMaxHeight = 0.0;
    double maxMaxHeight = 30.0;
    if (ImGui::SliderScalar("Max height", ImGuiDataType_Double, &settings.maxAmplitude, &minMaxHeight, &maxMaxHeight, "%f", 1.0f)) {
        updateTerrain(terrain, settings);
    }

    ImGui::End();
}

void UserInterface::onUpdateTerrain(Terrain *terrain, std::function<bool(Terrain *, TerrainSettings& settings)> updateTerrain) {
    this->terrain = terrain;
    this->updateTerrain = updateTerrain;
}
