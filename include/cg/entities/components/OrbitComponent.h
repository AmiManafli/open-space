#ifndef CG1_PROJECT_ORBITCOMPONENT_H
#define CG1_PROJECT_ORBITCOMPONENT_H

#include <cg/GLHeader.h>

class OrbitComponent {
public:
    OrbitComponent(glm::vec3 parentPosition, double semiMajorAxis, double semiMinorAxis, double speed, double startTheta);

    double semiMajorAxis;
    double semiMinorAxis;

    double startTheta;
    double theta;

    double speed;

    double eccentricity;
    double focusCord;
    double width;
    double height;

    glm::vec3 parentPosition;
    glm::vec2 startPosition;

    double calculateRadius(double theta);
    glm::vec3 getPosition();

    void update(glm::vec3 parentPosition, double semiMajorAxis, double semiMinorAxis, double speed);
};

#endif //CG1_PROJECT_ORBITCOMPONENT_H
