#ifndef CG1_PROJECT_SKYBOXSTAR_H
#define CG1_PROJECT_SKYBOXSTAR_H

#include <cg/entities/components/MeshComponent.h>

class SkyboxStar : public MeshComponent {
public:
    SkyboxStar(float size, ShaderProgram *shaderProgram);
};

#endif //CG1_PROJECT_SKYBOXSTAR_H
