#include <cg/Grid.h>
#include "cg/entities/systems/RenderSystem.h"

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

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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
}

void RenderSystem::setupGrid() {
    grid = new Grid(62, 62);
}

void RenderSystem::renderEntities() {
    for (auto& pair : entityManager->getPositionComponents()) {
        auto entityId = pair.first;
        auto position = pair.second;
        auto meshes = entityManager->getMeshComponents(entityId);
        auto highlight = entityManager->getHighlightComponent(entityId);

        if (highlight) {
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(0xFF);
        }

        // Render meshes
        for (auto it = meshes.first; it != meshes.second; it++) {
            renderMesh(it->second, it->second->shaderProgram, position->model);
        }

        if (highlight) {
            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00); // disable writing to the stencil buffer
            glDisable(GL_DEPTH_TEST);

            auto highlightModel = highlight->getModel(position->model);

            for (auto it = meshes.first; it != meshes.second; it++) {
                renderMesh(it->second, highlight->shaderProgram, highlightModel);
            }

            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glEnable(GL_DEPTH_TEST);
        }
    }
}

void RenderSystem::renderMesh(MeshComponent *mesh, ShaderProgram *shaderProgram, glm::mat4 model) {
    shaderProgram->use();
    shaderProgram->setUniform("view", context->getView());
    shaderProgram->setUniform("projection", context->getProjection());
    shaderProgram->setUniform("model", model);
    glBindVertexArray(mesh->vao);
    glDrawElements(mesh->mode, mesh->indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

