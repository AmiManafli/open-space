#ifndef CG1_PROJECT_VELOCITYCOMPONENT_H
#define CG1_PROJECT_VELOCITYCOMPONENT_H


#include <cg/GLHeader.h>
#include <cg/entities/Component.h>

class VelocityComponent : public Component {
public:
    explicit VelocityComponent(glm::vec3 velocity);
    VelocityComponent(glm::vec3 rotation, glm::vec3 rotationPoint);

    glm::vec3 velocity {};

    glm::vec3 rotation;
    glm::vec3 rotationPoint;

    glm::vec3 scaling;
};


#endif //CG1_PROJECT_VELOCITYCOMPONENT_H
