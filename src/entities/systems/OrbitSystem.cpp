#include "cg/entities/systems/OrbitSystem.h"

OrbitSystem::OrbitSystem(EntityManager &entityManager, GLContext &context)
        : System(entityManager), context(context) {
}

OrbitSystem::~OrbitSystem() {
}

void OrbitSystem::init() {
}

void OrbitSystem::update() {
    for (auto& pair : entityManager.getComponents<OrbitComponent>()) {
        auto entity = pair.first;
        auto orbit = dynamic_cast<OrbitComponent *>(pair.second);
        auto transform = entityManager.getComponent<TransformComponent>(entity);

        auto diff = orbit->getPosition() - transform->position;
        transform->move(diff);

        orbit->theta += orbit->speed * context.getDeltaTime();
    }
}
