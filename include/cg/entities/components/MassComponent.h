#ifndef CG1_PROJECT_MASSCOMPONENT_H
#define CG1_PROJECT_MASSCOMPONENT_H

#include <cg/entities/Component.h>

class MassComponent : public Component {
public:
    MassComponent(float mass);

    float mass;
};

#endif //CG1_PROJECT_MASSCOMPONENT_H
