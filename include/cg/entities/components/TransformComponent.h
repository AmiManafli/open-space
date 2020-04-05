#ifndef CG1_PROJECT_TRANSFORMCOMPONENT_H
#define CG1_PROJECT_TRANSFORMCOMPONENT_H

#include <cg/GLHeader.h>
#include <cg/entities/Component.h>

class TransformComponent : public Component {
public:
    TransformComponent(float x, float y, float z);
    TransformComponent(glm::vec3 position);

    void scale(float factor);

    glm::vec3 position;
    glm::mat4 model;
};

#endif //CG1_PROJECT_TRANSFORMCOMPONENT_H
