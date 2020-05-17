#ifndef CG1_PROJECT_GRAVITYSYSTEM_H
#define CG1_PROJECT_GRAVITYSYSTEM_H

#include <cg/entities/System.h>

class GravitySystem : public System {
public:
    constexpr static const double NEWTON_G = 6.6743e-11;

    GravitySystem(EntityManager *entityManager, Entity *object);
    ~GravitySystem() override;

    void init() override;
    void update() override;

private:
    Entity *object;
    TransformComponent *objectTransform = nullptr;
    VelocityComponent *objectVelocity = nullptr;
    MassComponent *objectMass = nullptr;
};

#endif //CG1_PROJECT_GRAVITYSYSTEM_H
