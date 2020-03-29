#include "RenderSystem.h"

RenderSystem::RenderSystem(GLContext *context) : context(context) {
    setClearColor(0.8f, 0.8f, 0.8f, 0.8f);
}

void RenderSystem::init() {
    glEnable(GL_DEPTH_TEST);

    createShaders();
}

void RenderSystem::createShaders() {
    auto vertexShader = new Shader("../assets/shaders/test.vert", GL_VERTEX_SHADER);
    auto fragmentShader = new Shader("../assets/shaders/test.frag", GL_FRAGMENT_SHADER);

    context->attach(vertexShader);
    context->attach(fragmentShader);
    context->linkProgram();

    // Cleanup shader objects
    delete vertexShader;
    delete fragmentShader;
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

void RenderSystem::setClearColor(float r, float g, float b, float a) {
    clearColor = glm::vec4(r, b, g, a);
}
