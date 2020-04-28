#ifndef CG1_PROJECT_GRAVITYSYSTEM_H
#define CG1_PROJECT_GRAVITYSYSTEM_H

#include <cg/entities/System.h>

class GravitySystem : public System {
public:
    constexpr static const double NEWTON_G = 6.6743e-11;

    GravitySystem(EntityManager *entityManager);
    ~GravitySystem() override;

    void init() override;
    void update() override;
};

#endif //CG1_PROJECT_GRAVITYSYSTEM_H
