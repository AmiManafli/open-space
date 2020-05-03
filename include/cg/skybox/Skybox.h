#ifndef CG1_PROJECT_SKYBOX_H
#define CG1_PROJECT_SKYBOX_H

#include <cg/GLHeader.h>
#include <cg/entities/EntityManager.h>
#include <cg/entities/components/MeshComponent.h>
#include <cg/entities/systems/RenderSystem.h>

class Skybox : public MeshComponent {
public:
    Skybox(glm::vec3 size, ShaderProgram *shaderProgram);
    Skybox(glm::vec3 size, std::string textureFilename, ShaderProgram *shaderProgram);
    Skybox(glm::vec3 size, glm::vec3 position, ShaderProgram *shaderProgram);

    void generate(uint64_t seed);
    void render(RenderSystem *renderSystem, EntityManager *entityManager, CameraComponent *camera);

    uint32_t numStars;

private:
    void createMesh();

    void createTexture(glm::vec3 position);
    void createTexture(std::string filename);

    void createTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);

    void createEntities(EntityManager *entityManager, ShaderProgram *shaderProgram);
    void renderEntities(RenderSystem *renderSystem, EntityManager *entityManager, ShaderProgram *shaderProgram);

    glm::vec3 size;

    static constexpr uint32_t resolution = 4096;
};

#endif //CG1_PROJECT_SKYBOX_H
