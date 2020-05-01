#ifndef CG1_PROJECT_ORBITCOMPONENT_H
#define CG1_PROJECT_ORBITCOMPONENT_H

#include <cg/GLHeader.h>
#include "TransformComponent.h"

class OrbitComponent {
public:
    OrbitComponent(TransformComponent *parentTransform, float semiMajorAxis, float semiMinorAxis, float speed, float startTheta);

    float semiMajorAxis;
    float semiMinorAxis;

    float startTheta;
    float theta;

    float speed;

    float eccentricity;
    float focusCord;
    float width;
    float height;

    float center;

    TransformComponent *parentTransform;
    glm::vec2 startPosition;

    float calculateRadius(float theta);
    glm::vec3 getPosition();

    void update(float semiMajorAxis, float semiMinorAxis, float speed);
};

#endif //CG1_PROJECT_ORBITCOMPONENT_H
