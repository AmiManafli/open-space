#include "cg/terrain/OpenSimplexNoise.h"

#include <OpenSimplexNoise.h>


OpenSimplexNoise::OpenSimplexNoise() : OpenSimplexNoise(1) {
}

OpenSimplexNoise::OpenSimplexNoise(uint64_t seed) : Noise(seed), noise(new osn::OpenSimplexNoise(seed)) {
}

void OpenSimplexNoise::reseed(uint64_t seed) {
    this->seed = seed;
    delete noise;
    noise = new osn::OpenSimplexNoise(seed);
}

double OpenSimplexNoise::evaluate(double x, double y) {
    auto offset = 2000.0;
    return noise->Evaluate(x + offset, y + offset);
}

double OpenSimplexNoise::evaluate(double x, double y, double z) {
    return (1.0 + noise->Evaluate(x, y, z)) / 2.0;
}

glm::vec3 OpenSimplexNoise::normal(double x, double y, double zoom) {
    auto dx = evaluate(x + 1, y) - evaluate(x, y);
    auto dz = evaluate(x, y + 1) - evaluate(x, y);

    auto vx = glm::vec3(1, dx, 0);
    auto vz = glm::vec3(0, dz, 1);

    return glm::normalize(glm::cross(vz, vx));
}
