#ifndef CG1_PROJECT_COLLISIONCOMPONENT_H
#define CG1_PROJECT_COLLISIONCOMPONENT_H


#include <cg/entities/Component.h>
#include <cg/collision/BoundingSphere.h>
#include <cg/entities/Entity.h>

class CollisionComponent : public Component {
public:
    enum Type {
        Sphere,
        Box,
    };

    CollisionComponent(Type type, BoundingSphere boundingSphere);

    Type type;
    BoundingSphere boundingSphere;
    Entity *collidingWith;
};


#endif //CG1_PROJECT_COLLISIONCOMPONENT_H
