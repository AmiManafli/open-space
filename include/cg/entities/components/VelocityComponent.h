#ifndef CG1_PROJECT_VELOCITYCOMPONENT_H
#define CG1_PROJECT_VELOCITYCOMPONENT_H

#include <functional>
#include <cg/GLHeader.h>
#include <cg/entities/Component.h>
#include <optional>
#include "TransformComponent.h"
#include "LightComponent.h"

struct Entity;
class EntityManager;

class VelocityComponent : public Component {
public:
    VelocityComponent();
    explicit VelocityComponent(glm::vec3 velocity);
    VelocityComponent(glm::vec3 rotation, glm::vec3 rotationPoint);
    VelocityComponent(glm::vec3 velocity, glm::vec3 rotation, glm::vec3 rotationPoint);

    void update(EntityManager *entityManager, Entity *entity);

    glm::vec3 velocity;

    glm::vec3 gravity;

    glm::vec3 rotation;
    glm::vec3 rotationPoint;

    glm::vec3 scaling;

    std::optional<std::function<void(EntityManager *, Entity *)>> customUpdate;

    Entity *attachedTo;
};


#endif //CG1_PROJECT_VELOCITYCOMPONENT_H
