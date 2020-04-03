#ifndef CG1_PROJECT_SYSTEM_H
#define CG1_PROJECT_SYSTEM_H

#include <cg/GLContext.h>
#include "EntityManager.h"

class System {
public:
    System(EntityManager *entityManager);
    virtual ~System() {}

    virtual void init() = 0;
    virtual void update() = 0;

protected:
    EntityManager *entityManager;
};

#endif //CG1_PROJECT_SYSTEM_H
