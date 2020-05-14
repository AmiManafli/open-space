#ifndef CG1_PROJECT_PLANETGENERATOR_H
#define CG1_PROJECT_PLANETGENERATOR_H

#include <cg/GLHeader.h>
#include <cg/entities/components/MeshComponent.h>
#include <cg/terrain/Noise.h>

enum PlanetNoiseType {
    Simple,
    Ridged,
};

struct PlanetNoiseSettings {
    PlanetNoiseType noiseType;

    bool enabled;
    bool useFirstLayerAsMask;

    float strength;
    float baseRoughness;
    float roughness;
    float persistence;
    float minValue;
    float weightMultiplier;

    int layers;
    glm::vec3 center;
};

struct PlanetSettings {
    float radius;
    int subdivision;
    int seed;
    std::vector<PlanetNoiseSettings> noiseSettings;
};

PlanetSettings getDefaultPlanetSettings();
PlanetNoiseSettings getDefaultPlanetNoiseSettings();

class PlanetGenerator : public MeshComponent {
public:
    PlanetGenerator(PlanetSettings settings, ShaderProgram &shaderProgram);
    ~PlanetGenerator() override;

    void subdivide(uint16_t subdivisions) override;

    void updateSettings(PlanetSettings settings);

private:
    std::vector<Noise *> noises;
    PlanetSettings settings;

    void createMesh();
    void createTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);

    void applyNoise();
    void updateNormals();
};

#endif //CG1_PROJECT_PLANETGENERATOR_H
