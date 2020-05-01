#include <cmath>
#include <cg/entities/Entity.h>
#include "cg/entities/components/OrbitComponent.h"

OrbitComponent::OrbitComponent(glm::vec3 parentPosition, float semiMajorAxis, float semiMinorAxis, float speed, float startTheta)
        : parentPosition(parentPosition), startTheta(startTheta), theta(startTheta) {
    update(parentPosition, semiMajorAxis, semiMinorAxis, speed);
}

void OrbitComponent::update(glm::vec3 parentPosition, float semiMajorAxis, float semiMinorAxis, float speed) {
    this->semiMajorAxis = semiMajorAxis;
    this->semiMinorAxis = semiMinorAxis;
    this->speed = speed;

    eccentricity = glm::sqrt(1 - glm::pow(semiMinorAxis, 2) / glm::pow(semiMajorAxis, 2));
    width = 2 * semiMajorAxis;
    height = 2 * semiMinorAxis;
    focusCord = glm::pow(semiMinorAxis, 2) / semiMajorAxis;

    // Set starting position based on the parent position
    float center = glm::sqrt(glm::pow(semiMajorAxis, 2) - glm::pow(semiMinorAxis, 2));
    startPosition = glm::vec2(parentPosition.x + 2 * center, parentPosition.z);
}

glm::vec3 OrbitComponent::getPosition() {
    auto radius = calculateRadius(theta);

    auto position = startPosition + glm::vec2(radius * cos(theta), radius * sin(theta));

    return glm::vec3(position.x, parentPosition.y, position.y);
}

float OrbitComponent::calculateRadius(float theta) {
    return focusCord / (1 + eccentricity * cos(theta));
}
