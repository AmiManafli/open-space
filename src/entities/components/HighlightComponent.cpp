#include "cg/entities/components/HighlightComponent.h"

HighlightComponent::HighlightComponent(float size, ShaderProgram *shaderProgram)
        : size(size), shaderProgram(shaderProgram) {
}

glm::mat4 HighlightComponent::getModel(glm::mat4 translated, float distance) {
    float scale = size + (0.0005 * distance);
    return glm::scale(translated, glm::vec3(scale, scale, scale));
}
