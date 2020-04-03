#ifndef CG1_PROJECT_POSITIONCOMPONENT_H
#define CG1_PROJECT_POSITIONCOMPONENT_H

#include <cg/GLHeader.h>
#include <cg/entities/Component.h>

class PositionComponent : public Component {
public:
    PositionComponent(float x, float y, float z);
    PositionComponent(glm::vec3 position);

    glm::vec3 position;
    glm::mat4 model;
};

#endif //CG1_PROJECT_POSITIONCOMPONENT_H
