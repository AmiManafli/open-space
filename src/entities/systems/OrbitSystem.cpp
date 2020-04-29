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

        auto orbitPosition = orbit->getPosition();
        auto position = glm::vec3(orbitPosition.x, transform->position.y, orbitPosition.y);

        auto diff = position - transform->position;
        transform->move(diff);

        orbit->theta += 0.5 * context->getDeltaTime();
    }
}
