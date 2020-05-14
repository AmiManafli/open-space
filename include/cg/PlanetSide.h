#ifndef CG1_PROJECT_PLANETSIDE_H
#define CG1_PROJECT_PLANETSIDE_H

#include <cg/GLHeader.h>
#include <cg/entities/components/MeshComponent.h>
#include <cg/terrain/Noise.h>
#include <cg/PlanetSettings.h>

class PlanetSide : public MeshComponent {
public:
    PlanetSide(PlanetSettings settings, ShaderProgram &shaderProgram);
    ~PlanetSide() override;

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


#endif //CG1_PROJECT_PLANETSIDE_H
