#include "cg/entities/EntityManager.h"

EntityManager::~EntityManager() {
    // Components
    for (auto& pair1 : components) {
        auto map = pair1.second;
        for (auto& pair2 : map) {
            auto component = pair2.second;
            delete component;
        }
        map.clear();
    }
    components.clear();

    // Multi components
    for (auto& pair1 : multiComponents) {
        auto map = pair1.second;
        for (auto& pair2 : map) {
            auto component = pair2.second;
            delete component;
        }
        map.clear();
    }
    multiComponents.clear();

    // Entities
    for (auto& entity : entities) {
        delete entity;
    }
    entities.clear();
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
