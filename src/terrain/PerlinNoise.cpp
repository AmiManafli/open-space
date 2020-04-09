#include "cg/terrain/PerlinNoise.h"


PerlinNoise::PerlinNoise() : PerlinNoise(1) {
}

PerlinNoise::PerlinNoise(uint64_t seed) : Noise(seed), noise(new siv::PerlinNoise(static_cast<uint32_t>(seed))) {
}

void PerlinNoise::reseed(uint64_t seed) {
    this->seed = seed;
    noise->reseed(static_cast<uint32_t>(seed));
}

double PerlinNoise::evaluate(float x, float y) {
    auto value = noise->accumulatedOctaveNoise2D(x / 1000.0, y / 1000.0, 8);
    return value;
}

double PerlinNoise::evaluate(float x, float y, float z) {
    return noise->noise3D(x, y, z);
}

glm::vec3 PerlinNoise::normal(float x, float y, double zoom) {
    x = x / 1000.0;
    y = y / 1000.0;
    auto dx = (evaluate((x + 1) / zoom, y / zoom) - evaluate(x / zoom, y / zoom)) / (1.0 / zoom);
    auto dz = (evaluate(x / zoom, (y + 1) / zoom) - evaluate(x / zoom, y / zoom)) / (1.0 / zoom);

    auto vx = glm::vec3(1, dx, 0);
    auto vz = glm::vec3(0, dz, 1);

    return glm::normalize(glm::cross(vz, vx));
}

