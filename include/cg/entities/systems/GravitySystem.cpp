#include "GravitySystem.h"

GravitySystem::GravitySystem(EntityManager *entityManager) : System(entityManager) {
}

GravitySystem::~GravitySystem() {
}

void GravitySystem::init() {
}

void GravitySystem::update() {
    // TODO: Don't force a reference object
    std::optional<uint32_t> sunId;
    MassComponent *sunMass;
    TransformComponent *sunTransform;

    for (auto& pair : entityManager->getMassComponents()) {
        auto entityId = pair.first;
        auto mass = pair.second;
        auto transform = entityManager->getTransformComponent(entityId);
        auto velocity = entityManager->getVelocityComponent(entityId);

        if (!sunId.has_value()) {
            sunId = entityId;
            sunMass = mass;
            sunTransform = transform;
        } else {
            auto distance = transform->position - sunTransform->position;
            auto length = glm::length(distance);
            auto unit = glm::normalize(distance);

            auto radius = 1.8;
            if (length < radius) {
                velocity->gravity = glm::vec3(0, 0, 0);
            } else {
                const double gravitationalConstant = 1.0;
                auto force = static_cast<float>(gravitationalConstant * mass->mass * sunMass->mass) * unit / glm::pow(length, 2);
                velocity->gravity = force;
            }
        }
    }
}
