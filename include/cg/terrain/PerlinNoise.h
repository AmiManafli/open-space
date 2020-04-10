#ifndef CG1_PROJECT_PERLINNOISE_H
#define CG1_PROJECT_PERLINNOISE_H


#include <cstdint>
#include <cg/terrain/Noise.h>
#include <PerlinNoise.hpp>


class PerlinNoise : public Noise {
public:
    PerlinNoise();
    PerlinNoise(uint64_t seed);
    
    void reseed(uint64_t seed) override;

    double evaluate(float x, float y) override;
    double evaluate(float x, float y, float z) override;

    glm::vec3 normal(float x, float y, double zoom) override;

private:
    uint64_t seed;
    siv::PerlinNoise *noise;
};


#endif //CG1_PROJECT_PERLINNOISE_H
