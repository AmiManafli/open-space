#include "Star.h"

Star::Star(std::vector<SeedType> seed, SeedType index, double x, double y, double z)
        : seed(seed), index(index), position({ x, y, z }) {
    seed.emplace_back(STAR_SEED);
    seed.emplace_back(index);

    auto seedSequence = std::seed_seq(seed.begin(), seed.end());
    random = std::mt19937(seedSequence);

    // Star size
    radius = SOLAR_SYSTEM_CELL_SIZE / 30.0;

    // Star color
    color = glm::vec3(0.95);
}
