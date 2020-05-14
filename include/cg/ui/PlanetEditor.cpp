#include "PlanetEditor.h"

PlanetEditor::PlanetEditor(EntityManager &entityManager, GLContext &context)
        : entityManager(entityManager), context(context), settings(getDefaultPlanetSettings()), noiseFunctions(0) {
    noiseTypes = { "Simple", "Ridged" };
}

void PlanetEditor::render() {
    auto selected = context.selectedEntity;

    if (!selected) return;

    ImGui::Begin("Planet Editor");

    ImGui::Checkbox("Auto Update", &autoUpdate);
    ImGui::DragInt("Seed", &settings.seed, 1, 0, 10000);
    ImGui::SliderInt("Subdivision", &settings.subdivision, 0, 200, "%d");
    ImGui::DragFloat("Radius", &settings.radius, 0.1, 0.1, 10000.0);
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
        renderNoise(i);
    }

    if (ImGui::Button("Generate")) {
        updatePlanet(selected);
    }

    ImGui::End();
}

void PlanetEditor::renderNoise(int index) {
    PlanetNoiseSettings noiseSettings = settings.noiseSettings[index];

    std::string name = "Noise " + std::to_string(index);
    if (ImGui::CollapsingHeader(name.c_str())) {
        ImGui::PushID(index * 100 + -2);
        if (ImGui::RadioButton("Simple noise", noiseSettings.noiseType == Simple)) {
            settings.noiseSettings[index].noiseType = Simple;
        }
        ImGui::PopID();

        ImGui::SameLine();

        ImGui::PushID(index * 100 + -1);
        if (ImGui::RadioButton("Ridged noise", noiseSettings.noiseType == Ridged)) {
            settings.noiseSettings[index].noiseType = Ridged;
        }
        ImGui::PopID();

        ImGui::PushID(index * 100 + 0);
        ImGui::Checkbox("Enabled", &settings.noiseSettings[index].enabled);
        ImGui::PopID();

        ImGui::PushID(index * 100 + 1);
        ImGui::Checkbox("Use first layer as mask", &settings.noiseSettings[index].useFirstLayerAsMask);
        ImGui::PopID();

        ImGui::PushID(index * 100 + 2);
        ImGui::DragFloat("Strength", &settings.noiseSettings[index].strength, 0.01, 0.01, 10000.0);
        ImGui::PopID();

        ImGui::PushID(index * 100 + 3);
        ImGui::DragInt("Layers", &settings.noiseSettings[index].layers, 1, 0, 20);
        ImGui::PopID();

        ImGui::PushID(index * 100 + 4);
        ImGui::DragFloat("Base Roughness", &settings.noiseSettings[index].baseRoughness, 0.1, 0.1, 10000.0);
        ImGui::PopID();

        ImGui::PushID(index * 100 + 5);
        ImGui::DragFloat("Roughness", &settings.noiseSettings[index].roughness, 0.1, 0.1, 10000.0);
        ImGui::PopID();

        ImGui::PushID(index * 100 + 6);
        ImGui::DragFloat("Persistence", &settings.noiseSettings[index].persistence, 0.1, 0.1, 10000.0);
        ImGui::PopID();

        ImGui::PushID(index * 100 + 7);
        ImGui::DragFloat("Min value", &settings.noiseSettings[index].minValue, 0.1, 0.1, 10000.0);
        ImGui::PopID();

        if (settings.noiseSettings[index].noiseType == Ridged) {
            ImGui::PushID(index * 100 + 8);
            ImGui::DragFloat("Weight multiplier", &settings.noiseSettings[index].weightMultiplier, 0.1, 0.1, 10000.0);
            ImGui::PopID();
        }

        ImGui::PushID(index * 100 + 9);
        ImGui::DragFloat3("Center", glm::value_ptr(settings.noiseSettings[index].center), 0.1, 0.1, 10000.0);
        ImGui::PopID();
    }
}

void PlanetEditor::updatePlanet(Entity *entity) {
    auto meshes = entityManager.getMultiComponents<MeshComponent>(entity);
    auto it = meshes.first;

    auto planet = dynamic_cast<PlanetGenerator *>(it->second);
    planet->updateSettings(settings);
}
