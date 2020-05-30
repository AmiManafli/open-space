#include "SkyboxEditor.h"

SkyboxEditor::SkyboxEditor(EntityManager &entityManager, GLContext &context)
        : entityManager(entityManager), context(context) {
}

void SkyboxEditor::render() {
    auto settings = context.skyboxSettings;

    ImGui::Begin("Skybox Editor");
    ImGui::SetWindowSize(ImVec2(250, 570));

    ImGui::SliderInt("Resolution", &settings.resolution, 1, 8192);
    ImGui::SliderInt("Size", &settings.size, 1, 100000);

    ImGui::SliderInt("Point stars", &settings.numPointStars, 0, 1000000);
    ImGui::SliderFloat("Small point stars (%)", &settings.smallPointStars, 0, 1);
    ImGui::SliderFloat("Medium point stars (%)", &settings.mediumPointStars, 0, 1);
    ImGui::SliderFloat("Large point stars (%)", &settings.largePointStars, 0, 1);

    ImGui::SliderInt("Bright stars", &settings.numBrightStars, 0, 1000000);

    ImGui::Checkbox("Enable nebula", &settings.enableNebulae);
    ImGui::ColorPicker3("Color", glm::value_ptr(settings.nebulaSettings.color));
    ImGui::SliderFloat3("Offset", glm::value_ptr(settings.nebulaSettings.offset), 0, 1000);
    ImGui::SliderFloat("Scale", &settings.nebulaSettings.scale, 0, 10.0f);
    ImGui::SliderFloat("Intensity", &settings.nebulaSettings.intensity, 0, 10.0f);
    ImGui::SliderFloat("Falloff", &settings.nebulaSettings.falloff, 0, 10.0f);

    ImGui::Checkbox("Save texture to file", &settings.saveToFile);

    if (ImGui::Button("Generate")) {
        context.generateSkybox = true;
        context.skyboxSettings = settings;
    }

    ImGui::End();
}
