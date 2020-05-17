#ifndef CG1_PROJECT_PERLINNOISE_H
#define CG1_PROJECT_PERLINNOISE_H


#include <cstdint>
#include <cg/terrain/Noise.h>
#include <PerlinNoise.hpp>


class PerlinNoise : public Noise {
public:
    PerlinNoise();
    PerlinNoise(uint64_t seed);
    ~PerlinNoise() override;
    
    void reseed(uint64_t seed) override;

    double evaluate(double x, double y) override;
    double evaluate(double x, double y, double z) override;

    glm::vec3 normal(double x, double y, double zoom) override;

private:
    uint64_t seed;
    siv::PerlinNoise *noise;
};


#endif //CG1_PROJECT_PERLINNOISE_H
