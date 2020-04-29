#include <cmath>
#include <cg/entities/Entity.h>
#include "cg/entities/components/OrbitComponent.h"

OrbitComponent::OrbitComponent(Entity *parent, double semiMajorAxis, double semiMinorAxis) {
    update(semiMajorAxis, semiMinorAxis);
}

void OrbitComponent::update(double semiMajorAxis, double semiMinorAxis) {
    this->semiMajorAxis = semiMajorAxis;
    this->semiMinorAxis = semiMinorAxis;

    eccentricity = sqrt(1 - pow(semiMinorAxis, 2) / pow(semiMajorAxis, 2));
    width = 2 * semiMajorAxis;
    height = 2 * semiMinorAxis;
    focusCord = pow(semiMinorAxis, 2) / semiMajorAxis;
}

glm::vec2 OrbitComponent::position(double theta) {
    auto radius = calculateRadius(theta);

    return glm::vec2(radius * cos(theta), radius * sin(theta));
}

double OrbitComponent::calculateRadius(double theta) {
    return focusCord / (1 + eccentricity * cos(theta));
}
