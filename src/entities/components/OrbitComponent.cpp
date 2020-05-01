#include <cmath>
#include <cg/entities/Entity.h>
#include "cg/entities/components/OrbitComponent.h"

OrbitComponent::OrbitComponent(TransformComponent *parentTransform, float semiMajorAxis, float semiMinorAxis, float speed, float startTheta)
        : parentTransform(parentTransform), startTheta(startTheta), theta(startTheta) {
    update(semiMajorAxis, semiMinorAxis, speed);
}

void OrbitComponent::update(float semiMajorAxis, float semiMinorAxis, float speed) {
    this->semiMajorAxis = semiMajorAxis;
    this->semiMinorAxis = semiMinorAxis;
    this->speed = speed;

    eccentricity = glm::sqrt(1 - glm::pow(semiMinorAxis, 2) / glm::pow(semiMajorAxis, 2));
    width = 2 * semiMajorAxis;
    height = 2 * semiMinorAxis;
    focusCord = glm::pow(semiMinorAxis, 2) / semiMajorAxis;

    // Set starting position based on the parent position
    center = glm::sqrt(glm::pow(semiMajorAxis, 2) - glm::pow(semiMinorAxis, 2));
//    startPosition = glm::vec2(parentTransf.x + 2 * center, parentPosition.z);
}

glm::vec3 OrbitComponent::getPosition() {
    auto radius = calculateRadius(theta);

    auto startPosition = glm::vec3(parentTransform->position.x + 2 * center, parentTransform->position.y, parentTransform->position.z);

    return startPosition + glm::vec3(radius * cos(theta), 0, radius * sin(theta));
}

float OrbitComponent::calculateRadius(float theta) {
    return focusCord / (1 + eccentricity * cos(theta));
}
