#ifndef CG1_PROJECT_PLANETGENERATOR_H
#define CG1_PROJECT_PLANETGENERATOR_H

#include <cg/GLHeader.h>
#include <cg/entities/components/MeshComponent.h>
#include <cg/terrain/Noise.h>


struct PlanetSettings {
    float radius;
    int subdivision;
    int seed;

    float strength;
    float baseRoughness;
    float roughness;
    float persistence;
    int layers;
    glm::vec3 center;
};

PlanetSettings getDefaultPlanetSettings();

class PlanetGenerator : public MeshComponent {
public:
    PlanetGenerator(uint16_t subdivisions, ShaderProgram &shaderProgram);
    ~PlanetGenerator() override;

    void subdivide(uint16_t subdivisions) override;

    void updateSettings(PlanetSettings &settings);

private:
    Noise *noise;
    PlanetSettings settings;

    void createMesh();
    void createTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);

    void applyNoise();
    void updateNormals();

    static Noise *createNoise(PlanetSettings settings);
};

#endif //CG1_PROJECT_PLANETGENERATOR_H
