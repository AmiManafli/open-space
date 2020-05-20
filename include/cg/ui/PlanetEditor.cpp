#include <omp.h>
#include "PlanetEditor.h"

PlanetEditor::PlanetEditor(EntityManager &entityManager, GLContext &context)
        : entityManager(entityManager), context(context), settings(getDefaultPlanetSettings()), noiseFunctions(0) {
    noiseTypes = {"Simple", "Ridged"};
}

void PlanetEditor::renderEmpty() {
    ImGui::Text("No planet selected.");
    ImGui::End();
}

void PlanetEditor::render() {
    auto selectedEntity = context.selectedEntity;

    PlanetSide *mesh;
    if (selectedEntity) {
        auto meshes = entityManager.getMultiComponents<MeshComponent>(selectedEntity);
        auto pair = meshes.first;
        mesh = dynamic_cast<PlanetSide *>(pair->second);

        if (selected != selectedEntity) {
            if (mesh) {
                settings = mesh->getSettings();
                selected = selectedEntity;
            } else {
                selected = nullptr;
                selectedEntity = nullptr;
            }
        }
    }

    ImGui::Begin("Planet Editor");

    if (!selectedEntity) return renderEmpty();

    ImGui::Text("Entity ID: %d", selectedEntity->id);

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
    if (ImGui::DragFloat("Spherical", &settings.sphereFactor, 0.01, 0.0, 1.0)) {
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
    auto i = index * 100;
    if (ImGui::CollapsingHeader(name.c_str())) {
        ImGui::PushID(i++);
        if (ImGui::RadioButton("Simple noise", noiseSettings.noiseType == Simple)) {
            settings.noiseSettings[index].noiseType = Simple;
            updatePlanet(selected, *planetSide->shaderProgram, false);
        }
        ImGui::PopID();

        ImGui::SameLine();

        ImGui::PushID(i++);
        if (ImGui::RadioButton("Ridged noise", noiseSettings.noiseType == Ridged)) {
            settings.noiseSettings[index].noiseType = Ridged;
            updatePlanet(selected, *planetSide->shaderProgram, false);
        }
        ImGui::PopID();

        ImGui::PushID(i++);
        if (ImGui::Checkbox("Enabled", &settings.noiseSettings[index].enabled)) {
            updatePlanet(selected, *planetSide->shaderProgram, false);
        }
        ImGui::PopID();

        ImGui::PushID(i++);
        if (ImGui::Checkbox("Use first layer as mask", &settings.noiseSettings[index].useFirstLayerAsMask)) {
            updatePlanet(selected, *planetSide->shaderProgram, false);
        }
        ImGui::PopID();

        ImGui::PushID(i++);
        if (ImGui::DragFloat("Strength", &settings.noiseSettings[index].strength, 0.01, 0.01, 10000.0)) {
            updatePlanet(selected, *planetSide->shaderProgram, false);
        }
        ImGui::PopID();

        ImGui::PushID(i++);
        if (ImGui::DragInt("Layers", &settings.noiseSettings[index].layers, 1, 0, 20)) {
            updatePlanet(selected, *planetSide->shaderProgram, false);
        }
        ImGui::PopID();

        ImGui::PushID(i++);
        if (ImGui::DragFloat("Base Roughness", &settings.noiseSettings[index].baseRoughness, 0.1, 0.1, 10000.0)) {
            updatePlanet(selected, *planetSide->shaderProgram, false);
        }
        ImGui::PopID();

        ImGui::PushID(i++);
        if (ImGui::DragFloat("Roughness", &settings.noiseSettings[index].roughness, 0.1, 0.1, 10000.0)) {
            updatePlanet(selected, *planetSide->shaderProgram, false);
        }
        ImGui::PopID();

        ImGui::PushID(i++);
        if (ImGui::DragFloat("Persistence", &settings.noiseSettings[index].persistence, 0.1, 0.1, 10000.0)) {
            updatePlanet(selected, *planetSide->shaderProgram, false);
        }
        ImGui::PopID();

        ImGui::PushID(i++);
        if (ImGui::DragFloat("Min value", &settings.noiseSettings[index].minValue, 0.1, 0.1, 10000.0)) {
            updatePlanet(selected, *planetSide->shaderProgram, false);
        }
        ImGui::PopID();

        if (settings.noiseSettings[index].noiseType == Ridged) {
            ImGui::PushID(i++);
            if (ImGui::DragFloat("Weight multiplier", &settings.noiseSettings[index].weightMultiplier, 0.1, 0.1, 10000.0)) {
                updatePlanet(selected, *planetSide->shaderProgram, false);
            }
            ImGui::PopID();
        }

        ImGui::PushID(i++);
        if (ImGui::DragFloat3("Center", glm::value_ptr(settings.noiseSettings[index].center), 0.1, 0.1, 10000.0)) {
            updatePlanet(selected, *planetSide->shaderProgram, false);
        }
        ImGui::PopID();
    }
}

void PlanetEditor::updatePlanet(Entity *entity, ShaderProgram &shaderProgram, bool force) {
    if (!force && !autoUpdate) return;

    FaceDirection directions[] = {FrontFace, BackFace, LeftFace, RightFace, UpFace, DownFace};
    auto meshPairs = entityManager.getMultiComponents<MeshComponent>(entity);
    auto it = meshPairs.first;
    PlanetSide* sides[6];
    sides[0] = dynamic_cast<PlanetSide *>(it->second); it++;
    sides[1] = dynamic_cast<PlanetSide *>(it->second); it++;
    sides[2] = dynamic_cast<PlanetSide *>(it->second); it++;
    sides[3] = dynamic_cast<PlanetSide *>(it->second); it++;
    sides[4] = dynamic_cast<PlanetSide *>(it->second); it++;
    sides[5] = dynamic_cast<PlanetSide *>(it->second);
#pragma omp parallel for
    for (int i = 0; i < 6; i++) {
        printf("Processing planet side %d in thread %d/%d...\n", i, omp_get_thread_num(), omp_get_max_threads());
        settings.direction = directions[i];
        sides[i]->updateSettings(settings);
    }
    for (int i = 0; i < 6; i++) {
        sides[i]->setupBuffers();
    }
    printf("Finished processing planet!\n", sides[0]->maxHeight);
}
