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
#include "Component.h"
#include "Entity.h"

typedef std::unordered_multimap<uint32_t, MeshComponent *> MeshComponentMultimap;
typedef std::unordered_map<uint32_t, CameraComponent *> CameraComponentMap;
typedef std::unordered_map<uint32_t, TransformComponent *> TransformComponentMap;
typedef std::unordered_map<uint32_t, HighlightComponent *> HighlightComponentMap;
typedef std::unordered_map<uint32_t, VelocityComponent *> VelocityComponentMap;
typedef std::unordered_map<uint32_t, LightComponent *> LightComponentMap;
typedef std::unordered_map<uint32_t, MassComponent *> MassComponentMap;


class EntityManager {
public:
    /// Entities
    Entity* createEntity();
    std::vector<Entity *> getEntities() { return entities; }

    /// Mesh component
    void addMeshComponent(uint32_t entityId, MeshComponent *component);
    MeshComponentMultimap getMeshComponents() { return meshComponents; }
    std::pair<MeshComponentMultimap::iterator, MeshComponentMultimap::iterator> getMeshComponents(uint32_t entityId);

    /// Transform component
    void addTransformComponent(uint32_t entityId, TransformComponent *component);
    TransformComponentMap getTransformComponents() { return transformComponents; }
    TransformComponent* getTransformComponent(uint32_t entityId);

    /// Camera component
    void addCameraComponent(uint32_t entityId, CameraComponent *component);
    CameraComponentMap getCameraComponents() { return cameraComponents; }
    CameraComponent* getCameraComponent(uint32_t entityId);

    /// Highlight component
    void addHighlightComponent(uint32_t entityId, HighlightComponent *component);
    HighlightComponentMap getHighlightComponents() { return highlightComponents; }
    HighlightComponent* getHighlightComponent(uint32_t entityId);

    /// Velocity component
    void addVelocityComponent(uint32_t entityId, VelocityComponent *component);
    VelocityComponentMap getVelocityComponents() { return velocityComponents; }
    VelocityComponent* getVelocityComponent(uint32_t entityId);

    /// Light component
    void addLightComponent(uint32_t entityId, LightComponent *component);
    LightComponentMap getLightComponents() { return lightComponents; }
    LightComponent* getLightComponent(uint32_t entityId);

    /// Mass component
    void addMassComponent(uint32_t entityId, MassComponent *component);
    MassComponentMap getMassComponents() { return massComponents; }
    MassComponent* getMassComponent(uint32_t entityId);

private:
    /// Entities
    uint32_t getNextEntityId();
    uint32_t nextId = 0;
    std::vector<Entity *> entities;

    /// Components
    MeshComponentMultimap meshComponents;
    TransformComponentMap transformComponents;
    CameraComponentMap cameraComponents;
    HighlightComponentMap highlightComponents;
    VelocityComponentMap velocityComponents;
    LightComponentMap lightComponents;
    MassComponentMap massComponents;

    template <class T> T* getComponent(std::unordered_map<uint32_t, T*> map, uint32_t entityId);
};

#endif //CG1_PROJECT_ENTITYMANAGER_H
