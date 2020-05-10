#include "SolarSystem.h"

SolarSystem::SolarSystem(std::vector<SeedType> seed, SeedType x, SeedType y, SeedType z)
        : seed(seed), x(x), y(y), z(z) {
    seed.emplace_back(x);
    seed.emplace_back(y);
    seed.emplace_back(z);

    auto seedSequence = std::seed_seq(seed.begin(), seed.end());
    random = std::mt19937(seedSequence);

    // Do not allow binary star systems for now
    auto size = SOLAR_SYSTEM_CELL_SIZE / 2.0;

    double starX = x * SOLAR_SYSTEM_CELL_SIZE + size;
    double starY = y * SOLAR_SYSTEM_CELL_SIZE + size;
    double starZ = z * SOLAR_SYSTEM_CELL_SIZE + size;
    auto star = getStar(0, starX, starY, starZ);
    stars.emplace_back(star);

    auto numPlanets = glm::round(gaussian(random(), 0, MAX_PLANETS, AVERAGE_PLANETS, PLANETS_STANDARD_DEVIATION));
    for (int i = 0; i < numPlanets; i++) {
        planets.emplace_back(getPlanet(star, i));
    }
}

Planet SolarSystem::getPlanet(Star &star, SeedType index) {
    return Planet(seed, star, index);
}

Star SolarSystem::getStar(SeedType index, double x, double y, double z) {
    return Star(seed, index, x, y, z);
}
