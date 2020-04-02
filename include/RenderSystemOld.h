#ifndef CG1_PROJECT_RENDERSYSTEMOLD_H
#define CG1_PROJECT_RENDERSYSTEMOLD_H

#include <GLHeader.h>
#include "GLContext.h"
#include "meshes/Mesh.h"
#include "meshes/Model.h"
#include "Grid.h"


class RenderSystemOld {
public:
    explicit RenderSystemOld(GLContext *context);

    void setClearColor(float r, float g, float b, float a);

    void init();
    void render(double deltaTime);

private:
    GLContext *context;

    ShaderProgram* shaderProgram;
    ShaderProgram* gridShaderProgram;
    ShaderProgram* highlightShaderProgram;

    std::vector<Model*> models;

    Grid *grid;

    glm::mat4 view;
    glm::vec4 clearColor;

    void createShaders();
};


#endif //CG1_PROJECT_RENDERSYSTEMOLD_H
