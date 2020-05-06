#ifndef CG1_PROJECT_ENTITYMANAGER_H
#define CG1_PROJECT_ENTITYMANAGER_H

#include <vector>
#include <unordered_map>
#include <cg/entities/components/MeshComponent.h>
#include <cg/entities/components/TransformComponent.h>
#include <cg/entities/components/CameraComponent.h>
#include <cg/entities/components/HighlightComponent.h>
#include <cg/entities/components/VelocityComponent.h>
#include <cg/entities/components/LightComponent.h>
#include <cg/entities/components/MassComponent.h>
#include <cg/entities/components/OrbitComponent.h>
#include "Component.h"
#include "Entity.h"

typedef std::unordered_multimap<Entity *, Component *> ComponentMultiMap;
typedef std::unordered_map<Entity *, Component *> ComponentMap;


class EntityManager {
public:
    ~EntityManager();

    /// Entities
    Entity* createEntity();
    Entity* getEntity(uint32_t id);
    std::vector<Entity *> getEntities() { return entities; }

    template<class T>
    void addComponent(Entity *entity, T *component) {
        auto key = typeid(T).name();
        if (components.find(key) == components.end()) {
            components[key] = std::unordered_map<Entity *, Component *>();
        }
        components[key][entity] = component;
    }

    template<class T>
    void removeComponent(Entity *entity) {
        auto key = typeid(T).name();
        auto component = getComponent<T>(entity);
        components[key].erase(entity);
        delete component;
    }

    template<class T>
    ComponentMap getComponents() {
        auto key = typeid(T).name();
        return components[key];
    }

    template<class T>
    T* getComponent(Entity *entity) {
        auto key = typeid(T).name();
        try {
            return reinterpret_cast<T *>(components[key].at(entity));
        } catch (std::out_of_range &e) {
            return nullptr;
        }
    }

    template<class T>
    void addMultiComponent(Entity *entity, T *component) {
        auto key = typeid(T).name();
        if (multiComponents.find(key) == multiComponents.end()) {
            multiComponents[key] = std::unordered_multimap<Entity *, Component *>();
        }
        multiComponents[key].emplace(entity, component);
    }

    template<class T>
    void removeMultiComponent(Entity *entity) {
        auto key = typeid(T).name();
        auto component = getMultiComponent<T>(entity);
        multiComponents[key].erase(entity);
        delete component;
    }

    template<class T>
    ComponentMultiMap getMultiComponents() {
        auto key = typeid(T).name();
        return multiComponents[key];
    }

    template<class T>
    std::pair<ComponentMultiMap::iterator, ComponentMultiMap::iterator> getMultiComponents(Entity *entity) {
        auto key = typeid(T).name();
        return multiComponents[key].equal_range(entity);
    }

private:
    /// Entities
    uint32_t getNextEntityId();
    uint32_t nextId = 0;
    std::vector<Entity *> entities;

    /// Components
    std::unordered_map<const char *, ComponentMap> components;
    std::unordered_map<const char *, ComponentMultiMap> multiComponents;
};

#endif //CG1_PROJECT_ENTITYMANAGER_H
