#include "cg/entities/EntityManager.h"

EntityManager::~EntityManager() {
}

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

//void EntityManager::addMeshComponent(uint32_t entityId, MeshComponent *component) {
//    meshComponents.emplace(entityId, component);
//}
//
//std::pair<MeshComponentMultimap::iterator, MeshComponentMultimap::iterator> EntityManager::getMeshComponents(uint32_t entityId) {
//    return meshComponents.equal_range(entityId);
//}

//template<class T>
//T *EntityManager::getComponent(std::unordered_map<uint32_t, T *> map, uint32_t entityId) {
//    try {
//        return map.at(entityId);
//    } catch (std::out_of_range &e) {
//        return nullptr;
//    }
//}
