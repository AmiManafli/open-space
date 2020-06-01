#include "cg/ui/UserInterface.h"
#include <json.hpp>
#include <fstream>
#include <string>
#include <iomanip>
#include <filesystem>

using json = nlohmann::json;


UserInterface::UserInterface(EntityManager *entityManager, GLContext *context)
        : entityManager(entityManager), context(context), settingsFilename("settings.json"), terrainProfileName("") {
    views = {"Spaceship", "Perspective", "Top", "Side"};
    noiseFunctions = {"Open Simplex", "Perlin", "Test"};

    loadTerrainProfiles();

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
    settings.redistribution = 1.0;

    entityWindow = new EntityWindow(entityManager, context);
    planetEditor = new PlanetEditor(*entityManager, *context);
    skyboxEditor = new SkyboxEditor(*entityManager, *context);
}

UserInterface::~UserInterface() {
    delete entityWindow;
    delete planetEditor;
    delete skyboxEditor;
}

void UserInterface::init() {
    ImGuiIO& io = ImGui::GetIO();

    auto lightFontPath = std::filesystem::absolute("./assets/fonts/Roboto-Light.ttf");
    auto thinFontPath = std::filesystem::absolute("./assets/fonts/Roboto-Thin.ttf");
    auto regularFontPath = std::filesystem::absolute("./assets/fonts/Roboto-Regular.ttf");
    auto blackFontPath = std::filesystem::absolute("./assets/fonts/Roboto-Black.ttf");

    context->uiRegularFont = io.Fonts->AddFontFromFileTTF(regularFontPath.string().c_str(), 13.0f);
    context->uiBlackFont = io.Fonts->AddFontFromFileTTF(blackFontPath.string().c_str(), 13.0f);
    context->uiEntityTitleFont = io.Fonts->AddFontFromFileTTF(blackFontPath.string().c_str(), 14.0f);
    context->uiEntityFont = io.Fonts->AddFontFromFileTTF(lightFontPath.string().c_str(), 13.0f);

    setupTheme(true, 0.9);
}

