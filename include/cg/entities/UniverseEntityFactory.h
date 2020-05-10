#ifndef CG1_PROJECT_UNIVERSEENTITYFACTORY_H
#define CG1_PROJECT_UNIVERSEENTITYFACTORY_H

#include <cg/procedural/Universe.h>
#include <cg/procedural/Moon.h>
#include <cg/GLContext.h>

class UniverseEntityFactory {
public:
    UniverseEntityFactory(EntityManager &entityManager, Universe &universe, GLContext &context);

    Entity* createEntities(std::vector<SolarSystem> solarSystems);
    Entity* createEntities(SolarSystem& solarSystem);

    Entity* createEntities(Star& star);
    Entity* createEntities(TransformComponent *parent, Planet& planet);

    Entity* createEntities(Entity* parent, Moon& moon);

private:
    EntityManager &entityManager;
    Universe &universe;
    GLContext &context;
};


#endif //CG1_PROJECT_UNIVERSEENTITYFACTORY_H
