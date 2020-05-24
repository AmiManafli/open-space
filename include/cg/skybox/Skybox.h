#ifndef CG1_PROJECT_SKYBOX_H
#define CG1_PROJECT_SKYBOX_H

#include <cg/GLHeader.h>
#include <cg/entities/EntityManager.h>
#include <cg/entities/components/MeshComponent.h>
#include <cg/entities/systems/RenderSystem.h>

class Skybox : public MeshComponent {
public:
    Skybox(uint32_t size, uint32_t numStars, uint32_t numBigStars, ShaderProgram *shaderProgram);
    Skybox(uint32_t size, uint32_t numStars, uint32_t numBigStars, std::string textureFilename, ShaderProgram *shaderProgram);
    Skybox(uint32_t size, uint32_t numStars, uint32_t numBigStars, glm::vec3 position, ShaderProgram *shaderProgram);

    void generate(uint64_t seed);
    void render(RenderSystem *renderSystem, EntityManager *entityManager, CameraComponent *camera, bool saveToDisk);

    uint32_t numStars;
    uint32_t numBigStars;

private:
    void createMesh();

    void createTexture(glm::vec3 position);
    void createTexture(std::string filename);

    void createTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);

    void createStars(EntityManager *entityManager, ShaderProgram *shaderProgram, uint32_t count, float starSize, float distance, uint16_t subdivisions);
    void renderEntities(RenderSystem *renderSystem, EntityManager *entityManager, ShaderProgram *shaderProgram);

    uint32_t size;

    static constexpr uint32_t resolution = 3072;
};

#endif //CG1_PROJECT_SKYBOX_H
