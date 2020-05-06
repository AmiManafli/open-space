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

typedef std::unordered_multimap<uint32_t, MeshComponent *> MeshComponentMultimap;
typedef std::unordered_map<uint32_t, CameraComponent *> CameraComponentMap;
typedef std::unordered_map<uint32_t, TransformComponent *> TransformComponentMap;
typedef std::unordered_map<uint32_t, HighlightComponent *> HighlightComponentMap;
typedef std::unordered_map<uint32_t, VelocityComponent *> VelocityComponentMap;
typedef std::unordered_map<uint32_t, LightComponent *> LightComponentMap;
typedef std::unordered_map<uint32_t, MassComponent *> MassComponentMap;
typedef std::unordered_map<uint32_t, OrbitComponent *> OrbitComponentMap;


class EntityManager {
public:
    ~EntityManager();

    /// Entities
    Entity* createEntity();
    std::vector<Entity *> getEntities() { return entities; }
    Entity* getEntity(uint32_t id);

    /// Mesh component
    void addMeshComponent(uint32_t entityId, MeshComponent *component);
    MeshComponentMultimap getMeshComponents() { return meshComponents; }
    std::pair<MeshComponentMultimap::iterator, MeshComponentMultimap::iterator> getMeshComponents(Entity *entity) { return getMeshComponents(entity->id); }
    std::pair<MeshComponentMultimap::iterator, MeshComponentMultimap::iterator> getMeshComponents(uint32_t entityId);


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
    std::unordered_map<Entity *, Component *> getComponents() {
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

    /// Velocity component
    void addVelocityComponent(uint32_t entityId, VelocityComponent *component);
    VelocityComponentMap getVelocityComponents() { return velocityComponents; }
    VelocityComponent* getVelocityComponent(Entity *entity) { return getVelocityComponent(entity->id); }
    VelocityComponent* getVelocityComponent(uint32_t entityId);

    /// Light component
    void addLightComponent(uint32_t entityId, LightComponent *component);
    LightComponentMap getLightComponents() { return lightComponents; }
    LightComponent* getLightComponent(Entity *entity) { return getLightComponent(entity->id); }
    LightComponent* getLightComponent(uint32_t entityId);

    /// Mass component
    void addMassComponent(uint32_t entityId, MassComponent *component);
    MassComponentMap getMassComponents() { return massComponents; }
    MassComponent* getMassComponent(Entity *entity) { return getMassComponent(entity->id); }
    MassComponent* getMassComponent(uint32_t entityId);

    /// Orbit component
    void addOrbitComponent(uint32_t entityId, OrbitComponent *component);
    OrbitComponentMap getOrbitComponents() { return orbitComponents; }
    OrbitComponent* getOrbitComponent(Entity *entity) { return getOrbitComponent(entity->id); }
    OrbitComponent* getOrbitComponent(uint32_t entityId);

private:
    /// Entities
    uint32_t getNextEntityId();
    uint32_t nextId = 0;
    std::vector<Entity *> entities;

    /// Components
    std::unordered_map<const char *, std::unordered_map<Entity *, Component *>> components;

    MeshComponentMultimap meshComponents;
    HighlightComponentMap highlightComponents;
    VelocityComponentMap velocityComponents;
    LightComponentMap lightComponents;
    MassComponentMap massComponents;
    OrbitComponentMap orbitComponents;

    template <class T> T* getComponent(std::unordered_map<uint32_t, T*> map, uint32_t entityId);
};

#endif //CG1_PROJECT_ENTITYMANAGER_H
