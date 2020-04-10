#ifndef CG1_PROJECT_OPENSIMPLEXNOISE_H
#define CG1_PROJECT_OPENSIMPLEXNOISE_H

#include <cg/terrain/Noise.h>
#include <cstdint>


namespace osn {
    class OpenSimplexNoise;
}

class OpenSimplexNoise : public Noise {
public:
    OpenSimplexNoise();
    OpenSimplexNoise(uint64_t seed);

    void reseed(uint64_t seed) override;

    double evaluate(float x, float y) override;
    double evaluate(float x, float y, float z) override;

    glm::vec3 normal(float x, float y, double zoom) override;

private:
    uint64_t seed;
    osn::OpenSimplexNoise *noise;
};


#endif //CG1_PROJECT_OPENSIMPLEXNOISE_H
