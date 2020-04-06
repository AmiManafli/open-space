#include <cg/entities/components/TransformComponent.h>

TransformComponent::TransformComponent()
        : model(glm::mat4(1.0f)), position(glm::vec3(0, 0, 0)), scaling(glm::vec3(0, 0, 0)), rotation(glm::vec3(0, 0, 0)) {
}

TransformComponent::TransformComponent(glm::vec3 position) : TransformComponent() {
    this->position = position;
    model = glm::translate(glm::mat4(1.0f), position);
}

void TransformComponent::scale(float factor) {
    scaling = glm::vec3(factor, factor, factor);
    scale(scaling);
}

void TransformComponent::scale(glm::vec3 scaling) {
    this->scaling += scaling;
    model = glm::scale(model, scaling);
}

void TransformComponent::move(float x, float y, float z) {
    move(glm::vec3(x, y, z));
}

void TransformComponent::move(glm::vec3 diff) {
    position += diff;
    model = glm::translate(model, diff);
}

void TransformComponent::rotate(float angle, glm::vec3 axis) {
    rotation += axis * angle;
    model = glm::rotate(model, angle, axis);
}

