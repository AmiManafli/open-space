#ifndef CG1_PROJECT_PLANET_H
#define CG1_PROJECT_PLANET_H

#include <cg/entities/Entity.h>
#include <cg/entities/EntityManager.h>
#include "cg/procedural/Generator.h"
#include "Moon.h"
#include "Star.h"

class Planet {
public:
    Planet(std::vector<SeedType> seed, Star &star, SeedType index);

    Moon getMoon(int index);

    SeedType index;
    glm::dvec3 position;
    glm::vec3 color;
    float radius;
    float semiMajorAxis;
    float semiMinorAxis;
    float orbitSpeed;
    float orbitAngle;
    SeedType planetSeed;
    uint32_t mass;
    glm::vec3 rotation;

    std::vector<Moon> moons;

private:
    std::vector<SeedType> seed;
    std::mt19937 random;
};

#endif //CG1_PROJECT_PLANET_H
