#include "SkyboxEditor.h"

SkyboxEditor::SkyboxEditor(EntityManager &entityManager, GLContext &context)
        : entityManager(entityManager), context(context) {
    settings = DEFAULT_SKYBOX_SETTINGS;
}

void SkyboxEditor::render() {
    ImGui::Begin("Skybox Editor");

    ImGui::SliderInt("Resolution", &settings.resolution, 1, 8192);

    if (ImGui::Button("Generate")) {
        printf("Generating skybox...\n");
    }

    ImGui::End();
}
