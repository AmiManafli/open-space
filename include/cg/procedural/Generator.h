#ifndef CG1_PROJECT_GENERATOR_H
#define CG1_PROJECT_GENERATOR_H

#include <limits>
#include <random>
#include <cg/GLHeader.h>

typedef int SeedType;

const SeedType UNIVERSE_SEED = 1;

const SeedType MIN_SEED = std::numeric_limits<SeedType>::min();
const SeedType MAX_SEED = std::numeric_limits<SeedType>::max();

const SeedType PLANET_SEED = 0;
const SeedType STAR_SEED = 1;

/// Solar systems
const int SOLAR_SYSTEM_GRID_SIZE = 1;
const int SOLAR_SYSTEM_CELL_SIZE = 10000;

/// Moon configuration
const int MAX_MOONS = 3;
const int AVERAGE_MOONS = 1;
const int MOONS_STANDARD_DEVIATION = 1;

/// Planet configuration
const int MAX_PLANETS = 20;
const int AVERAGE_PLANETS = 8;
const int PLANETS_STANDARD_DEVIATION = 2;

double normalized(SeedType value);
double gaussian(SeedType value, double min, double max, double mean, double sigma);
double limit(SeedType value, double min, double max);

#endif //CG1_PROJECT_GENERATOR_H
