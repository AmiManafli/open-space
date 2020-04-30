#include "cg/entities/components/VelocityComponent.h"

VelocityComponent::VelocityComponent()
        : VelocityComponent(glm::vec3(0), glm::vec3(0), glm::vec3(0)) {
}

VelocityComponent::VelocityComponent(glm::vec3 position)
        : VelocityComponent(position, glm::vec3(0), glm::vec3(0)) {
}

VelocityComponent::VelocityComponent(glm::vec3 rotation, glm::vec3 rotationPoint)
        : VelocityComponent(glm::vec3(0), rotation, rotationPoint) {
}

VelocityComponent::VelocityComponent(glm::vec3 position, glm::vec3 rotation, glm::vec3 rotationPoint)
        : position(position), rotation(rotation), rotationPoint(rotationPoint), scaling(glm::vec3(0)), gravity(glm::vec3(0)) {
}

void VelocityComponent::update(EntityManager *entityManager, uint32_t entityId) {
    if (customUpdate.has_value()) {
        customUpdate.value()(entityManager, entityId);
    }
}
