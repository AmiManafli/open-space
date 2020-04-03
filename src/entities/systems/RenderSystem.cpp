#include "cg/entities/systems/RenderSystem.h"

RenderSystem::RenderSystem(EntityManager *entityManager, GLContext *context)
        : System(entityManager), context(context) {
    userInterface = new UserInterface(context);
}

RenderSystem::~RenderSystem() {
    delete userInterface;
}

void RenderSystem::init() {
    clearColor = glm::vec4(0.2, 0.2, 0.2, 1.0);

    // Depth buffer
    glEnable(GL_DEPTH_TEST);

    // Stencil buffer
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glEnable(GL_CULL_FACE);
}

void RenderSystem::update() {
    /// Setup UI
    if (context->displayGui) {
        userInterface->render();
    }

    /// Clear buffers
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glStencilMask(0x00); // glClear is affected by glStencilMask so disable it after it's cleared

    /// Render entities
    renderEntities();

    /// Display UI
    if (context->displayGui) {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    context->swapBuffers();
}

void RenderSystem::renderEntities() {
    uint32_t triangleCount = 0;

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
            triangleCount += (double) it->second->indices.size() / 3.0;
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

    context->triangleCount = triangleCount;
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

