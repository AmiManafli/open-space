#include "cg/SpaceshipControl.h"

SpaceshipControl::SpaceshipControl(Entity *spaceship, Entity *camera, EntityManager *entityManager) {
    this->cameraTransform = entityManager->getComponent<TransformComponent>(camera);
    this->cameraComponent = entityManager->getComponent<CameraComponent>(camera);
    this->velocityComponent = entityManager->getComponent<VelocityComponent>(camera);
    this->entityManager = entityManager;
}

void SpaceshipControl::processMouseMovement(float offsetX, float offsetY) {
    offsetX *= cameraComponent->mouseSensitivity;
    offsetY *= cameraComponent->mouseSensitivity;
    cameraComponent->pitch = glm::clamp(cameraComponent->pitch + offsetY, -89.0f, 89.0f);
    cameraComponent->yaw -= offsetX;

    cameraComponent->x += offsetY;
    cameraComponent->y += offsetX;
}

void SpaceshipControl::processKeyboard(Entity *camera, CameraComponent::Direction direction, float deltaTime) {
    auto position = cameraTransform->position;
    float speed = cameraComponent->getSpeed() * deltaTime;
    float rollSpeed = 50.0 * deltaTime;

    if (direction == cameraComponent->RollLeft) {
        cameraComponent->z += rollSpeed;
    } else if (direction == cameraComponent->RollRight) {
        cameraComponent->z -= rollSpeed;
    }

    if (direction == cameraComponent->Forward) {
        auto translation = glm::vec3(0, 0, 1) * speed;
        velocityComponent->position = translation * glm::inverse(cameraComponent->orientation);
    } else if (direction == cameraComponent->Backward) {
        auto translation = glm::vec3(0, 0, -1) * speed;
        velocityComponent->position = translation * glm::inverse(cameraComponent->orientation);
    }

    if (direction == cameraComponent->Left) {
        auto translation = glm::vec3(1, 0, 0) * speed;
        velocityComponent->position = translation * glm::inverse(cameraComponent->orientation);
    } else if (direction == cameraComponent->Right) {
        auto translation = glm::vec3(-1, 0, 0) * speed;
        velocityComponent->position = translation * glm::inverse(cameraComponent->orientation);
    }

    if (direction == cameraComponent->Down) {
        auto translation = glm::vec3(0, 1, 0) * speed;
        velocityComponent->position = translation * glm::inverse(cameraComponent->orientation);
    } else if (direction == cameraComponent->Up) {
        auto translation = glm::vec3(0, -1, 0) * speed;
        velocityComponent->position = translation * glm::inverse(cameraComponent->orientation);
    }
}

void SpaceshipControl::processInput() {
    glm::vec3 angles(glm::radians(cameraComponent->x), glm::radians(cameraComponent->y), glm::radians(cameraComponent->z));
    glm::quat rotation(angles);
    cameraComponent->orientation = glm::normalize(cameraComponent->orientation * rotation);

    cameraComponent->x = 0;
    cameraComponent->y = 0;
    cameraComponent->z = 0;
}
