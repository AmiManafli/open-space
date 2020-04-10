#ifndef CG1_PROJECT_NOISE_H
#define CG1_PROJECT_NOISE_H


#include <cstdint>
#include <cg/GLHeader.h>

class Noise {
public:
    Noise(uint64_t seed) : seed(seed) {}
    virtual ~Noise() {}

    uint64_t getSeed() { return seed; }
    virtual void reseed(uint64_t seed) = 0;

    virtual double evaluate(float x, float y) = 0;
    virtual double evaluate(float x, float y, float z) = 0;

    virtual glm::vec3 normal(float x, float y, double zoom) = 0;

protected:
    uint64_t seed;
};


#endif //CG1_PROJECT_NOISE_H
