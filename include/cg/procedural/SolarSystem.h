#ifndef CG1_PROJECT_SOLARSYSTEM_H
#define CG1_PROJECT_SOLARSYSTEM_H

#include <random>
#include "Planet.h"
#include "Star.h"

class SolarSystem {
public:
    SolarSystem(std::vector<SeedType> seed, SeedType x, SeedType y, SeedType z);

    glm::vec3 getPosition() { return glm::vec3(x, y, z); }

    Planet getPlanet(Star &star, SeedType index);
    Star getStar(SeedType index, double x, double y, double z);

    std::vector<Star> getStars() { return stars; }
    std::vector<Planet> getPlanets() { return planets; }

private:
    std::vector<SeedType> seed;
    std::mt19937 random;

    SeedType x, y, z;

    std::vector<Star> stars;
    std::vector<Planet> planets;
};

#endif //CG1_PROJECT_SOLARSYSTEM_H
