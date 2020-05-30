#ifndef CG1_PROJECT_SKYBOX_H
#define CG1_PROJECT_SKYBOX_H

#include <cg/GLHeader.h>
#include <cg/entities/EntityManager.h>
#include <cg/entities/components/MeshComponent.h>
#include <cg/entities/systems/RenderSystem.h>
#include <cg/skybox/SkyboxSettings.h>

class Skybox : public MeshComponent {
public:
    Skybox(SkyboxSettings settings, ShaderProgram *shaderProgram);
    Skybox(SkyboxSettings settings, std::string textureFilename, ShaderProgram *shaderProgram);
    Skybox(SkyboxSettings settings, glm::vec3 position, ShaderProgram *shaderProgram);

    void generate(uint64_t seed);
    void render(RenderSystem *renderSystem, EntityManager *entityManager, CameraComponent *camera);

    SkyboxSettings settings;

private:
    static std::vector<MeshComponent::Vertex> createCubeMesh(float size);
    static void createTriangle(std::vector<MeshComponent::Vertex> &vertices, glm::vec3 a, glm::vec3 b, glm::vec3 c);

    void createTexture(glm::vec3 position);
    void createTexture(std::string filename);

    void createStars(EntityManager *entityManager, ShaderProgram *shaderProgram, uint32_t count, float starSize, float distance, uint16_t subdivisions);
    void createNebulae(EntityManager *entityManager, ShaderProgram *shaderProgram);

    void renderEntities(RenderSystem *renderSystem, EntityManager *entityManager);
};

#endif //CG1_PROJECT_SKYBOX_H
