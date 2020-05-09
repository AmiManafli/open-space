//
// Created by amina on 5/8/2020.
//

#ifndef CG1_PROJECT_COLLISIONSYSTEM_H
#define CG1_PROJECT_COLLISIONSYSTEM_H

#include <cg/entities/System.h>
#include <cg/entities/EntityManager.h>
#include <cg/entities/Entity.h>
#include <cg/collision/BoundingSphere.h>

class CollisionSystem : public System {
public:
    CollisionSystem(EntityManager *entityManager, Entity *object, BoundingSphere boundingSphere);

    void init() override;
    void update() override;

private:
    Entity *object;
    BoundingSphere objectBoundingSphere;
    CameraComponent *cameraComponent;
    TransformComponent *transformComponent;
};


#endif //CG1_PROJECT_COLLISIONSYSTEM_H
