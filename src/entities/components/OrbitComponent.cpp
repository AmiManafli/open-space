#include <cmath>
#include <cg/entities/Entity.h>
#include "cg/entities/components/OrbitComponent.h"

OrbitComponent::OrbitComponent(glm::vec3 parentPosition, double semiMajorAxis, double semiMinorAxis, double speed, double startTheta)
        : startTheta(startTheta) {
    update(parentPosition, semiMajorAxis, semiMinorAxis, speed);
}

void OrbitComponent::update(glm::vec3 parentPosition, double semiMajorAxis, double semiMinorAxis, double speed) {
    this->semiMajorAxis = semiMajorAxis;
    this->semiMinorAxis = semiMinorAxis;
    this->speed = speed;
    theta = startTheta;

    eccentricity = sqrt(1 - pow(semiMinorAxis, 2) / pow(semiMajorAxis, 2));
    width = 2 * semiMajorAxis;
    height = 2 * semiMinorAxis;
    focusCord = pow(semiMinorAxis, 2) / semiMajorAxis;

    // Set starting position based on the parent position
    float center = sqrt(pow(semiMajorAxis, 2) - pow(semiMinorAxis, 2));
    startPosition = glm::vec2(parentPosition.x + 2 * center, parentPosition.z);

    printf("Orbit:\n  width: %f\n  height: %f\n  eccentricity: %f\n\n", width, height, eccentricity);
}

glm::vec2 OrbitComponent::getPosition() {
    auto radius = calculateRadius(theta);

    return startPosition + glm::vec2(radius * cos(theta), radius * sin(theta));
}

double OrbitComponent::calculateRadius(double theta) {
    return focusCord / (1 + eccentricity * cos(theta));
}
