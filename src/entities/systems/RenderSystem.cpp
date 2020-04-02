#include <Grid.h>
#include "entities/systems/RenderSystem.h"

RenderSystem::RenderSystem(EntityManager *entityManager, GLContext *context)
        : System(entityManager), context(context) {
}

void RenderSystem::init() {
    clearColor = glm::vec4(0.2, 0.2, 0.2, 1.0);

    // Depth buffer
    glEnable(GL_DEPTH_TEST);

    // Stencil buffer
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    setupShaders();
    setupGrid();
}

void RenderSystem::update() {
    /// Setup UI
    if (context->displayGui) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Render();
    }

    /// Clear buffers
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // glClear is affected by glStencilMask so disable it after it's cleared
    glStencilMask(0x00);

    /// Render grid
    if (context->displayGrid) {
        gridShaderProgram->use();
        gridShaderProgram->setUniform("view", context->getView());
        gridShaderProgram->setUniform("projection", context->getProjection());
        grid->draw(*gridShaderProgram);
    }

    /// Render entities
    renderEntities();

    /// Display UI
    if (context->displayGui) {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    context->swapBuffers();
}

void RenderSystem::setupShaders() {
    gridShaderProgram = new ShaderProgram();
    gridShaderProgram->attachShader("../assets/shaders/grid.vert", ShaderType::VertexShader);
    gridShaderProgram->attachShader("../assets/shaders/grid.frag", ShaderType::FragmentShader);
    gridShaderProgram->link();

    testShaderProgram = new ShaderProgram();
    testShaderProgram->attachShader("../assets/shaders/test.vert", ShaderType::VertexShader);
    testShaderProgram->attachShader("../assets/shaders/test.frag", ShaderType::FragmentShader);
    testShaderProgram->link();

    highlightShaderProgram = new ShaderProgram();
    highlightShaderProgram->attachShader("../assets/shaders/test.vert", ShaderType::VertexShader);
    highlightShaderProgram->attachShader("../assets/shaders/highlight.frag", ShaderType::FragmentShader);
    highlightShaderProgram->link();
}

void RenderSystem::setupGrid() {
    grid = new Grid(62, 62);
}

void RenderSystem::renderEntities() {
}
