#pragma once

#include <cg/GLContext.h>
#include "EntityManager.h"

class System {
public:
    System(EntityManager &entityManager) : entityManager(entityManager) {}
    virtual ~System() {}

    virtual void update() = 0;

protected:
    EntityManager &entityManager;
};
