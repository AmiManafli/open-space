#include "cg/entities/components/VelocityComponent.h"

VelocityComponent::VelocityComponent(glm::vec3 position)
        : position(position) {
}

VelocityComponent::VelocityComponent(glm::vec3 rotation, glm::vec3 rotationPoint)
        : rotation(rotation), rotationPoint(rotationPoint) {
}

void VelocityComponent::update(EntityManager *entityManager, uint32_t entityId) {
    if (customUpdate.has_value()) {
        customUpdate.value()(entityManager, entityId);
    }
}