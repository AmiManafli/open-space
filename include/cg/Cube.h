#ifndef CG1_PROJECT_CUBE_H
#define CG1_PROJECT_CUBE_H

#include <cg/GLHeader.h>
#include <cg/entities/components/MeshComponent.h>


class Cube : public MeshComponent {
public:
    Cube(uint16_t subdivisions, ShaderProgram &shaderProgram);
    ~Cube() override;

    void subdivide(uint16_t subdivisions) override;

private:
    void createTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);
    void createMesh();
};

#endif //CG1_PROJECT_CUBE_H
