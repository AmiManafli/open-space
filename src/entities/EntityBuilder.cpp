#include "cg/entities/EntityBuilder.h"


Entity *EntityBuilder::build(EntityManager *entityManager) {
    auto entity = entityManager->createEntity();

    if (positionComponent != nullptr) {
        entityManager->addPositionComponent(entity->id, positionComponent);
    }
    if (cameraComponent != nullptr) {
        entityManager->addCameraComponent(entity->id, cameraComponent);
    }
    if (highlightComponent != nullptr) {
        entityManager->addHighlightComponent(entity->id, highlightComponent);
    }

    for (auto& mesh : meshComponents) {
        entityManager->addMeshComponent(entity->id, mesh);
    }

    return entity;
}

/**
 * Destroy entity and all components.
 */
void EntityBuilder::destroy() {
    for (auto mesh : meshComponents) {
        delete mesh;
    }
    delete positionComponent;
    delete this;
}

EntityBuilder *EntityBuilder::withPosition(float x, float y, float z) {
    return withPosition(glm::vec3(x, y, z));
}

EntityBuilder *EntityBuilder::withPosition(glm::vec3 position) {
    if (positionComponent != nullptr) {
        throw std::runtime_error("entity already has a position");
    }
    positionComponent = new PositionComponent(position);
    return this;
}

EntityBuilder *EntityBuilder::withMesh(std::string filename, ShaderProgram *shaderProgram) {
    auto meshes = MeshComponent::createMeshComponentsFromFile(filename, shaderProgram);
    for (auto& mesh : meshes) {
        meshComponents.push_back(mesh);
    }
    return this;
}

EntityBuilder *EntityBuilder::withMesh(std::vector<MeshComponent::Vertex> &vertices, std::vector<uint32_t> &indices,
                                       std::vector<MeshComponent::Texture> &textures, ShaderProgram *shaderProgram, GLenum mode) {
    meshComponents.push_back(new MeshComponent(vertices, indices, textures, shaderProgram, mode));
    return this;
}

EntityBuilder * EntityBuilder::withCamera(CameraComponent::Mode mode, CameraComponent::Type type, glm::vec3 target, glm::vec3 front, glm::vec3 up, float aspectRatio) {
    if (cameraComponent != nullptr) {
        throw std::runtime_error("entity already has a camera");
    }
    if (positionComponent == nullptr) {
        throw std::runtime_error("need a PositionComponent to initialize the CameraComponent");
    }
    cameraComponent = new CameraComponent(mode, type, target, front, up, positionComponent);
    return this;
}

EntityBuilder *EntityBuilder::withHighlight(float scaleFactor, ShaderProgram *shaderProgram) {
    if (highlightComponent != nullptr) {
        throw std::runtime_error("entity already has a highlight");
    }
    highlightComponent = new HighlightComponent(scaleFactor, shaderProgram);
    return this;
}
