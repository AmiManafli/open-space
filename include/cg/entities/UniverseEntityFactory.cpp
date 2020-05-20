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
    auto builder = EntityBuilder::create();
    auto entity = builder
            ->withTransform(system.getPosition())
            ->build(&entityManager);
    delete builder;

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
    auto builder = EntityBuilder::create();
    Entity* entity = builder
            ->withTransform(star.position)
            ->withMesh(new IcoSphere(1, 3, star.color, 11, context.starProgram))
            ->withScale(star.radius)
            ->withPointLight(glm::vec3(0.05), glm::vec3(0.96), glm::vec3(1.0), 1.0, 0.0014, 0.000007)
            ->withSphereCollision(star.radius)
            ->isSelectable()
            ->build(&entityManager);
    delete builder;
    return entity;
}

Entity *UniverseEntityFactory::createEntities(TransformComponent *parent, Planet &planet) {
    std::vector<PlanetNoiseSettings> noisePresets[2];
    noisePresets[0] = {
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
                            planet.noiseStrength,
                            1.0,
                            5.7,
                            0.6,
                            0.1,
                            1.0,
                            4,
                            glm::vec3(0.7, 1.7, 0.7),
                    },
    };
    noisePresets[1] = {
                {
                        Simple,
                        true,
                        false,
                        0.5f * planet.noiseStrength,
                        2.3,
                        2.1,
                        0.4,
                        1.0,
                        0.0,
                        6,
                        glm::vec3(0, 0, 0),
                },
                {
                        Ridged,
                        true,
                        true,
                        0.6f * planet.noiseStrength,
                        1.0,
                        2.0,
                        0.5,
                        0.1,
                        0.0,
                        1,
                        glm::vec3(0, 0, 0),
                },
    };
    auto settings = PlanetSettings {
        1.0,
        80,
        planet.planetSeed,
        1.0,
        MeshComponent::Material {
            planet.waterColor,
            glm::vec3(0.9),
            8,
        },
        MeshComponent::Material {
                planet.groundColor,
                glm::vec3(0.1),
                8,
        },
        MeshComponent::Material {
                planet.peakColor,
                glm::vec3(0.1),
                8,
        },
        FrontFace,
        noisePresets[planet.noisePreset],
    };

    auto planetVelocity = new VelocityComponent();
    planetVelocity->rotation = planet.rotation;

    PlanetGenerator planetGenerator(settings, *context.meshProgram);
    auto meshes = planetGenerator.getMeshes();
    auto builder = EntityBuilder::create();
    Entity* entity = builder
            ->withTransform(planet.position)
            ->withVelocity(planetVelocity)
            ->withOrbit(parent, planet.semiMajorAxis, planet.semiMinorAxis, planet.orbitSpeed, planet.orbitAngle)
            ->withMesh(meshes)
            ->withSphereCollision(planet.radius + meshes[0]->maxHeight)
            ->withScale(planet.radius)
            ->withMass(planet.mass)
            ->isSelectable()
            ->build(&entityManager);
    delete builder;
    return entity;
}

Entity *UniverseEntityFactory::createEntities(Entity* parent, Moon &moon) {
    Entity* entity = nullptr;
    return entity;
}
