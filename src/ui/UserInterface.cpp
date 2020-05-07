#include "cg/ui/UserInterface.h"
#include <json.hpp>
#include <fstream>
#include <string>
#include <iomanip>
#include <filesystem>

using json = nlohmann::json;


UserInterface::UserInterface(EntityManager *entityManager, GLContext *context)
        : entityManager(entityManager), context(context), settingsFilename("settings.json"), terrainProfileName("") {
    views = { "Spaceship", "Perspective", "Top", "Side" };
    noiseFunctions = { "Open Simplex", "Perlin", "Test" };

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
            ImGui::MenuItem("Terrain Generator", nullptr, &showTerrainGeneratorWindow);
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
                    context->setActiveCamera(context->spaceshipCamera);
                } else if (n == 1) {
                    context->setActiveCamera(context->perspectiveCamera);
                }
                else if (n == 2) {
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

    ImGui::InputText("Profile name", terrainProfileName, IM_ARRAYSIZE(terrainProfileName));

    if (ImGui::BeginCombo("Profiles", currentProfile)) {
        for (auto& profile : availableProfiles) {
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

    double minRedistribution = 0.0;
    double maxRedistribution = 10.0;
    if (ImGui::InputDouble("Redistribution", &settings.redistribution, 0.1, 0.5)) {
        updateTerrain(terrain, settings);
    }

    double minMaxHeight = 0.0;
    double maxMaxHeight = 30.0;
    if (ImGui::SliderScalar("Max height", ImGuiDataType_Double, &settings.maxAmplitude, &minMaxHeight, &maxMaxHeight, "%f", 1.0f)) {
        updateTerrain(terrain, settings);
    }

    if (ImGui::Button("Save settings")) {
        saveTerrainSettings();
    }

    ImGui::End();
}

void UserInterface::onUpdateTerrain(Terrain *terrain, std::function<bool(Terrain *, TerrainSettings& settings)> updateTerrainFunc) {
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
        for (auto& profile : jsonSettings["terrain"]["profiles"].items()) {
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

void UserInterface::updateTerrain(Terrain *terrain, TerrainSettings& settings) {
    if (updateTerrainFunc) {
        updateTerrainFunc(terrain, settings);
    }
}
