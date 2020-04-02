#ifndef CG1_PROJECT_SYSTEM_H
#define CG1_PROJECT_SYSTEM_H

#include <GLContext.h>
#include "EntityManager.h"

class System {
public:
    System(EntityManager *manager);
    virtual ~System() {}

    virtual void init() = 0;
    virtual void update() = 0;

private:
    EntityManager *manager;
};

#endif //CG1_PROJECT_SYSTEM_H
