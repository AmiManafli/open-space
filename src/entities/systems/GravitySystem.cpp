#include "cg/entities/systems/GravitySystem.h"

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

            printf("Length: %f\n", length);

            auto radius = 0.0001;
            if (length < radius) {
                velocity->gravity = glm::vec3(0, 0, 0);
            } else {
                const double gravitationalConstant = 0.1;
                auto force = static_cast<float>(gravitationalConstant * mass->mass * sunMass->mass) * unit / glm::pow(length, 2);
                printf("Position (m: %f): %s\n", mass->mass, glm::to_string(transform->position).c_str());
                printf("Force: %s\n", glm::to_string(force).c_str());
                velocity->gravity = force;
            }
        }
    }
}
