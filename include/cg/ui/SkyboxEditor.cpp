#include "SkyboxEditor.h"

SkyboxEditor::SkyboxEditor(EntityManager &entityManager, GLContext &context)
        : entityManager(entityManager), context(context) {
}

void SkyboxEditor::render() {
    auto settings = context.skyboxSettings;

    ImGui::Begin("Skybox Editor");
    ImGui::SetWindowSize(ImVec2(250, 600));

    ImGui::SliderInt("Seed", &context.skyboxSettings.seed, 1, 1000000000);
    ImGui::SliderInt("Resolution", &context.skyboxSettings.resolution, 1, 8192);
    ImGui::SliderInt("Size", &context.skyboxSettings.size, 1, 100000);

    ImGui::SliderInt("Point stars", &context.skyboxSettings.numPointStars, 0, 1000000);
    ImGui::SliderFloat("Small point stars (%)", &context.skyboxSettings.smallPointStars, 0, 1);
    ImGui::SliderFloat("Medium point stars (%)", &context.skyboxSettings.mediumPointStars, 0, 1);
    ImGui::SliderFloat("Large point stars (%)", &context.skyboxSettings.largePointStars, 0, 1);

    ImGui::SliderInt("Bright stars", &context.skyboxSettings.numBrightStars, 0, 1000000);

    ImGui::Checkbox("Enable nebula", &context.skyboxSettings.enableNebulae);
    ImGui::ColorPicker3("Color", glm::value_ptr(context.skyboxSettings.nebulaSettings.color));
    ImGui::SliderFloat3("Offset", glm::value_ptr(context.skyboxSettings.nebulaSettings.offset), 0, 1000);
    ImGui::SliderFloat("Scale", &context.skyboxSettings.nebulaSettings.scale, 0, 10.0f);
    ImGui::SliderFloat("Intensity", &context.skyboxSettings.nebulaSettings.intensity, 0, 10.0f);
    ImGui::SliderFloat("Falloff", &context.skyboxSettings.nebulaSettings.falloff, 0, 10.0f);

    ImGui::Checkbox("Save texture to file", &context.skyboxSettings.saveToFile);

    if (ImGui::Button("Generate")) {
        context.generateSkybox = true;
    }

    ImGui::End();
}
