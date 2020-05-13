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
    OpenSimplexNoise(uint64_t seed, float strength, float roughness, glm::vec3 center);

    void reseed(uint64_t seed) override;

    double evaluate(double x, double y) override;
    double evaluate(double x, double y, double z) override;

    glm::vec3 normal(double x, double y, double zoom) override;

private:
    uint64_t seed;
    osn::OpenSimplexNoise *noise;

    float strength;
    float roughness;
    glm::vec3 center;
};


#endif //CG1_PROJECT_OPENSIMPLEXNOISE_H
