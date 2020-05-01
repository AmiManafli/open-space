#ifndef CG1_PROJECT_ICOSPHERE_H
#define CG1_PROJECT_ICOSPHERE_H

#include <cg/GLHeader.h>
#include <cg/entities/components/MeshComponent.h>
#include <unordered_map>


class IcoSphere : public MeshComponent {
public:
    IcoSphere(double radius, int maxSubdivisions, ShaderProgram *shaderProgram);

private:
    double radius;

    std::unordered_map<uint16_t, std::vector<uint32_t>> subdividedIndices;

    void generateMesh();
    void subdivide(uint16_t level);

    glm::vec3 halfPosition(glm::vec3 a, glm::vec3 b);
};


#endif //CG1_PROJECT_ICOSPHERE_H
