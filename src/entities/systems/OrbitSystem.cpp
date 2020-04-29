#include "cg/entities/systems/OrbitSystem.h"

OrbitSystem::OrbitSystem(EntityManager *entityManager, GLContext *context)
        : System(entityManager), context(context) {
}

OrbitSystem::~OrbitSystem() {
}

void OrbitSystem::init() {
}

void OrbitSystem::update() {
    for (auto& pair : entityManager->getOrbitComponents()) {
        auto entityId = pair.first;
        auto orbit = pair.second;
        auto transform = entityManager->getTransformComponent(entityId);

        auto diff = orbit->getPosition() - transform->position;
        transform->move(diff);

        orbit->theta += orbit->speed * context->getDeltaTime();
    }
}
