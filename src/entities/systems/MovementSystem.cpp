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
        auto velocity = pair.second;
        auto transform = entityManager->getTransformComponent(entityId);
        auto light = entityManager->getLightComponent(entityId);

        if (velocity && transform) {
            if (velocity->position.has_value()) {
                auto translate = velocity->position.value() * (float) context->getDeltaTime();
                transform->move(translate);
            }

            if (velocity->rotation.has_value()) {
                auto rotation = velocity->rotation.value() * (float) context->getDeltaTime();
                transform->rotate(glm::length(rotation), glm::normalize(rotation));
            }

            if (velocity->scaling.has_value()) {
                auto scaling = velocity->scaling.value() * (float) context->getDeltaTime();
                transform->scale(glm::normalize(scaling));
            }

            velocity->update(entityManager, entityId);
        }
    }
}
