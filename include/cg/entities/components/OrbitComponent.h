#ifndef CG1_PROJECT_ORBITCOMPONENT_H
#define CG1_PROJECT_ORBITCOMPONENT_H

#include <cg/GLHeader.h>

class OrbitComponent {
public:
    OrbitComponent(Entity *parent, double semiMajorAxis, double semiMinorAxis);

    double semiMajorAxis;
    double semiMinorAxis;

    double eccentricity;
    double focusCord;
    double width;
    double height;

    double calculateRadius(double theta);
    glm::vec2 position(double theta);

    void update(double semiMajorAxis, double semiMinorAxis);
};

#endif //CG1_PROJECT_ORBITCOMPONENT_H
