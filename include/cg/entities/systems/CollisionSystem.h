#ifndef CG1_PROJECT_COLLISIONSYSTEM_H
#define CG1_PROJECT_COLLISIONSYSTEM_H

#include <cg/entities/System.h>
#include <cg/entities/EntityManager.h>
#include <cg/entities/Entity.h>
#include <cg/collision/BoundingSphere.h>

class CollisionSystem : public System {
public:
    CollisionSystem(EntityManager &entityManager);

    void init(Entity *object);
    void update() override;

private:
    Entity *object;
    CameraComponent *cameraComponent;
    CollisionComponent *collisionComponent;
    TransformComponent *transformComponent;
};


#endif //CG1_PROJECT_COLLISIONSYSTEM_H
