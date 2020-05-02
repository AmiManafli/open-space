#ifndef CG1_PROJECT_ICOSPHERE_H
#define CG1_PROJECT_ICOSPHERE_H

#include <cg/GLHeader.h>
#include <cg/entities/components/MeshComponent.h>
#include <unordered_map>


class IcoSphere : public MeshComponent {
public:
    IcoSphere(double radius, uint16_t subdivisions, ShaderProgram *shaderProgram);

    void subdivide(uint16_t subdivisions) override;

private:
    uint16_t textureWidth, textureHeight;
    double horizontalStep, verticalStep;

    double radius;

    std::vector<std::vector<uint32_t>> subdividedIndices;

    void generateMesh();
    void generateTexture();

    glm::vec3 halfPosition(glm::vec3 a, glm::vec3 b);

    glm::vec2 halfTextureCoord(glm::vec2 a, glm::vec2 b);
};


#endif //CG1_PROJECT_ICOSPHERE_H
