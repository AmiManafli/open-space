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
            if (transformComponent->position.x > 30 || transformComponent->position.x < -30) {
                component->velocity = -component->velocity;
            }
            auto translate = component->velocity * (float) context->getDeltaTime();
            transformComponent->move(translate);

            auto rotation = component->rotation * (float) context->getDeltaTime();
            transformComponent->rotate(glm::length(rotation), glm::normalize(rotation));
        }
    }
}
