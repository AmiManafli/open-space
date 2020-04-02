#ifndef CG1_PROJECT_RENDERSYSTEM_H
#define CG1_PROJECT_RENDERSYSTEM_H

#include <entities/System.h>

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
    ShaderProgram *testShaderProgram;
    ShaderProgram *highlightShaderProgram;

    void setupGrid();
    Grid *grid;
};

#endif //CG1_PROJECT_RENDERSYSTEM_H
