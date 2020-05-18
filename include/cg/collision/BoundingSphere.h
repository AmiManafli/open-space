#ifndef CG1_PROJECT_BOUNDINGSPHERE_H
#define CG1_PROJECT_BOUNDINGSPHERE_H

#include <cg/entities/components/TransformComponent.h>

class BoundingSphere {
public:
    BoundingSphere(float radius, TransformComponent &transformComponent);

    bool intersects(BoundingSphere boundingSphere);

    glm::vec3 getPosition() { return transformComponent.position; }

    float getRadius() { return radius; }

private:
    float radius;
    TransformComponent &transformComponent;
};


#endif //CG1_PROJECT_BOUNDINGSPHERE_H
