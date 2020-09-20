#include <cg/entities/components/CollisionComponent.h>
#include "cg/entities/systems/MovementSystem.h"
#include "cg/entities/components/VelocityComponent.h"

MovementSystem::MovementSystem(EntityManager &entityManager, GLContext &context)
    : System(entityManager), context(context) {
}

MovementSystem::~MovementSystem() {
}

void MovementSystem::init() {
}

void MovementSystem::update() {
    for (auto& pair : entityManager.getComponents<VelocityComponent>()) {
        auto entity = pair.first;
        auto velocity = reinterpret_cast<VelocityComponent *>(pair.second);
        auto transform = entityManager.getComponent<TransformComponent>(entity);

        if (entity->id == context.player->id) {
            if (entityManager.hasComponent<CollisionComponent>(entity)) {
                auto collision = dynamic_cast<CollisionComponent *>(entityManager.getComponent<CollisionComponent>(entity));
                if (collision->collidingWith) {
                    // 1. Check if player is already attached to colliding with
                    // 2. If not, attach it
                    // 3. Implement follow for both position and rotation
                    velocity->attachedTo = collision->collidingWith;

                    auto collisionTransform = entityManager.getComponent<TransformComponent>(velocity->attachedTo);
                    transform->position += collisionTransform->position - collisionTransform->previousPosition;
                }
            }
        }

        if (transform) {
            auto translation = velocity->velocity;
            translation -= velocity->gravity;
            transform->move(translation * static_cast<float>(context.getDeltaTime()));

            auto rotation = velocity->rotation * static_cast<float>(context.getDeltaTime());
            transform->rotate(rotation);

            if (glm::length(velocity->scaling) > 0) {
                auto scaling = velocity->scaling * static_cast<float>(context.getDeltaTime());
                transform->scale(glm::normalize(scaling));
            }

            if (velocity->customUpdate.has_value()) {
                velocity->customUpdate.value()(&entityManager, entity);
            }
        }
    }
}
