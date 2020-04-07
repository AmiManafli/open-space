#include <cg/entities/components/TransformComponent.h>
#include "cg/entities/components/LightComponent.h"

LightComponent::LightComponent(Type type, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
        : type(type), ambient(ambient), diffuse(diffuse), specular(specular) {
}

void LightComponent::setUniforms(ShaderProgram *shaderProgram, TransformComponent *transform) {
    shaderProgram->setUniform("light.position", transform->position);
    shaderProgram->setUniform("light.ambient", ambient);
    shaderProgram->setUniform("light.diffuse", diffuse);
    shaderProgram->setUniform("light.specular", specular);
}
