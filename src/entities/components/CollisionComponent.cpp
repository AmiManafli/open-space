#include "cg/entities/components/CollisionComponent.h"

CollisionComponent::CollisionComponent(CollisionComponent::Type type, BoundingSphere boundingSphere)
        : type(type), boundingSphere(boundingSphere) {

}
