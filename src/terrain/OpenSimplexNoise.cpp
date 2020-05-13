#include "cg/terrain/OpenSimplexNoise.h"

#include <OpenSimplexNoise.h>


OpenSimplexNoise::OpenSimplexNoise() : OpenSimplexNoise(1, 1, 1, glm::vec3(0)) {
}

OpenSimplexNoise::OpenSimplexNoise(uint64_t seed, float strength, float roughness, glm::vec3 center)
        : Noise(seed), noise(new osn::OpenSimplexNoise(seed)), strength(strength), roughness(roughness), center(center) {
}

void OpenSimplexNoise::reseed(uint64_t seed) {
    this->seed = seed;
    delete noise;
    noise = new osn::OpenSimplexNoise(seed);
}

double OpenSimplexNoise::evaluate(double x, double y) {
    return (1.0 + noise->Evaluate(x, y)) / 2.0;
}

double OpenSimplexNoise::evaluate(double x, double y, double z) {
    auto point = glm::vec3(x, y, z) * roughness + center;
    auto value = (1.0 + noise->Evaluate(point.x, point.y, point.z)) * 0.5;
    return value * strength;
}

glm::vec3 OpenSimplexNoise::normal(double x, double y, double zoom) {
    auto dx = evaluate(x + 1, y) - evaluate(x, y);
    auto dz = evaluate(x, y + 1) - evaluate(x, y);

    auto vx = glm::vec3(1, dx, 0);
    auto vz = glm::vec3(0, dz, 1);

    return glm::normalize(glm::cross(vz, vx));
}
