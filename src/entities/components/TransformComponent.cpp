#include <cg/entities/components/TransformComponent.h>

TransformComponent::TransformComponent(float x, float y, float z)
        : TransformComponent(glm::vec3(x, y, z)) {
}

TransformComponent::TransformComponent(glm::vec3 position)
        : position(position) {
    model = glm::translate(glm::mat4(1.0f), position);
}

void TransformComponent::scale(float factor) {
    model = glm::scale(model, glm::vec3(factor, factor, factor));
}
