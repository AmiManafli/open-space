#include "cg/terrain/OpenSimplexNoise.h"

#include <OpenSimplexNoise.h>


OpenSimplexNoise::OpenSimplexNoise() : OpenSimplexNoise(1) {
}

OpenSimplexNoise::OpenSimplexNoise(uint64_t seed) : seed(seed), noise(new osn::OpenSimplexNoise(seed)) {
}

double OpenSimplexNoise::evaluate(float x, float y) {
    return noise->Evaluate(x, y);
}

double OpenSimplexNoise::evaluate(float x, float y, float z) {
    return noise->Evaluate(x, y, z);
}
