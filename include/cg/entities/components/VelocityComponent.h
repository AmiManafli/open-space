#ifndef CG1_PROJECT_VELOCITYCOMPONENT_H
#define CG1_PROJECT_VELOCITYCOMPONENT_H


#include <cg/GLHeader.h>
#include <cg/entities/Component.h>

class VelocityComponent : public Component {
public:
    VelocityComponent(glm::vec3 velocity);

    glm::vec3 velocity;
};


#endif //CG1_PROJECT_VELOCITYCOMPONENT_H
