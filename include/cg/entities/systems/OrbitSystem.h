#ifndef CG1_PROJECT_ORBITSYSTEM_H
#define CG1_PROJECT_ORBITSYSTEM_H

#include <cg/entities/System.h>

class OrbitSystem : public System {
public:
    OrbitSystem(EntityManager *entityManager, GLContext *context);
    ~OrbitSystem() override;

    void init() override;
    void update() override;

private:
    GLContext *context;
};

#endif //CG1_PROJECT_ORBITSYSTEM_H
