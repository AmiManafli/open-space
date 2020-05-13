#include "cg/entities/systems/GravitySystem.h"

GravitySystem::GravitySystem(EntityManager *entityManager, Entity *object) : System(entityManager), object(object) {
}

GravitySystem::~GravitySystem() {
}

void GravitySystem::init() {
    objectTransform = entityManager->getComponent<TransformComponent>(object);
    objectVelocity = entityManager->getComponent<VelocityComponent>(object);
    objectMass = entityManager->getComponent<MassComponent>(object);
}

void GravitySystem::update() {
    auto totalForce = glm::vec3(0);

    if (!objectMass) return;

    for (auto &pair : entityManager->getComponents<MassComponent>()) {
        auto entity = pair.first;
        if (entity->id == object->id) continue;

        auto mass = dynamic_cast<MassComponent *>(pair.second);
        auto transform = entityManager->getComponent<TransformComponent>(entity);

        auto distance = transform->position - objectTransform->position;
        auto length = glm::length(distance);
        auto unit = glm::normalize(distance);

        const double gravitationalConstant = 0.7;
        auto force = static_cast<float>(gravitationalConstant * objectMass->mass * mass->mass) * unit / pow(length, 2);

        totalForce += force;
    }

    objectVelocity->gravity = totalForce;
}
