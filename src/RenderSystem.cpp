#include "RenderSystem.h"

RenderSystem::RenderSystem(GLContext *context) : context(context) {
    setClearColor(0.0f, 0.0f, 0.0f, 0.8f);
}

void RenderSystem::init() {
    glEnable(GL_DEPTH_TEST);

    createShaders();

    shaderProgram->use();

//    glm::mat4 initialModel = glm::scale(glm::mat4(1.0f), glm::vec3(0.01, 0.01, 0.01));
    models.push_back(new Model("../assets/models/ico-sphere.dae", glm::vec3(0, 0, 0)));
    models.push_back(new Model("../assets/models/cylinder.dae", glm::vec3(2, 0, 0)));

    grid = new Grid(62, 62);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void RenderSystem::createShaders() {
    gridShaderProgram = new ShaderProgram();
    gridShaderProgram->attachShader("../assets/shaders/grid.vert", ShaderType::VertexShader);
    gridShaderProgram->attachShader("../assets/shaders/grid.frag", ShaderType::FragmentShader);
    gridShaderProgram->link();

    shaderProgram = new ShaderProgram();
    shaderProgram->attachShader("../assets/shaders/test.vert", ShaderType::VertexShader);
    shaderProgram->attachShader("../assets/shaders/test.frag", ShaderType::FragmentShader);
    shaderProgram->link();
}

void RenderSystem::render(double deltaTime) {
    double currentTime = glfwGetTime();

    if (context->isDebug()) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Render();
    }

    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (context->showGrid()) {
        gridShaderProgram->use();
        gridShaderProgram->setUniform("model", glm::mat4());
        gridShaderProgram->setUniform("view", context->getView());
        gridShaderProgram->setUniform("projection", context->getProjection());
        grid->draw(*gridShaderProgram);
    }

    shaderProgram->use();
    shaderProgram->setUniform("view", context->getView());
    shaderProgram->setUniform("projection", context->getProjection());
    for (auto &model : models) {
        model->draw(*shaderProgram);
    }

    // Render cameras (except the active one)
    auto cameras = context->getCameras();
    auto activeCameraIndex = context->getActiveCamera();
    auto perspectiveCameraIndex = 2;
    for (size_t i = 0; i < cameras.size(); i++) {
        if (i != activeCameraIndex && i != perspectiveCameraIndex) {
            cameras[i]->draw(*shaderProgram);
        }
    }

    if (context->isDebug()) {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    // Process events and swap buffers
    context->swapBuffers();

    lastTime = currentTime;
}

void RenderSystem::setClearColor(float r, float g, float b, float a) {
    clearColor = glm::vec4(r, b, g, a);
}
