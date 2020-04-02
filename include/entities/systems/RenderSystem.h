#ifndef CG1_PROJECT_RENDERSYSTEM_H
#define CG1_PROJECT_RENDERSYSTEM_H

#include <entities/System.h>
#include <Grid.h>

class RenderSystem : public System {
public:
    RenderSystem(EntityManager *entityManager, GLContext *context);

    void init() override;
    void update() override;

private:
    GLContext *context;

    glm::vec4 clearColor;

    void setupShaders();
    ShaderProgram *gridShaderProgram;

    void setupGrid();
    Grid *grid;

    void renderEntities();
    void renderMesh(MeshComponent *mesh, PositionComponent *position);
};

#endif //CG1_PROJECT_RENDERSYSTEM_H
