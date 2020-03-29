#ifndef CG1_PROJECT_RENDERSYSTEM_H
#define CG1_PROJECT_RENDERSYSTEM_H

#include <GLHeader.h>
#include "GLContext.h"


class RenderSystem {
public:
    RenderSystem(GLContext *context);

    void setClearColor(float r, float g, float b, float a);

    void init();
    void render();

private:
    GLContext *context;

    glm::vec4 clearColor;
    double lastTime;
};


#endif //CG1_PROJECT_RENDERSYSTEM_H
