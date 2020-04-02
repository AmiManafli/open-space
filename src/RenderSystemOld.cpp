#include "RenderSystemOld.h"

RenderSystemOld::RenderSystemOld(GLContext *context) : context(context) {
    setClearColor(0.0f, 0.0f, 0.0f, 0.8f);
}

void RenderSystemOld::init() {
    // Depth buffer
    glEnable(GL_DEPTH_TEST);

    // Stencil buffer
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    createShaders();

    shaderProgram->use();

    auto model = new Model("../assets/models/ico-sphere.dae", glm::vec3(0, 0, 0));
    model->setHighlighted(true);

    models.push_back(model);
    models.push_back(new Model("../assets/models/cube.dae", glm::vec3(5, 0, 0)));

    grid = new Grid(62, 62);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void RenderSystemOld::createShaders() {
    gridShaderProgram = new ShaderProgram();
    gridShaderProgram->attachShader("../assets/shaders/grid.vert", ShaderType::VertexShader);
    gridShaderProgram->attachShader("../assets/shaders/grid.frag", ShaderType::FragmentShader);
    gridShaderProgram->link();

    shaderProgram = new ShaderProgram();
    shaderProgram->attachShader("../assets/shaders/test.vert", ShaderType::VertexShader);
    shaderProgram->attachShader("../assets/shaders/test.frag", ShaderType::FragmentShader);
    shaderProgram->link();

    highlightShaderProgram = new ShaderProgram();
    highlightShaderProgram->attachShader("../assets/shaders/test.vert", ShaderType::VertexShader);
    highlightShaderProgram->attachShader("../assets/shaders/highlight.frag", ShaderType::FragmentShader);
    highlightShaderProgram->link();
}

void RenderSystemOld::render(double deltaTime) {
    if (context->displayGui) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Render();
    }

    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // glClear is affected by glStencilMask so disable it after it's cleared
    glStencilMask(0x00);

    if (context->displayGrid) {
        gridShaderProgram->use();
        gridShaderProgram->setUniform("view", context->getView());
        gridShaderProgram->setUniform("projection", context->getProjection());
        grid->draw(*gridShaderProgram);
    }

    highlightShaderProgram->use();
    highlightShaderProgram->setUniform("view", context->getView());
    highlightShaderProgram->setUniform("projection", context->getProjection());

    shaderProgram->use();
    shaderProgram->setUniform("view", context->getView());
    shaderProgram->setUniform("projection", context->getProjection());
    for (auto &model : models) {
        model->draw(*shaderProgram, *highlightShaderProgram);
    }

    // Render cameras (except the active one)
//    auto cameras = context->getCameras();
//    auto activeCameraIndex = context->getActiveCamera();
//    auto perspectiveCameraIndex = 2;
//    for (size_t i = 0; i < cameras.size(); i++) {
//        if (i != activeCameraIndex && i != perspectiveCameraIndex) {
//            cameras[i]->draw(*shaderProgram);
//        }
//    }

    if (context->displayGui) {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    // Process events and swap buffers
    context->swapBuffers();
}

void RenderSystemOld::setClearColor(float r, float g, float b, float a) {
    clearColor = glm::vec4(r, b, g, a);
}
