#ifndef CG1_PROJECT_STAR_H
#define CG1_PROJECT_STAR_H

#include "cg/procedural/Generator.h"

class Star {
public:
    Star(std::vector<SeedType> seed, SeedType index, double x, double y, double z);

    SeedType index;
    glm::dvec3 position;
    float radius;
    glm::dvec3 color;

private:
    std::vector<SeedType> seed;
    std::mt19937 random;
};


#endif //CG1_PROJECT_STAR_H
