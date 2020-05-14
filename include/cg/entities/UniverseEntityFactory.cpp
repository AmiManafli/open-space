#include <cg/IcoSphere.h>
#include <cg/PlanetGenerator.h>
#include "UniverseEntityFactory.h"
#include "EntityBuilder.h"

UniverseEntityFactory::UniverseEntityFactory(EntityManager &entityManager, Universe &universe, GLContext &context)
        : entityManager(entityManager), universe(universe), context(context) {
}

Entity *UniverseEntityFactory::createEntities(std::vector<SolarSystem> solarSystems) {
    for (auto& system : solarSystems) {
        createEntities(system);
    }
    return nullptr;
}

Entity *UniverseEntityFactory::createEntities(SolarSystem &system) {
    auto entity = EntityBuilder::create()
            ->withTransform(system.getPosition())
            ->build(&entityManager);

    Entity *parent = nullptr;
    for (auto& star : system.getStars()) {
        parent = createEntities(star);
    }
    auto transform = entityManager.getComponent<TransformComponent>(parent);
    for (auto& planet : system.getPlanets()) {
        createEntities(transform, planet);
    }
    return entity;
}

Entity *UniverseEntityFactory::createEntities(Star &star) {
    Entity* entity = EntityBuilder::create()
            ->withTransform(star.position)
            ->withMesh(new IcoSphere(1, 3, star.color, 11, context.starProgram))
            ->withScale(star.radius)
            ->withPointLight(glm::vec3(0.05), star.color, glm::vec3(1.0), 1.0, 0.0014, 0.000007)
            ->withSphereCollision(star.radius)
            ->isSelectable()
            ->build(&entityManager);
    return entity;
}

Entity *UniverseEntityFactory::createEntities(TransformComponent *parent, Planet &planet) {
    Entity* entity = EntityBuilder::create()
            ->withTransform(planet.position)
            ->withOrbit(parent, planet.semiMajorAxis, planet.semiMinorAxis, planet.orbitSpeed, planet.orbitAngle)
            ->withMesh(new PlanetGenerator(0, *context.planetProgram))
            ->withSphereCollision(planet.radius)
            ->withScale(planet.radius)
            ->withMass(10)
            ->isSelectable()
            ->build(&entityManager);
    return entity;
}

Entity *UniverseEntityFactory::createEntities(Entity* parent, Moon &moon) {
    Entity* entity = nullptr;
    return entity;
}
