#ifndef CG1_PROJECT_OPENSIMPLEXNOISE_H
#define CG1_PROJECT_OPENSIMPLEXNOISE_H

#include <cg/terrain/Noise.h>
#include <cstdint>
#include <cg/PlanetGenerator.h>


namespace osn {
    class OpenSimplexNoise;
}

class OpenSimplexNoise : public Noise {
public:
    explicit OpenSimplexNoise(PlanetSettings &planetSettings, PlanetNoiseSettings &settings);

    void reseed(uint64_t seed) override;

    double evaluate(double x, double y) override;
    double evaluate(double x, double y, double z) override;

    glm::vec3 normal(double x, double y, double zoom) override;

private:
    PlanetSettings &planetSettings;
    PlanetNoiseSettings &settings;
    osn::OpenSimplexNoise *noise;

    double evaluateSimple(double x, double y, double z);
    double evaluateRidged(double x, double y, double z);
};


#endif //CG1_PROJECT_OPENSIMPLEXNOISE_H
