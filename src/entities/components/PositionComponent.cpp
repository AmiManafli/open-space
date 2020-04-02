#include <entities/components/PositionComponent.h>

PositionComponent::PositionComponent(float x, float y, float z)
        : PositionComponent(glm::vec3(x, y, z)){
}

PositionComponent::PositionComponent(glm::vec3 position)
        : position(position) {
    model = glm::translate(glm::mat4(1.0f), position);
}
