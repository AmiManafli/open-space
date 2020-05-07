#ifndef CG1_PROJECT_UNIVERSE_H
#define CG1_PROJECT_UNIVERSE_H

#include "cg/procedural/Generator.h"
#include "Galaxy.h"
#include "SolarSystem.h"
#include "Planet.h"
#include "Star.h"

class Universe {
public:
    Universe();
    explicit Universe(SeedType seed);

    Galaxy getGalaxy(SeedType x, SeedType y, SeedType z);

private:
    std::vector<SeedType> seed;
    std::mt19937 random;
};

#endif //CG1_PROJECT_UNIVERSE_H
