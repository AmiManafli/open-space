#include <cg/entities/components/CollisionComponent.h>
#include "cg/entities/systems/CollisionSystem.h"

CollisionSystem::CollisionSystem(EntityManager &entityManager) : System(entityManager) {
}

void CollisionSystem::init(Entity *object) {
    this->object = object;

    cameraComponent = entityManager.getComponent<CameraComponent>(object);
    collisionComponent = entityManager.getComponent<CollisionComponent>(object);
    transformComponent = entityManager.getComponent<TransformComponent>(object);
}

void CollisionSystem::update() {
    auto objectBoundingSphere = collisionComponent->boundingSphere;

    Entity *collidingWith = nullptr;

    for(auto& pair : entityManager.getComponents<CollisionComponent>()) {
        auto entity = pair.first;
        if (entity->id == object->id) continue;

        auto entityCollisionComponent = dynamic_cast<CollisionComponent *>(pair.second);
        bool foundCollision = objectBoundingSphere.intersects(entityCollisionComponent->boundingSphere);

        if (foundCollision) {
            // Calculate how much we need to move away to not be stuck inside the mesh
            auto position = transformComponent->position - entityCollisionComponent->boundingSphere.getPosition();
            float distance = objectBoundingSphere.getRadius() + entityCollisionComponent->boundingSphere.getRadius();

            transformComponent->position = entityCollisionComponent->boundingSphere.getPosition() + glm::normalize(position) * distance;

            collidingWith = entity;
        }
    }

    collisionComponent->collidingWith = collidingWith;
}