#ifndef CG1_PROJECT_ORBITCOMPONENT_H
#define CG1_PROJECT_ORBITCOMPONENT_H

#include <cg/GLHeader.h>

class OrbitComponent {
public:
    OrbitComponent(Entity *parent, double semiMajorAxis, double semiMinorAxis);

    double semiMajorAxis;
    double semiMinorAxis;

    double theta;

    double eccentricity;
    double focusCord;
    double width;
    double height;

    double calculateRadius(double theta);
    glm::vec2 getPosition();

    void update(double semiMajorAxis, double semiMinorAxis);
};

#endif //CG1_PROJECT_ORBITCOMPONENT_H
