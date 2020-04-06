#include "cg/entities/systems/MovementSystem.h"

MovementSystem::MovementSystem(EntityManager *entityManager, GLContext *context) : System(entityManager), context(context) {
}

MovementSystem::~MovementSystem() {
}

void MovementSystem::init() {
}

void MovementSystem::update() {
    for (auto& pair : entityManager->getVelocityComponents()) {
        auto entityId = pair.first;
        auto component = pair.second;
        auto transformComponent = entityManager->getTransformComponent(entityId);

        if (component && transformComponent) {
            if (transformComponent->position.x > 30) {
                component->velocity = glm::vec3(-1, 0, 0);
            }
            if (transformComponent->position.x < -30) {
                component->velocity = glm::vec3(1, 0, 0);
            }
            auto translate = component->velocity * (float) context->getDeltaTime();
            transformComponent->position += translate;
            transformComponent->model = glm::translate(transformComponent->model, translate);
        }
    }
}