void UserInterface::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::PushFont(context->uiEntityFont);

    renderSpaceDisplay();
    renderAppInfoDisplay();

    if (context->showEntityNames) {
        renderEntityNames();
    }

    ImGui::PopFont();

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

    if (context->displayCursor && showPlanetEditor) {
        planetEditor->render();
    }

    if (showTerrainGeneratorWindow) {
        renderTerrainGeneratorWindow();
    }

    if (showSkyboxEditor) {
        skyboxEditor->render();
    }

    if (showEntityWindow) {
        entityWindow->render();
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
            ImGui::MenuItem("Entity Window", nullptr, &showEntityWindow);
            ImGui::MenuItem("Planet Editor", nullptr, &showPlanetEditor);
            ImGui::MenuItem("Skybox Editor", nullptr, &showSkyboxEditor);
            ImGui::MenuItem("ImGui Demo", nullptr, &showDemoWindow);
            if (ImGui::MenuItem("Turn on/offc wireframe mode")) {
                showWireframe = !showWireframe;
                if (showWireframe) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                } else {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
            }
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
    auto transform = entityManager->getComponent<TransformComponent>(cameraEntity);
    auto camera = entityManager->getComponent<CameraComponent>(cameraEntity);

    ImGui::SetNextWindowPos(ImVec2(context->getWidth() - 320, context->getHeight() - 160));
    ImGui::Begin("Camera Information", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);

    if (ImGui::BeginCombo("View", currentView)) {
        for (int n = 0; n < views.size(); n++) {
            bool isSelected = currentView == views[n];
            if (ImGui::Selectable(views[n], isSelected)) {
                currentView = const_cast<char *>(views[n]);
                if (n == 0) {
                    context->setActiveCamera(context->spaceshipCamera);
                } else if (n == 1) {
                    context->setActiveCamera(context->perspectiveCamera);
                } else if (n == 2) {
                    context->setActiveCamera(context->topCamera);
                } else if (n == 3) {
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
    ImGui::Text("Zoom: %.2f x", camera->zoom);
    ImGui::SameLine();
    if (ImGui::Button("Reset")) {
        camera->zoom = 1.0f;
    }
    ImGui::End();
}

void UserInterface::renderTerrainGeneratorWindow() {
    ImGui::Begin("Terrain Generator");

    ImGui::InputText("Profile name", terrainProfileName, IM_ARRAYSIZE(terrainProfileName));

    if (ImGui::BeginCombo("Profiles", currentProfile)) {
        for (auto &profile : availableProfiles) {
            auto profileStr = profile.c_str();
            bool isSelected = currentProfile == profileStr;
            if (ImGui::Selectable(profileStr, isSelected)) {
                strcpy(currentProfile, profileStr);
                strcpy(terrainProfileName, currentProfile);
                loadTerrainSettings();
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    ImGui::Separator();

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

    if (ImGui::SliderScalar("Size", ImGuiDataType_S32, &settings.width, &minSize, &maxSize, "%d")) {
        settings.height = settings.width;
        updateTerrain(terrain, settings);
    }

    int minSubdivisions = 1;
    int maxSubdivisions = 400;

    if (ImGui::SliderScalar("Subdivisions", ImGuiDataType_S32, &settings.subdivisionWidth, &minSubdivisions,
                            &maxSubdivisions, "%d")) {
        settings.subdivisionHeight = settings.subdivisionWidth;
        updateTerrain(terrain, settings);
    }

    int minOctaves = 0;
    int maxOctaves = 32;

    if (ImGui::SliderScalar("Octaves", ImGuiDataType_S32, &settings.octaves, &minOctaves, &maxOctaves, "%d")) {
        updateTerrain(terrain, settings);
    }

    int minSeed = 0;
    int maxSeed = 100000;
    if (ImGui::SliderScalar("Seed", ImGuiDataType_S32, &settings.seed, &minSeed, &maxSeed, "%d")) {
        settings.subdivisionHeight = settings.subdivisionWidth;
        updateTerrain(terrain, settings);
    }
    double minFrequency = 0.0;
    double maxFrequency = 2.0;
    if (ImGui::SliderScalar("Frequency", ImGuiDataType_Double, &settings.frequency, &minFrequency, &maxFrequency, "%f",
                            1.0f)) {
        updateTerrain(terrain, settings);
    }
    double minPersistence = 0.0;
    double maxPersistence = 2.0;
    if (ImGui::SliderScalar("Persistence", ImGuiDataType_Double, &settings.persistence, &minPersistence,
                            &maxPersistence, "%f", 1.0f)) {
        updateTerrain(terrain, settings);
    }

    double minRedistribution = 0.0;
    double maxRedistribution = 10.0;
    if (ImGui::InputDouble("Redistribution", &settings.redistribution, 0.1, 0.5)) {
        updateTerrain(terrain, settings);
    }

    double minMaxHeight = 0.0;
    double maxMaxHeight = 30.0;
    if (ImGui::SliderScalar("Max height", ImGuiDataType_Double, &settings.maxAmplitude, &minMaxHeight, &maxMaxHeight,
                            "%f", 1.0f)) {
        updateTerrain(terrain, settings);
    }

    if (ImGui::Button("Save settings")) {
        saveTerrainSettings();
    }

    ImGui::End();
}

void UserInterface::onUpdateTerrain(Terrain *terrain,
                                    std::function<bool(Terrain *, TerrainSettings &settings)> updateTerrainFunc) {
    this->terrain = terrain;
    this->updateTerrainFunc = updateTerrainFunc;
}

void UserInterface::loadTerrainSettings() {
    json jsonSettings;

    std::ifstream settingsFile(settingsFilename.c_str());
    if (settingsFile.is_open()) {
        settingsFile >> jsonSettings;
        settingsFile.close();
    } else {
        printf("Error: Failed to load \"%s\"\n", settingsFilename.c_str());
        return;
    }

    if (!(jsonSettings.contains("terrain") && \
            jsonSettings["terrain"].contains("profiles") && \
            jsonSettings["terrain"]["profiles"].contains(terrainProfileName))) {
        printf("No terrain profile with name: %s\n", terrainProfileName);
        return;
    }

    auto profile = jsonSettings["terrain"]["profiles"][terrainProfileName];
    settings.width = profile["width"];
    settings.height = profile["height"];
    settings.subdivisionWidth = profile["subdivisionWidth"];
    settings.subdivisionHeight = profile["subdivisionHeight"];
    settings.maxAmplitude = profile["maxAmplitude"];
    settings.frequency = profile["frequency"];
    settings.octaves = profile["octaves"];
    settings.persistence = profile["persistence"];
    settings.seed = profile["seed"];
    settings.redistribution = profile["redistribution"];

    settings.noiseType = profile["noiseType"].get<NoiseType>();
    currentNoise = const_cast<char *>(noiseFunctions[static_cast<int>(settings.noiseType)]);

    updateTerrain(terrain, settings);

    printf("Loaded terrain settings for profile \"%s\"\n", terrainProfileName);
}

void UserInterface::saveTerrainSettings() {
    json jsonSettings;

    std::ifstream settingsFile(settingsFilename.c_str());

    if (settingsFile.is_open()) {
        settingsFile >> jsonSettings;
        settingsFile.close();
    } else {
        jsonSettings["terrain"] = {
                {"profiles", {}}
        };
    }

    auto profile = json::object();
    profile["width"] = settings.width;
    profile["height"] = settings.height;
    profile["subdivisionWidth"] = settings.subdivisionWidth;
    profile["subdivisionHeight"] = settings.subdivisionHeight;
    profile["maxAmplitude"] = settings.maxAmplitude;
    profile["frequency"] = settings.frequency;
    profile["octaves"] = settings.octaves;
    profile["persistence"] = settings.persistence;
    profile["seed"] = settings.seed;
    profile["noiseType"] = settings.noiseType;
    profile["redistribution"] = settings.redistribution;

    jsonSettings["terrain"]["profiles"][terrainProfileName] = profile;

    std::ofstream outFile(settingsFilename);
    outFile << std::setw(4) << jsonSettings << std::endl;

    printf("Saved terrain settings to \"%s\"\n", settingsFilename.c_str());

    availableProfiles.emplace_back(terrainProfileName);
    strcpy(currentProfile, terrainProfileName);
}

void UserInterface::loadTerrainProfiles() {
    availableProfiles.clear();
    bool selectedProfile = false;

    json jsonSettings;

    auto path = std::filesystem::absolute(settingsFilename);
    std::ifstream settingsFile(path);
    if (settingsFile.is_open()) {
        settingsFile >> jsonSettings;
        settingsFile.close();
        for (auto &profile : jsonSettings["terrain"]["profiles"].items()) {
            std::string name = profile.key();
            availableProfiles.push_back(name);
            if (!selectedProfile) {
                const char *nameStr = static_cast<const char *>(name.c_str());
                strcpy(currentProfile, nameStr);
                selectedProfile = true;
            }
        }
    }
}

void UserInterface::updateTerrain(Terrain *terrain, TerrainSettings &settings) {
    if (updateTerrainFunc) {
        updateTerrainFunc(terrain, settings);
    }
}

void UserInterface::renderSpaceDisplay() {
    ImGui::PushFont(context->uiEntityFont);

    ImGui::Begin("Space Display", &showSpaceDisplay,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs |
                 ImGuiWindowFlags_NoBackground);
    ImGui::SetWindowPos(ImVec2(0, context->displayCursor ? 15 : 0));

    auto player = context->player;
    auto velocity = entityManager->getComponent<VelocityComponent>(player);

    ImGui::Text("Speed: %.1f m/s", glm::length(velocity->velocity));
    ImGui::Text("Gravity: %.1f", glm::length(velocity->gravity));

    ImGui::End();

    ImGui::PopFont();
}

void UserInterface::renderEntityNames() {
    auto entity = context->selectedEntity;

    if (entity) {
        auto transform = entityManager->getComponent<TransformComponent>(entity);
        auto cameraTransform = entityManager->getComponent<TransformComponent>(context->getCamera());
        auto camera = entityManager->getComponent<CameraComponent>(context->getCamera());
        auto pos = glm::vec4(transform->position, 1.0);

        if (entityManager->hasComponent<CollisionComponent>(entity)) {
            auto collision = entityManager->getComponent<CollisionComponent>(entity);
            if (!context->viewFrustum->isInside(collision->boundingSphere)) {
                return;
            }
            float radius = collision->boundingSphere.getRadius();
            pos = glm::vec4(transform->position + camera->right * radius * 1.2f, 1);
        }

        const static float MAX_DISTANCE = 100.0;
        auto distance = glm::length(transform->position - cameraTransform->position);
        if (distance < MAX_DISTANCE) return;

        auto clipSpace = context->getProjection() * context->getView() * pos;
        auto normalizedDeviceSpace = glm::vec3(clipSpace) / clipSpace.w;

        auto screen = glm::vec2(normalizedDeviceSpace);
        screen.x = (screen.x + 1.0) / 2.0 * context->getWidth();
        screen.y = (1.0 - screen.y) / 2.0 * context->getHeight();

        if (screen.x >= 0.0 && screen.x <= context->getWidth() && screen.y >= 0.0 && screen.y <= context->getHeight()) {
            ImGui::Begin("Planet", &showSpaceDisplay,
                         ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs |
                         ImGuiWindowFlags_NoBackground);
            ImGui::SetWindowPos(ImVec2(screen.x, screen.y));

            ImGui::PushFont(context->uiEntityTitleFont);
            ImGui::Text("Entity %d", entity->id);
            ImGui::PopFont();
            ImGui::Text("Position: (%.1f, %.1f, %.1f)", pos.x, pos.y, pos.z);
            if (entityManager->hasComponent<MassComponent>(entity)) {
                auto mass = entityManager->getComponent<MassComponent>(entity);
                ImGui::Text("Mass: %.1f", mass->mass);
            }

            ImGui::Text("Distance: %.1f", distance);

            ImGui::End();
        }
    }
}

void UserInterface::setupTheme(bool styleDark, float alpha) {
    ImGuiStyle &style = ImGui::GetStyle();

    // light style from Pacôme Danhiez (user itamago) https://github.com/ocornut/imgui/pull/511#issuecomment-175719267
    style.Alpha = 1.0f;
    style.FrameRounding = 3.0f;
    style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
//    style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
//    style.Colors[ImGuiCol_ComboBg] = ImVec4(0.86f, 0.86f, 0.86f, 0.99f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
//    style.Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
//    style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
//    style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
//    style.Colors[ImGuiCol_CloseButton] = ImVec4(0.59f, 0.59f, 0.59f, 0.50f);
//    style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
//    style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

    if (styleDark) {
        for (int i = 0; i <= ImGuiCol_COUNT; i++) {
            ImVec4 &col = style.Colors[i];
            float H, S, V;
            ImGui::ColorConvertRGBtoHSV(col.x, col.y, col.z, H, S, V);

            if (S < 0.1f) {
                V = 1.0f - V;
            }
            ImGui::ColorConvertHSVtoRGB(H, S, V, col.x, col.y, col.z);
            if (col.w < 1.00f) {
                col.w *= alpha;
            }
        }
    } else {
        for (int i = 0; i <= ImGuiCol_COUNT; i++) {
            ImVec4 &col = style.Colors[i];
            if (col.w < 1.00f) {
                col.x *= alpha;
                col.y *= alpha;
                col.z *= alpha;
                col.w *= alpha;
            }
        }
    }
}

void UserInterface::renderAppInfoDisplay() {
    ImGui::PushFont(context->uiEntityFont);

    ImGui::Begin("Application Info", &showSpaceDisplay,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs |
                 ImGuiWindowFlags_NoBackground);
    ImGui::SetWindowPos(ImVec2(context->width - 120, context->displayCursor ? 15 : 0));

    ImGui::PushFont(context->uiEntityTitleFont);
    ImGui::Text("Mode");
    ImGui::PopFont();
    ImGui::SameLine();
    ImGui::Text("%s", context->displayWireframe ? "Wireframe" : "Shaded");

    ImGui::PushFont(context->uiEntityTitleFont);
    ImGui::Text("Bloom");
    ImGui::PopFont();
    ImGui::SameLine();
    ImGui::Text("%s", context->bloomEnabled ? "Enabled" : "Disabled");

    ImGui::End();

    ImGui::PopFont();
}
