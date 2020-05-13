#include "PlanetEditor.h"

PlanetEditor::PlanetEditor(EntityManager &entityManager, GLContext &context)
        : entityManager(entityManager), context(context) {
    settings = getDefaultPlanetSettings();
}

void PlanetEditor::render() {
    auto selected = context.selectedEntity;

    if (!selected) return;

    ImGui::Begin("Planet Editor");

    if (ImGui::DragInt("Seed", &settings.seed, 1, 0, 10000)) {
        updatePlanet(selected);
    }

    if (ImGui::DragFloat("Radius", &settings.radius, 0.1, 0.1, 10000.0)) {
        updatePlanet(selected);
    }

    if (ImGui::SliderInt("Subdivision", &settings.subdivision, 0, 200, "%d")) {
        updatePlanet(selected);
    }

    if (ImGui::DragInt("Layers", &settings.layers, 1, 0, 20)) {
        updatePlanet(selected);
    }

    if (ImGui::DragFloat("Strength", &settings.strength, 0.1, 0.1, 10000.0)) {
        updatePlanet(selected);
    }

    if (ImGui::DragFloat("Base Roughness", &settings.baseRoughness, 0.1, 0.1, 10000.0)) {
        updatePlanet(selected);
    }

    if (ImGui::DragFloat("Roughness", &settings.roughness, 0.1, 0.1, 10000.0)) {
        updatePlanet(selected);
    }

    if (ImGui::DragFloat("Persistence", &settings.persistence, 0.1, 0.1, 10000.0)) {
        updatePlanet(selected);
    }

    if (ImGui::DragFloat("Min value", &settings.minValue, 0.1, 0.1, 10000.0)) {
        updatePlanet(selected);
    }

    if (ImGui::DragFloat3("Center", glm::value_ptr(settings.center), 0.1, 0.1, 10000.0)) {
        updatePlanet(selected);
    }

    ImGui::End();
}

void PlanetEditor::updatePlanet(Entity *entity) {
    auto meshes = entityManager.getMultiComponents<MeshComponent>(entity);
    auto it = meshes.first;

    auto planet = dynamic_cast<PlanetGenerator *>(it->second);
    planet->updateSettings(settings);
}
