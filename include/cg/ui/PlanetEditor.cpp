#include "PlanetEditor.h"

PlanetEditor::PlanetEditor(EntityManager &entityManager, GLContext &context)
        : entityManager(entityManager), context(context), settings(getDefaultPlanetSettings()), noiseFunctions(0) {
    noiseTypes = {"Simple", "Ridged"};
}

void PlanetEditor::render() {
    auto selectedEntity = context.selectedEntity;

    if (!selectedEntity) return;

    auto meshes = entityManager.getMultiComponents<MeshComponent>(selectedEntity);
    auto pair = meshes.first;
    auto mesh = dynamic_cast<PlanetSide *>(pair->second);

    if (selected != selectedEntity) {
        settings = mesh->getSettings();
        selected = selectedEntity;
    }

    ImGui::Begin("Planet Editor");

    if (ImGui::Checkbox("Auto Update", &autoUpdate)) {
        updatePlanet(selected, *mesh->shaderProgram, autoUpdate);
    }
    if (ImGui::DragInt("Seed", &settings.seed, 1, 0, 10000)) {
        updatePlanet(selected, *mesh->shaderProgram, false);
    }
    if (ImGui::SliderInt("Subdivision", &settings.subdivision, 0, 200, "%d")) {
        updatePlanet(selected, *mesh->shaderProgram, false);
    }
    if (ImGui::DragFloat("Radius", &settings.radius, 0.1, 0.1, 10000.0)) {
        updatePlanet(selected, *mesh->shaderProgram, false);
    }

    noiseFunctions = settings.noiseSettings.size();
    if (ImGui::SliderInt("Noise functions", &noiseFunctions, 0, 4, "%d")) {
        auto count = settings.noiseSettings.size();
        settings.noiseSettings.resize(noiseFunctions);
        if (count < noiseFunctions) {
            for (int i = count; i < noiseFunctions; i++) {
                settings.noiseSettings[i] = getDefaultPlanetNoiseSettings();
            }
        }
    }

    for (int i = 0; i < noiseFunctions; i++) {
        renderNoise(i, mesh);
    }

    if (ImGui::Button("Generate")) {
        updatePlanet(selected, *mesh->shaderProgram, true);
    }

    ImGui::End();
}

void PlanetEditor::renderNoise(int index, PlanetSide *planetSide) {
    PlanetNoiseSettings noiseSettings = settings.noiseSettings[index];

    std::string name = "Noise " + std::to_string(index);
    if (ImGui::CollapsingHeader(name.c_str())) {
        ImGui::PushID(index * 100 + -2);
        if (ImGui::RadioButton("Simple noise", noiseSettings.noiseType == Simple)) {
            settings.noiseSettings[index].noiseType = Simple;
            updatePlanet(selected, *planetSide->shaderProgram, false);
        }
        ImGui::PopID();

        ImGui::SameLine();

        ImGui::PushID(index * 100 + -1);
        if (ImGui::RadioButton("Ridged noise", noiseSettings.noiseType == Ridged)) {
            settings.noiseSettings[index].noiseType = Ridged;
            updatePlanet(selected, *planetSide->shaderProgram, false);
        }
        ImGui::PopID();

        ImGui::PushID(index * 100 + 0);
        if (ImGui::Checkbox("Enabled", &settings.noiseSettings[index].enabled)) {
            updatePlanet(selected, *planetSide->shaderProgram, false);
        }
        ImGui::PopID();

        ImGui::PushID(index * 100 + 1);
        if (ImGui::Checkbox("Use first layer as mask", &settings.noiseSettings[index].useFirstLayerAsMask)) {
            updatePlanet(selected, *planetSide->shaderProgram, false);
        }
        ImGui::PopID();

        ImGui::PushID(index * 100 + 2);
        if (ImGui::DragFloat("Strength", &settings.noiseSettings[index].strength, 0.01, 0.01, 10000.0)) {
            updatePlanet(selected, *planetSide->shaderProgram, false);
        }
        ImGui::PopID();

        ImGui::PushID(index * 100 + 3);
        if (ImGui::DragInt("Layers", &settings.noiseSettings[index].layers, 1, 0, 20)) {
            updatePlanet(selected, *planetSide->shaderProgram, false);
        }
        ImGui::PopID();

        ImGui::PushID(index * 100 + 4);
        if (ImGui::DragFloat("Base Roughness", &settings.noiseSettings[index].baseRoughness, 0.1, 0.1, 10000.0)) {
            updatePlanet(selected, *planetSide->shaderProgram, false);
        }
        ImGui::PopID();

        ImGui::PushID(index * 100 + 5);
        if (ImGui::DragFloat("Roughness", &settings.noiseSettings[index].roughness, 0.1, 0.1, 10000.0)) {
            updatePlanet(selected, *planetSide->shaderProgram, false);
        }
        ImGui::PopID();

        ImGui::PushID(index * 100 + 6);
        if (ImGui::DragFloat("Persistence", &settings.noiseSettings[index].persistence, 0.1, 0.1, 10000.0)) {
            updatePlanet(selected, *planetSide->shaderProgram, false);
        }
        ImGui::PopID();

        ImGui::PushID(index * 100 + 7);
        if (ImGui::DragFloat("Min value", &settings.noiseSettings[index].minValue, 0.1, 0.1, 10000.0)) {
            updatePlanet(selected, *planetSide->shaderProgram, false);
        }
        ImGui::PopID();

        if (settings.noiseSettings[index].noiseType == Ridged) {
            ImGui::PushID(index * 100 + 8);
            if (ImGui::DragFloat("Weight multiplier", &settings.noiseSettings[index].weightMultiplier, 0.1, 0.1, 10000.0)) {
                updatePlanet(selected, *planetSide->shaderProgram, false);
            }
            ImGui::PopID();
        }

        ImGui::PushID(index * 100 + 9);
        if (ImGui::DragFloat3("Center", glm::value_ptr(settings.noiseSettings[index].center), 0.1, 0.1, 10000.0)) {
            updatePlanet(selected, *planetSide->shaderProgram, false);
        }
        ImGui::PopID();
    }
}

void PlanetEditor::updatePlanet(Entity *entity, ShaderProgram &shaderProgram, bool force) {
    if (!force && !autoUpdate) return;

    auto meshes = entityManager.getMultiComponents<MeshComponent>(entity);
    FaceDirection directions[] = {FrontFace, BackFace, LeftFace, RightFace, UpFace, DownFace};
    int i = 0;
    for (auto it = meshes.first; it != meshes.second; it++) {
        settings.direction = directions[i++];
        auto planetSide = dynamic_cast<PlanetSide *>(it->second);
        planetSide->updateSettings(settings);
    }
}
