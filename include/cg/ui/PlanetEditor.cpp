#include "PlanetEditor.h"

PlanetEditor::PlanetEditor(EntityManager &entityManager, GLContext &context) : entityManager(entityManager), context(context) {
}

void PlanetEditor::render() {
    ImGui::Begin("Planet Editor");

    ImGui::DragInt("Seed", &settings.seed, 1, 0, UINT64_MAX);

    ImGui::End();
}
