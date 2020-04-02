#include "entities/EntityManager.h"


Entity *EntityManager::createEntity() {
    auto entity = new Entity();
    entity->id = getNextEntityId();
    entities.push_back(entity);
    return entity;
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
    return positionComponents.at(entityId);
}

void EntityManager::addCameraComponent(uint32_t entityId, CameraComponent* component) {
    cameraComponents.emplace(entityId, component);
}

CameraComponent* EntityManager::getCameraComponent(uint32_t entityId) {
    return cameraComponents.at(entityId);
}

uint32_t EntityManager::getNextEntityId() {
    return nextId++;
}

