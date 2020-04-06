#include "cg/entities/components/VelocityComponent.h"

VelocityComponent::VelocityComponent(glm::vec3 velocity)
        : velocity(velocity) {
}

VelocityComponent::VelocityComponent(glm::vec3 rotation, glm::vec3 rotationPoint)
        : rotation(rotation), rotationPoint(rotationPoint) {
}
