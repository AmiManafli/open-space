#ifndef CG1_PROJECT_TRANSFORMCOMPONENT_H
#define CG1_PROJECT_TRANSFORMCOMPONENT_H

#include <cg/GLHeader.h>
#include <cg/entities/Component.h>

class TransformComponent : public Component {
public:
    TransformComponent();
    TransformComponent(glm::vec3 position);

    void move(float x, float y, float z);
    void move(glm::vec3 position);

    void scale(float factor);
    void scale(glm::vec3 scaling);

    void rotate(glm::vec3 angles);

    glm::vec3 position;
    glm::vec3 scaling;
    glm::vec3 rotation;

    glm::mat4 getModel();
};

#endif //CG1_PROJECT_TRANSFORMCOMPONENT_H
