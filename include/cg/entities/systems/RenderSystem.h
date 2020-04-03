#ifndef CG1_PROJECT_RENDERSYSTEM_H
#define CG1_PROJECT_RENDERSYSTEM_H

#include <cg/entities/System.h>

class RenderSystem : public System {
public:
    RenderSystem(EntityManager *entityManager, GLContext *context);

    void init() override;
    void update() override;

private:
    GLContext *context;

    glm::vec4 clearColor;

    void renderEntities();
    void renderMesh(MeshComponent *mesh, ShaderProgram *shaderProgram, glm::mat4 model);
};

#endif //CG1_PROJECT_RENDERSYSTEM_H
