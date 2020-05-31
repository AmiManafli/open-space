#include <cg/entities/components/TransformComponent.h>

TransformComponent::TransformComponent()
        : position(glm::vec3(0, 0, 0)), scaling(glm::vec3(1, 1, 1)),
          rotation(glm::vec3(0, 0, 0)) {
}

TransformComponent::TransformComponent(glm::vec3 position) : TransformComponent() {
    this->position = position;
}

void TransformComponent::scale(float factor) {
    scale(glm::vec3(factor, factor, factor));
}

void TransformComponent::scale(glm::vec3 scaling) {
    this->scaling *= scaling;
}

void TransformComponent::move(float x, float y, float z) {
    move(glm::vec3(x, y, z));
}

void TransformComponent::move(glm::vec3 diff) {
    previousPosition = position;
    position += diff;
}

void TransformComponent::rotate(glm::vec3 angles) {
    rotation += angles;
}

glm::mat4 TransformComponent::getModel() {
    auto model = glm::translate(glm::mat4(1.0), position);

    if (rotation.x != 0.0f) {
        model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
    }
    if (rotation.y != 0.0f) {
        model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
    }
    if (rotation.z != 0.0f) {
        model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));
    }

    model = glm::scale(model, scaling);

    return model;
}
