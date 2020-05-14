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
    auto settings = PlanetSettings {
        1.0,
        80,
        planet.planetSeed,
        FrontFace,
        {
                {
                    Simple,
                    true,
                    false,
                    0.06,
                    0.6,
                    2.5,
                    0.7,
                    1.3,
                    0.0,
                    4,
                    glm::vec3(0.7, 1.1, 1.8),
                },
                {
                    Ridged,
                    true,
                    true,
                    1.67,
                    1.0,
                    5.7,
                    0.6,
                    0.1,
                    1.0,
                    4,
                    glm::vec3(0.7, 1.7, 0.7),
                },
        }
    };

    auto planetVelocity = new VelocityComponent();
    planetVelocity->rotation = planet.rotation;

    PlanetGenerator planetGenerator(settings, *context.planetProgram);
    Entity* entity = EntityBuilder::create()
            ->withTransform(planet.position)
            ->withVelocity(planetVelocity)
            ->withOrbit(parent, planet.semiMajorAxis, planet.semiMinorAxis, planet.orbitSpeed, planet.orbitAngle)
            ->withMesh(planetGenerator.getMeshes())
            ->withSphereCollision(planet.radius)
            ->withScale(planet.radius)
            ->withMass(planet.mass)
            ->isSelectable()
            ->build(&entityManager);
    return entity;
}

Entity *UniverseEntityFactory::createEntities(Entity* parent, Moon &moon) {
    Entity* entity = nullptr;
    return entity;
}
