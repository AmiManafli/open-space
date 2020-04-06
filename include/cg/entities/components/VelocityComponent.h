#ifndef CG1_PROJECT_VELOCITYCOMPONENT_H
#define CG1_PROJECT_VELOCITYCOMPONENT_H


#include <cg/GLHeader.h>
#include <cg/entities/Component.h>
#include <optional>

class VelocityComponent : public Component {
public:
    explicit VelocityComponent(glm::vec3 position);
    VelocityComponent(glm::vec3 rotation, glm::vec3 rotationPoint);

    std::optional<glm::vec3> position;

    std::optional<glm::vec3> rotation;
    std::optional<glm::vec3> rotationPoint;

    std::optional<glm::vec3> scaling;

    std::optional<std::function<void()>> customUpdate;

    void update();
};


#endif //CG1_PROJECT_VELOCITYCOMPONENT_H
