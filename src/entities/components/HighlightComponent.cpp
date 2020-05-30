#include "cg/entities/components/HighlightComponent.h"

HighlightComponent::HighlightComponent(float size, ShaderProgram *shaderProgram)
        : size(size), shaderProgram(shaderProgram) {
}

glm::mat4 HighlightComponent::getModel(glm::mat4 translated, float distance) {
    float normSize = glm::max(size, 1.04f);
    auto minSize = 1.03;
    auto delta = (normSize - minSize) / 1450.0;
    auto scale = minSize + delta * distance;
    printf("s=%f, d=%f, scale: %f\n", size, distance, scale);
    return glm::scale(translated, glm::vec3(scale));
}
