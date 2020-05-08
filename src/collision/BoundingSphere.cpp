//
// Created by amina on 5/8/2020.
//

#include "cg/collision/BoundingSphere.h"

BoundingSphere::BoundingSphere(float radius, TransformComponent &transformComponent)
        : radius(radius), transformComponent(transformComponent) {
}

bool BoundingSphere::intersects(BoundingSphere boundingSphere) {
    auto distance = glm::length(transformComponent.position - boundingSphere.getPosition());
    if(distance <= boundingSphere.radius + this->radius) {
        printf("Spheres intersected! Distance was = %f\n", distance);
        return true;
    }
    printf("Spheres didn't intersect! Distance was = %f\n", distance);
    return false;
};

