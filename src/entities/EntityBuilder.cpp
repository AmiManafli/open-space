#include "entities/EntityBuilder.h"


EntityBuilder::EntityBuilder(EntityManager *entityManager) : entityManager(entityManager) {
}

EntityBuilder::~EntityBuilder() {
}

Entity *EntityBuilder::build() {
    auto entity = entityManager->createEntity();

    if (positionComponent != nullptr) {
        entityManager->addPositionComponent(entity->id, positionComponent);
    }

    for (auto& mesh : meshComponents) {
        entityManager->addMeshComponent(entity->id, mesh);
    }

    return entity;
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
                                       std::vector<MeshComponent::Texture> &textures, ShaderProgram *shaderProgram) {
    meshComponents.push_back(new MeshComponent(vertices, indices, textures, shaderProgram));
    return this;
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