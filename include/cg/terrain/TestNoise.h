#ifndef CG1_PROJECT_TESTNOISE_H
#define CG1_PROJECT_TESTNOISE_H

#include <cg/terrain/Noise.h>
#include <cstdint>


class TestNoise : public Noise {
public:
    TestNoise(uint64_t seed) : Noise(seed) {}

    void reseed(uint64_t seed) override {}

    double evaluate(double x, double y) override { printf("x = %f\n", x); return 0.01 * x; }
    double evaluate(double x, double y, double z) override { return 0.01 * x * y * z; }

    glm::vec3 normal(double x, double y, double zoom) override { return glm::vec3(0, 0, 0); }
};


#endif //CG1_PROJECT_TESTNOISE_H
