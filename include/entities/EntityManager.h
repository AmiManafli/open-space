#ifndef CG1_PROJECT_ENTITYMANAGER_H
#define CG1_PROJECT_ENTITYMANAGER_H

#include <vector>
#include <unordered_map>
#include <entities/components/PositionComponent.h>
#include "Component.h"

class EntityManager {
public:
    PositionComponent* getPositionComponent(uint32_t entityId) {
        return positionComponents.at(entityId);
    }
    void addPositionComponent(uint32_t entityId, PositionComponent* component) {
        positionComponents.emplace(entityId, component);
    }

private:
    // Components
    std::unordered_map<uint32_t, PositionComponent *> positionComponents;
};

#endif //CG1_PROJECT_ENTITYMANAGER_H
