#ifndef CG1_PROJECT_ICOSPHERE_H
#define CG1_PROJECT_ICOSPHERE_H

#include <cg/GLHeader.h>
#include <cg/entities/components/MeshComponent.h>


class IcoSphere : public MeshComponent {
public:
    IcoSphere(double radius, int subdivisions, ShaderProgram *shaderProgram);

private:
    double radius;
    int subdivisions;

    void generateMesh();

    glm::vec3 calculateNormal(int top, int prev, int upper, int next, int prevLower, int lower);
};


#endif //CG1_PROJECT_ICOSPHERE_H
