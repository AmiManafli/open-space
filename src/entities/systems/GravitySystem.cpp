#include "cg/entities/systems/GravitySystem.h"

GravitySystem::GravitySystem(EntityManager &entityManager) : System(entityManager) {
}

GravitySystem::~GravitySystem() {
}

void GravitySystem::init(Entity *object) {
    this->object = object;

    if (entityManager.hasComponent<TransformComponent>(object)) {
        objectTransform = entityManager.getComponent<TransformComponent>(object);
    }
    if (entityManager.hasComponent<VelocityComponent>(object)) {
        objectVelocity = entityManager.getComponent<VelocityComponent>(object);
    }
    if (entityManager.hasComponent<MassComponent>(object)) {
        objectMass = entityManager.getComponent<MassComponent>(object);
    }
}

void GravitySystem::update() {
    auto totalForce = glm::vec3(0);

    if (!objectMass) return;

    for (auto &pair : entityManager.getComponents<MassComponent>()) {
        auto entity = pair.first;
        if (entity->id == object->id) continue;

        auto mass = dynamic_cast<MassComponent *>(pair.second);
        auto transform = entityManager.getComponent<TransformComponent>(entity);

        auto distance = objectTransform->position - transform->position;
        auto length = glm::length(distance);
        auto unit = glm::normalize(distance);

        const double gravitationalConstant = 2.0;
        auto force = static_cast<float>(gravitationalConstant * objectMass->mass * mass->mass) * unit / static_cast<float>(pow(length, 2));

        totalForce += force;
    }

    objectVelocity->gravity = totalForce;
}
