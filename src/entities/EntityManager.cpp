#include "cg/entities/EntityManager.h"


Entity *EntityManager::createEntity() {
    auto entity = new Entity();
    entity->id = getNextEntityId();
    entities.push_back(entity);
    return entity;
}

Entity *EntityManager::getEntity(uint32_t id) {
    for (auto& entity : entities) {
        if (entity->id == id) {
            return entity;
        }
    }
    return nullptr;
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

void EntityManager::addTransformComponent(uint32_t entityId, TransformComponent* component) {
    transformComponents.emplace(entityId, component);
}

TransformComponent* EntityManager::getTransformComponent(uint32_t entityId) {
    return getComponent(transformComponents, entityId);
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

void EntityManager::addVelocityComponent(uint32_t entityId, VelocityComponent *component) {
    velocityComponents.emplace(entityId, component);
}

VelocityComponent *EntityManager::getVelocityComponent(uint32_t entityId) {
    return getComponent(velocityComponents, entityId);
}

void EntityManager::addLightComponent(uint32_t entityId, LightComponent *component) {
    lightComponents.emplace(entityId, component);
}

LightComponent *EntityManager::getLightComponent(uint32_t entityId) {
    return getComponent(lightComponents, entityId);
}

void EntityManager::addMassComponent(uint32_t entityId, MassComponent *component) {
    massComponents.emplace(entityId, component);
}

MassComponent *EntityManager::getMassComponent(uint32_t entityId) {
    return getComponent(massComponents, entityId);
}

void EntityManager::addOrbitComponent(uint32_t entityId, OrbitComponent *component) {
    orbitComponents.emplace(entityId, component);
}

OrbitComponent *EntityManager::getOrbitComponent(uint32_t entityId) {
    return getComponent(orbitComponents, entityId);
}

template<class T>
T *EntityManager::getComponent(std::unordered_map<uint32_t, T *> map, uint32_t entityId) {
    try {
        return map.at(entityId);
    } catch (std::out_of_range &e) {
        return nullptr;
    }
}
