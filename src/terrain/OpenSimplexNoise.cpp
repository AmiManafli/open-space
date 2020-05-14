#include "cg/terrain/OpenSimplexNoise.h"

#include <OpenSimplexNoise.h>

OpenSimplexNoise::OpenSimplexNoise(PlanetSettings &planetSettings, PlanetNoiseSettings &settings)
        : Noise(planetSettings.seed), noise(new osn::OpenSimplexNoise(seed)), planetSettings(planetSettings), settings(settings) {
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
    if (settings.noiseType == PlanetNoiseType::Simple) {
        return evaluateSimple(x, y, z);
    } else if (settings.noiseType == PlanetNoiseType::Ridged) {
        return evaluateRidged(x, y, z);
    } else {
        throw std::runtime_error("Invalid planet noise type!");
    }
}

double OpenSimplexNoise::evaluateSimple(double x, double y, double z) {
    double height = 0.0;
    double frequency = settings.baseRoughness;
    double amplitude = 1.0;

    for (int layer = 0; layer < settings.layers; layer++) {
        auto point = glm::vec3(x, y, z) * static_cast<float>(frequency) + settings.center;
        auto value = (1.0 + noise->Evaluate(point.x, point.y, point.z)) * 0.5;
        height += value * amplitude;
        frequency *= settings.roughness;
        amplitude *= settings.persistence;
    }
    height = glm::max(0.0, height - settings.minValue);
    return height * settings.strength;
}

double OpenSimplexNoise::evaluateRidged(double x, double y, double z) {
    double height = 0.0;
    double frequency = settings.baseRoughness;
    double amplitude = 1.0;
    float weight = 1;

    for (int layer = 0; layer < settings.layers; layer++) {
        auto point = glm::vec3(x, y, z) * static_cast<float>(frequency) + settings.center;
        auto value = 1.0 - glm::abs(noise->Evaluate(point.x, point.y, point.z));

        value *= weight;
        weight = glm::clamp(value * settings.weightMultiplier, 0.0, 1.0);

        height += value * value * amplitude;
        frequency *= settings.roughness;
        amplitude *= settings.persistence;
    }
    height = glm::max(0.0, height - settings.minValue);
    return height * settings.strength;
}

glm::vec3 OpenSimplexNoise::normal(double x, double y, double zoom) {
    auto dx = evaluate(x + 1, y) - evaluate(x, y);
    auto dz = evaluate(x, y + 1) - evaluate(x, y);

    auto vx = glm::vec3(1, dx, 0);
    auto vz = glm::vec3(0, dz, 1);

    return glm::normalize(glm::cross(vz, vx));
}
