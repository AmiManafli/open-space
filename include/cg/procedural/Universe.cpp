#include "Universe.h"

Universe::Universe() : Universe(UNIVERSE_SEED) {
}

Universe::Universe(SeedType seed) : seed({ seed }) {
    random = std::mt19937(seed);
}

Galaxy Universe::getGalaxy(SeedType x, SeedType y, SeedType z) {
    return Galaxy(seed, x, y, z);
}
