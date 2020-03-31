#include "RenderSystem.h"

RenderSystem::RenderSystem(GLContext *context) : context(context) {
    setClearColor(0.0f, 0.0f, 0.0f, 0.8f);
}

void RenderSystem::init() {
    glEnable(GL_DEPTH_TEST);

    createShaders();

    shaderProgram->use();
    shaderProgram->setUniform("projection", context->getProjection());

    glm::mat4 initialModel = glm::scale(glm::mat4(1.0f), glm::vec3(0.01, 0.01, 0.01));
    models.push_back(new Model("../assets/models/ico-sphere.dae", glm::vec3(0, 0, 0), initialModel));
    models.push_back(new Model("../assets/models/cylinder.dae", glm::vec3(4, 0, 0), initialModel));
}

void RenderSystem::createShaders() {
    shaderProgram = new ShaderProgram();
    shaderProgram->attachShader("../assets/shaders/test.vert", ShaderType::VertexShader);
    shaderProgram->attachShader("../assets/shaders/test.frag", ShaderType::FragmentShader);
    shaderProgram->link();
}

void RenderSystem::render(double deltaTime) {
    double currentTime = glfwGetTime();

    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram->setUniform("view", context->getView());

    for (auto &model : models) {
        model->draw(*shaderProgram);
    }

    // Render cameras (except the active one)
    auto cameras = context->getCameras();
//    cameras[2]->processMouseMovement(0.1f, 0.0f);
    auto activeCameraIndex = context->getActiveCamera();
    auto perspectiveCameraIndex = 2;
    for (size_t i = 0; i < cameras.size(); i++) {
        if (i != activeCameraIndex && i != perspectiveCameraIndex) {
            cameras[i]->draw(*shaderProgram);
        }
    }

    // Process events and swap buffers
    context->swapBuffers();

    lastTime = currentTime;
}

void RenderSystem::setClearColor(float r, float g, float b, float a) {
    clearColor = glm::vec4(r, b, g, a);
}
