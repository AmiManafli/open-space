#include "cg/entities/components/VelocityComponent.h"

VelocityComponent::VelocityComponent()
        : VelocityComponent(glm::vec3(0), glm::vec3(0), glm::vec3(0)) {
}

VelocityComponent::VelocityComponent(glm::vec3 velocity)
        : VelocityComponent(velocity, glm::vec3(0), glm::vec3(0)) {
}

VelocityComponent::VelocityComponent(glm::vec3 rotation, glm::vec3 rotationPoint)
        : VelocityComponent(glm::vec3(0), rotation, rotationPoint) {
}

VelocityComponent::VelocityComponent(glm::vec3 velocity, glm::vec3 rotation, glm::vec3 rotationPoint)
        : velocity(velocity), rotation(rotation), rotationPoint(rotationPoint), scaling(glm::vec3(0)), gravity(glm::vec3(0)), attachedTo(nullptr) {
}

void VelocityComponent::update(EntityManager *entityManager, Entity *entity) {
    if (customUpdate.has_value()) {
        customUpdate.value()(entityManager, entity);
    }
}
