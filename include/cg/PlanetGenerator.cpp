#include <cg/terrain/OpenSimplexNoise.h>
#include "PlanetGenerator.h"

PlanetSettings getDefaultPlanetSettings() {
    PlanetSettings settings;
    settings.seed = 0;
    settings.radius = 1;
    settings.subdivision = 0;
    settings.sphereFactor = 1.0;
    return settings;
}

PlanetNoiseSettings getDefaultPlanetNoiseSettings() {
    PlanetNoiseSettings settings;
    settings.noiseType = PlanetNoiseType::Simple;
    settings.enabled = true;
    settings.useFirstLayerAsMask = false;
    settings.strength = 1;
    settings.baseRoughness = 1;
    settings.roughness = 2;
    settings.persistence = 0.5;
    settings.minValue = 0.1;
    settings.weightMultiplier = 0.8;
    settings.center = glm::vec3(0);
    settings.layers = 0;
    return settings;
}

PlanetGenerator::PlanetGenerator(PlanetSettings settings, ShaderProgram &shaderProgram)
        : settings(settings), shaderProgram(shaderProgram) {
    updateSettings(settings);
}

void PlanetGenerator::updateSettings(PlanetSettings settings) {
    this->settings = settings;

    createMeshes();
}

void PlanetGenerator::createMeshes() {
    meshes.clear();

    auto directions = { FrontFace, BackFace, LeftFace, RightFace, UpFace, DownFace };

    float maxHeight = 0.0f;

    for (auto &direction : directions) {
        settings.direction = direction;
        auto mesh = new PlanetSide(settings, shaderProgram);
        meshes.emplace_back(mesh);

        if (mesh->maxHeight > maxHeight) {
            maxHeight = mesh->maxHeight;
        }
    }

    for (auto &mesh : meshes) {
        auto planetSide = dynamic_cast<PlanetSide *>(mesh);
        planetSide->maxHeight = maxHeight;
    }
}

