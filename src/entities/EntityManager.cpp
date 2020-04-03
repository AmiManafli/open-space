#include "cg/entities/EntityManager.h"


Entity *EntityManager::createEntity() {
    auto entity = new Entity();
    entity->id = getNextEntityId();
    entities.push_back(entity);
    return entity;
}

uint32_t EntityManager::getNextEntityId() {
    return nextId++;
}

void EntityManager::addMeshComponent(uint32_t entityId, MeshComponent *component) {
    meshComponents.emplace(entityId, component);
}

std::pair<MeshComponentMultimap::iterator, MeshComponentMultimap::iterator> EntityManager::getMeshComponents(uint32_t entityId) {
    return meshComponents.equal_range(entityId);
}

void EntityManager::addPositionComponent(uint32_t entityId, PositionComponent* component) {
    positionComponents.emplace(entityId, component);
}

PositionComponent* EntityManager::getPositionComponent(uint32_t entityId) {
    return getComponent(positionComponents, entityId);
}

void EntityManager::addCameraComponent(uint32_t entityId, CameraComponent* component) {
    cameraComponents.emplace(entityId, component);
}

CameraComponent* EntityManager::getCameraComponent(uint32_t entityId) {
    return getComponent(cameraComponents, entityId);
}

void EntityManager::addHighlightComponent(uint32_t entityId, HighlightComponent *component) {
    highlightComponents.emplace(entityId, component);
}

HighlightComponent *EntityManager::getHighlightComponent(uint32_t entityId) {
    return getComponent(highlightComponents, entityId);
}

template<class T>
T *EntityManager::getComponent(std::unordered_map<uint32_t, T *> map, uint32_t entityId) {
    try {
        return map.at(entityId);
    } catch (std::out_of_range &e) {
        return nullptr;
    }
}
