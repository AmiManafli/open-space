#ifndef CG1_PROJECT_PLANETEDITOR_H
#define CG1_PROJECT_PLANETEDITOR_H

#include <cg/GLHeader.h>
#include <cg/entities/EntityManager.h>
#include <cg/GLContext.h>


enum PlanetNoiseType {
    POpenSimplex,
    PPerlin,
};

struct PlanetSettings {
    double radius;
    uint16_t subdivision;
    int seed;
    double maxAmplitude;
    double frequency;
    int octaves;
    double persistence;
    PlanetNoiseType noiseType;
    double redistribution;
};

class PlanetEditor {
public:
    PlanetEditor(EntityManager &entityManager, GLContext &context);

    void render();

private:
    EntityManager &entityManager;
    GLContext &context;

    PlanetSettings settings;
};

#endif //CG1_PROJECT_PLANETEDITOR_H
