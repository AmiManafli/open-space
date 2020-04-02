#ifndef CG1_PROJECT_ENTITYMANAGER_H
#define CG1_PROJECT_ENTITYMANAGER_H

#include <vector>
#include <unordered_map>
#include <entities/components/MeshComponent.h>
#include <entities/components/PositionComponent.h>
#include "Component.h"
#include "Entity.h"


typedef std::unordered_multimap<uint32_t, MeshComponent *> MeshComponentMultimap;
typedef std::unordered_map<uint32_t, PositionComponent *> PositionComponentMap;

class EntityManager {
public:
    /// Entities
    Entity* createEntity();
    std::vector<Entity *> getEntities() { return entities; }

    /// Mesh component
    void addMeshComponent(uint32_t entityId, MeshComponent *component);
    MeshComponentMultimap getMeshComponents() { return meshComponents; }
    std::pair<MeshComponentMultimap::iterator, MeshComponentMultimap::iterator> getMeshComponents(uint32_t entityId);

    /// Position component
    void addPositionComponent(uint32_t entityId, PositionComponent *component);
    PositionComponentMap getPositionComponents() { return positionComponents; }
    PositionComponent* getPositionComponent(uint32_t entityId);

private:
    /// Entities
    uint32_t getNextEntityId();
    uint32_t nextId = 0;
    std::vector<Entity *> entities;

    /// Components
    MeshComponentMultimap meshComponents;
    PositionComponentMap positionComponents;
};

#endif //CG1_PROJECT_ENTITYMANAGER_H
