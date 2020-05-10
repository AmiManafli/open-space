#ifndef CG1_PROJECT_MOON_H
#define CG1_PROJECT_MOON_H

#include "cg/procedural/Generator.h"

class Moon {
public:
    Moon(std::vector<SeedType> seed, SeedType index);

private:
    SeedType universeSeed;
    SeedType galaxyX, galaxyY, galaxyZ;
    SeedType solarSystemX, solarSystemY, solarSystemZ;
    SeedType planetIndex;
    SeedType index;
    std::mt19937 random;
};


#endif //CG1_PROJECT_MOON_H
