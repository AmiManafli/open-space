#ifndef CG1_PROJECT_GALAXY_H
#define CG1_PROJECT_GALAXY_H

#include <random>
#include "SolarSystem.h"

class Galaxy {
public:
    Galaxy(std::vector<SeedType> seed, SeedType x, SeedType y, SeedType z);

    /**
     * Get all solar systems inside this solar systems cell.
     * @param x solar systems cell X coordinate.
     * @param y solar systems cell Y coordinate.
     * @param z solar systems cell Z coordinate.
     * @return Vector of solar systems.
     */
    std::vector<SolarSystem> getSolarSystems(SeedType x, SeedType y, SeedType z);

private:
    std::vector<SeedType> seed;
    SeedType x, y, z;
    std::mt19937 random;
};

#endif //CG1_PROJECT_GALAXY_H
