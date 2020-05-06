#include "cg/entities/systems/RenderSystem.h"

RenderSystem::RenderSystem(EntityManager *entityManager, GLContext *context)
        : System(entityManager), context(context) {
    userInterface = new UserInterface(entityManager, context);
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
}

void RenderSystem::update() {
    /// Setup UI
    if (context->displayGui || context->displayCursor) {
        userInterface->render();
    }

    /// Clear buffers
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glStencilMask(0x00); // glClear is affected by glStencilMask so disable it after it's cleared

    /// Render entities
    renderEntities();

    /// Display UI
    if (context->displayGui || context->displayCursor) {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    context->swapBuffers();
}

void RenderSystem::renderEntities() {
    uint32_t triangleCount = 0;

    for (auto& pair : entityManager->getComponents<TransformComponent>()) {
        auto entity = pair.first;
        auto transform = dynamic_cast<TransformComponent *>(pair.second);
        auto meshes = entityManager->getMultiComponents<MeshComponent>(entity);
        auto highlight = entityManager->getComponent<HighlightComponent>(entity);

        if (highlight) {
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(0xFF);
        }

        // Render meshes
        for (auto it = meshes.first; it != meshes.second; it++) {
            auto entity = it->first;
            auto mesh = dynamic_cast<MeshComponent *>(it->second);
            triangleCount += (double) mesh->indices.size() / 3.0;
            renderMesh(mesh, mesh->shaderProgram, transform->getModel());
        }

        if (highlight) {
            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00); // disable writing to the stencil buffer
            glDisable(GL_DEPTH_TEST);

            auto cameraPosition = entityManager->getComponent<TransformComponent>(context->getCamera())->position;
            auto highlightModel = highlight->getModel(transform->getModel(), glm::length(transform->position - cameraPosition));
            for (auto it = meshes.first; it != meshes.second; it++) {
                auto mesh = dynamic_cast<MeshComponent *>(it->second);
                renderMesh(mesh, highlight->shaderProgram, highlightModel);
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
    shaderProgram->setUniform("objectColor", glm::vec3(0.5, 0.5, 0.5));
    shaderProgram->setUniform("lightColor", glm::vec3(1, 1, 1));
    shaderProgram->setUniform("lightPos", entityManager->getComponent<TransformComponent>(context->light)->position);
    shaderProgram->setUniform("viewPos", entityManager->getComponent<TransformComponent>(context->getCamera())->position);

    if (mesh->textures.size() > 0) {
        renderTexture(mesh, shaderProgram);
    }

    glBindVertexArray(mesh->vao);
    if (mesh->instances > 1) {
        if (mesh->indexed) {
            glDrawElementsInstanced(mesh->mode, mesh->indices.size(), GL_UNSIGNED_INT, nullptr, mesh->instances);
        } else {
            glDrawArraysInstanced(mesh->mode, 0, mesh->vertices.size(), mesh->instances);
        }
    } else {
        if (mesh->indexed) {
            glDrawElements(mesh->mode, mesh->indices.size(), GL_UNSIGNED_INT, nullptr);
        } else {
            glDrawArrays(mesh->mode, 0, mesh->vertices.size());
        }
    }

	//cleanup
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void RenderSystem::renderTexture(MeshComponent *mesh, ShaderProgram *shaderProgram) {
	for (unsigned int i = 0; i < mesh->textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		auto texture = mesh->textures[i];
		std::string name = texture.type;
		if (name == "texture_diffuse") {
		    shaderProgram->setUniform("material.diffuse", i);
		} else if (name == "texture_specular") {
            shaderProgram->setUniform("material.specular", i);
		} else if (name == "texture_normal") {
//            shaderProgram->setUniform("material.normal", i);
        } else if (name == "texture_height") {
//            shaderProgram->setUniform("material.height", i);
		}

		if (texture.isCubeMap) {
            glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);
		} else {
            glBindTexture(GL_TEXTURE_2D, texture.id);
		}
	}
}