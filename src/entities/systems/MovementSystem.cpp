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
        auto transform = entityManager->getComponent<TransformComponent>(entityManager->getEntity(entityId));
        auto light = entityManager->getLightComponent(entityId);

        if (velocity && transform) {
            auto translation = velocity->position;
            translation -= velocity->gravity;
            transform->move(translation * static_cast<float>(context->getDeltaTime()));

            auto rotation = velocity->rotation * static_cast<float>(context->getDeltaTime());
            transform->rotate(rotation);

            if (glm::length(velocity->scaling) > 0) {
                auto scaling = velocity->scaling * static_cast<float>(context->getDeltaTime());
                transform->scale(glm::normalize(scaling));
            }

            velocity->update(entityManager, entityId);
        }
    }
}
