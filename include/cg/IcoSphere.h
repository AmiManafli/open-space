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
    void subdivide(uint16_t level);

    glm::vec3 halfPosition(glm::vec3 a, glm::vec3 b);
};


#endif //CG1_PROJECT_ICOSPHERE_H
