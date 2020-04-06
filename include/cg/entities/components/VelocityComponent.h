#ifndef CG1_PROJECT_VELOCITYCOMPONENT_H
#define CG1_PROJECT_VELOCITYCOMPONENT_H

#include <functional>
#include <cg/GLHeader.h>
#include <cg/entities/Component.h>
#include <optional>
#include "TransformComponent.h"

class VelocityComponent : public Component {
public:
    explicit VelocityComponent(glm::vec3 position);
    VelocityComponent(glm::vec3 rotation, glm::vec3 rotationPoint);

    std::optional<glm::vec3> position;

    std::optional<glm::vec3> rotation;
    std::optional<glm::vec3> rotationPoint;

    std::optional<glm::vec3> scaling;

    std::optional<std::function<void(VelocityComponent *, TransformComponent *)>> customUpdate;

    void update(TransformComponent *transform);
};


#endif //CG1_PROJECT_VELOCITYCOMPONENT_H
