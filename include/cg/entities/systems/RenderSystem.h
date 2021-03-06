#ifndef CG1_PROJECT_RENDERSYSTEM_H
#define CG1_PROJECT_RENDERSYSTEM_H

#include <cg/entities/System.h>
#include <cg/ui/UserInterface.h>

class RenderSystem : public System {
public:
    RenderSystem(EntityManager &, GLContext &, UserInterface &);
    ~RenderSystem();

    UserInterface* getUserInterface() { return &userInterface; }

    void init();
    void initBloomBuffers();

    void update() override;

    void renderEntities();
    void renderMesh(MeshComponent *mesh, ShaderProgram *shaderProgram, glm::mat4 model);
    void renderTexture(MeshComponent* mesh, ShaderProgram* shader);
    void renderMaterials(MeshComponent *mesh, ShaderProgram *shader);

private:
    GLContext &context;

    UserInterface &userInterface;

    glm::vec4 clearColor;

    uint32_t bloomFramebuffer;
    uint32_t bloomTextures[2];

    uint32_t blurFramebuffers[2];
    uint32_t blurTextures[2];
};

#endif //CG1_PROJECT_RENDERSYSTEM_H
