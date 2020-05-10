#include "Galaxy.h"

Galaxy::Galaxy(std::vector<SeedType> seed, SeedType x, SeedType y, SeedType z) : seed(seed), x(x), y(y), z(z) {
    seed.emplace_back(x);
    seed.emplace_back(y);
    seed.emplace_back(z);

    auto seedSequence = std::seed_seq(seed.begin(), seed.end());
    random = std::mt19937(seedSequence);
}

std::vector<SolarSystem> Galaxy::getSolarSystems(SeedType x, SeedType y, SeedType z) {
    return { SolarSystem(seed, x, y, z) };
}
