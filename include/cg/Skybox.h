#ifndef CG1_PROJECT_SKYBOX_H
#define CG1_PROJECT_SKYBOX_H

#include <cg/GLHeader.h>
#include <cg/entities/components/MeshComponent.h>

class Skybox : public MeshComponent {
public:
    Skybox(ShaderProgram *shaderProgram);
    Skybox(std::string textureFilename, ShaderProgram *shaderProgram);
    Skybox(glm::vec3 position, ShaderProgram *shaderProgram);

private:
    void createMesh();

    void createTexture(glm::vec3 position);
    void createTexture(std::string filename);

    void createTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);
};

#endif //CG1_PROJECT_SKYBOX_H
