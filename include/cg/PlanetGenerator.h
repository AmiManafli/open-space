#ifndef CG1_PROJECT_PLANETGENERATOR_H
#define CG1_PROJECT_PLANETGENERATOR_H

#include <cg/GLHeader.h>
#include <cg/entities/components/MeshComponent.h>
#include <cg/terrain/Noise.h>
#include <cg/PlanetSettings.h>
#include <cg/PlanetSide.h>


PlanetSettings getDefaultPlanetSettings();
PlanetNoiseSettings getDefaultPlanetNoiseSettings();

class PlanetGenerator {
public:
    PlanetGenerator(PlanetSettings settings, ShaderProgram &shaderProgram);

    std::vector<MeshComponent *> getMeshes() { return meshes; }

    void updateSettings(PlanetSettings settings);

private:
    PlanetSettings settings;
    ShaderProgram &shaderProgram;

    std::vector<MeshComponent *> meshes;

    void createMeshes();
};

#endif //CG1_PROJECT_PLANETGENERATOR_H
