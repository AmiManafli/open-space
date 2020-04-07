#include <cg/entities/components/TransformComponent.h>
#include "cg/entities/components/LightComponent.h"

LightComponent::LightComponent() {
}

void LightComponent::setUniforms(ShaderProgram *shaderProgram, TransformComponent *transform) {
    switch (type) {
        case Direction:
            shaderProgram->setUniform("directionalLight.direction", direction);
            shaderProgram->setUniform("directionalLight.ambient", ambient);
            shaderProgram->setUniform("directionalLight.diffuse", diffuse);
            shaderProgram->setUniform("directionalLight.specular", specular);
            break;
        default:
            throw std::runtime_error("unsupported light type");
    }
}

LightComponent *
LightComponent::createDirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
    auto component = new LightComponent();
    component->type = Direction;
    component->direction = direction;
    component->ambient = ambient;
    component->diffuse = diffuse;
    component->specular = specular;
    return component;
}
