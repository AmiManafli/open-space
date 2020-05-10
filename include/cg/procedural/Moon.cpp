#include "Moon.h"

Moon::Moon(std::vector<SeedType> seed, SeedType index) {
    seed.emplace_back(index);

    auto seedSequence = std::seed_seq(seed.begin(), seed.end());
    random = std::mt19937(seedSequence);
}
