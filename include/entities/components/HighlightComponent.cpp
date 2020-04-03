#include "HighlightComponent.h"

HighlightComponent::HighlightComponent(float scaleFactor, ShaderProgram *shaderProgram)
        : scaleFactor(scaleFactor), shaderProgram(shaderProgram) {
}

glm::mat4 HighlightComponent::getModel(glm::mat4 translated) {
    return glm::scale(translated, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
}
