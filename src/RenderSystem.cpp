#include "RenderSystem.h"

RenderSystem::RenderSystem(GLContext *context) : context(context) {
}

void RenderSystem::init() {
    glEnable(GL_DEPTH_TEST);

    // TODO: Create shaders
}

void RenderSystem::render() {
    double currentTime = glfwGetTime();

    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TODO: Draw stuff

    // Process events and swap buffers
    context->swapBuffers();

    lastTime = currentTime;
}

void RenderSystem::setClearColor(glm::vec4 color) {
    clearColor = color;
}
