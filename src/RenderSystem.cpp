#include "RenderSystem.h"

RenderSystem::RenderSystem(GLContext *context) : context(context) {
    setClearColor(0.0f, 0.0f, 0.0f, 0.8f);
}

void RenderSystem::init() {
    glEnable(GL_DEPTH_TEST);

    createShaders();

    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(vao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
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

    context->use();

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Process events and swap buffers
    context->swapBuffers();

    lastTime = currentTime;
}

void RenderSystem::setClearColor(float r, float g, float b, float a) {
    clearColor = glm::vec4(r, b, g, a);
}
