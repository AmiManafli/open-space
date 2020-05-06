#include <cg/entities/components/TransformComponent.h>
#include "cg/entities/components/LightComponent.h"

LightComponent::LightComponent() {
}

void LightComponent::setUniforms(ShaderProgram *shaderProgram, TransformComponent *transform) {
    switch (type) {
        case Direction:
            shaderProgram->setUniform("light.type", type);
            shaderProgram->setUniform("light.direction", direction);
            shaderProgram->setUniform("light.ambient", ambient);
            shaderProgram->setUniform("light.diffuse", diffuse);
            shaderProgram->setUniform("light.specular", specular);
            break;
        case Point:
            shaderProgram->setUniform("light.type", type);
            shaderProgram->setUniform("light.position", transform->position);

            shaderProgram->setUniform("light.ambient", ambient);
            shaderProgram->setUniform("light.diffuse", diffuse);
            shaderProgram->setUniform("light.specular", specular);

            shaderProgram->setUniform("light.constant", constant);
            shaderProgram->setUniform("light.linear", linear);
            shaderProgram->setUniform("light.quadratic", quadratic);
            break;
        default:
            throw std::runtime_error("Unsupported light type");
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

LightComponent *
LightComponent::createPointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic) {
    auto component = new LightComponent();
    component->type = Point;
    component->ambient = ambient;
    component->diffuse = diffuse;
    component->specular = specular;
    component->constant = constant;
    component->linear = linear;
    component->quadratic = quadratic;
    return component;
}
