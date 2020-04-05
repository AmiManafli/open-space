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

    glEnable(GL_CULL_FACE);
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

    for (auto& pair : entityManager->getTransformComponents()) {
        auto entityId = pair.first;
        auto transform = pair.second;
        auto meshes = entityManager->getMeshComponents(entityId);
        auto highlight = entityManager->getHighlightComponent(entityId);

        if (highlight) {
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(0xFF);
        }

        // Render meshes
        for (auto it = meshes.first; it != meshes.second; it++) {
            triangleCount += (double) it->second->indices.size() / 3.0;
            renderMesh(it->second, it->second->shaderProgram, transform->model);
        }

        if (highlight) {
            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00); // disable writing to the stencil buffer
            glDisable(GL_DEPTH_TEST);

            auto highlightModel = highlight->getModel(transform->model);
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
    shaderProgram->setUniform("objectColor", glm::vec3(0.5, 0.5, 0.5));
    shaderProgram->setUniform("lightColor", glm::vec3(1, 1, 1));
    shaderProgram->setUniform("lightPos", entityManager->getTransformComponent(0)->position);

    if (mesh->textures.size() > 0) {
        renderTexture(mesh, shaderProgram);
    }

    glBindVertexArray(mesh->vao);
    if (mesh->instances > 1) {
        glDrawElementsInstanced(mesh->mode, mesh->indices.size(), GL_UNSIGNED_INT, nullptr, mesh->instances);
    } else {
        glDrawElements(mesh->mode, mesh->indices.size(), GL_UNSIGNED_INT, nullptr);
    }

	//cleanup
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void RenderSystem::renderTexture(MeshComponent *mesh, ShaderProgram *shaderProgram) {
	uint32_t diffuseNr = 1;
	uint32_t specularNr = 1;
	uint32_t normalNr = 1;
	uint32_t heightNr = 1;
	for (unsigned int i = 0; i < mesh->textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = mesh->textures[i].type;
		if (name == "texture_diffuse") {
			number = std::to_string(diffuseNr++);
		}
		else if (name == "texture_specular") {
			number = std::to_string(specularNr++); // transfer unsigned int to stream
		}
		else if (name == "texture_normal") {
			number = std::to_string(normalNr++); // transfer unsigned int to stream
		}
		else if (name == "texture_height") {
			number = std::to_string(heightNr++); // transfer unsigned int to stream
		}

		// now set the sampler to the correct texture unit
		shaderProgram->setUniform((name + number).c_str(), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, mesh->textures[i].id);
	}
}