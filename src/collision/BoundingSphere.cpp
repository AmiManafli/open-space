#include "cg/collision/BoundingSphere.h"

BoundingSphere::BoundingSphere(float radius, TransformComponent &transformComponent)
        : radius(radius), transformComponent(transformComponent) {
}

bool BoundingSphere::intersects(BoundingSphere boundingSphere) {
    auto distance = glm::length(transformComponent.position - boundingSphere.getPosition());
    return (distance <= boundingSphere.radius + this->radius);
};

