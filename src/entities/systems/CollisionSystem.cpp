//
// Created by amina on 5/8/2020.
//

#include <cg/entities/components/CollisionComponent.h>
#include "cg/entities/systems/CollisionSystem.h"

CollisionSystem::CollisionSystem(EntityManager *entityManager, Entity *object, BoundingSphere boundingSphere)
        : System(entityManager), object(object), objectBoundingSphere(boundingSphere) {

}


void CollisionSystem::init() {
    cameraComponent = entityManager->getComponent<CameraComponent>(object);
    transformComponent = entityManager->getComponent<TransformComponent>(object);
}


void CollisionSystem::update() {
    for(auto& pair : entityManager->getComponents<CollisionComponent>()) {
        auto entity = pair.first;
        auto entityCollisionComponent = dynamic_cast<CollisionComponent *>(pair.second);
        bool foundCollision = objectBoundingSphere.intersects(entityCollisionComponent->boundingSphere);

        if (foundCollision) {
            auto getAwayVec = (transformComponent->position - entityCollisionComponent->boundingSphere.getPosition());
            float getAwayVecLen = objectBoundingSphere.getRadius() + entityCollisionComponent->boundingSphere.getRadius();
            getAwayVec = glm::normalize(getAwayVec) * getAwayVecLen;

            transformComponent->position = entityCollisionComponent->boundingSphere.getPosition() + getAwayVec;
        }
    }
}