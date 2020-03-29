#ifndef CG1_PROJECT_RENDERSYSTEM_H
#define CG1_PROJECT_RENDERSYSTEM_H

#include <GLHeader.h>
#include "GLContext.h"
#include "meshes/Mesh.h"
#include "meshes/Model.h"


class RenderSystem {
public:
    RenderSystem(GLContext *context);

    void setClearColor(float r, float g, float b, float a);

    void init();
    void render(double deltaTime);

private:
    GLContext *context;

    ShaderProgram* shaderProgram;

    std::vector<Model*> models;

    glm::vec4 clearColor;
    double lastTime;

    void createShaders();
};


#endif //CG1_PROJECT_RENDERSYSTEM_H
