#include "cg/entities/systems/GravitySystem.h"

GravitySystem::GravitySystem(EntityManager *entityManager, Entity* object) : System(entityManager), object(object) {
}

GravitySystem::~GravitySystem() {
}

void GravitySystem::init() {
    objectTransform = entityManager->getComponent<TransformComponent>(object);
    objectVelocity = entityManager->getVelocityComponent(object);
    objectMass = entityManager->getMassComponent(object);
}

void GravitySystem::update() {
    auto totalForce = glm::vec3(0);

    for (auto& pair : entityManager->getMassComponents()) {
        auto entityId = pair.first;
        if (entityId == object->id) continue;

        auto mass = pair.second;
        auto transform = entityManager->getComponent<TransformComponent>(entityManager->getEntity(entityId));

        auto distance = transform->position - objectTransform->position;
        auto length = glm::length(distance);
        auto unit = glm::normalize(distance);

        const double gravitationalConstant = 0.7;
        auto force = -static_cast<float>(gravitationalConstant * objectMass->mass * mass->mass) * unit / pow(length, 2);

        totalForce += force;
    }

    objectVelocity->gravity = totalForce;
}
