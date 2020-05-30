#include "SkyboxEditor.h"

SkyboxEditor::SkyboxEditor(EntityManager &entityManager, GLContext &context)
        : entityManager(entityManager), context(context) {
//    settings = Skybox::getDefaultSettings();
}

void SkyboxEditor::render() {
    ImGui::Begin("Skybox Editor");

//    if (ImGui::SliderInt("Resolution", &settings.resolution, 32, 8192)) {
//        printf("Resolution changed to %d!\n", settings.resolution);
//    }

    ImGui::End();
}
