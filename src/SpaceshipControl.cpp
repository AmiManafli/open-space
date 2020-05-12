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

    if (direction == cameraComponent->RollLeft) {
        cameraComponent->z += deltaTime * 50.0f;
    } else if (direction == cameraComponent->RollRight) {
        cameraComponent->z -= deltaTime * 50.0f;
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
//    glm::vec3 eulerAngles = {glm::radians(cameraComponent->pitch), glm::radians(cameraComponent->yaw),
//                             glm::radians(0.0)};
//    glm::quat pitchYaw(eulerAngles);
//
//    cameraComponent->front = pitchYaw * glm::vec3(0.0, 0.0, -1.0);
//
//    glm::quat roll = glm::angleAxis(glm::radians(cameraComponent->roll), cameraComponent->front);
//    glm::quat final = roll * pitchYaw;
//
//    cameraComponent->right = final * glm::vec3(1.0, 0.0, 0.0);
//    cameraComponent->up = glm::cross(cameraComponent->right, cameraComponent->front);

    auto yawRad = glm::radians(cameraComponent->yaw);
    auto pitchRad = glm::radians(cameraComponent->pitch);
    auto rollRad = glm::radians(cameraComponent->roll);

    glm::vec3 direction;
    direction.x = glm::cos(yawRad) * glm::cos(pitchRad);
    direction.y = glm::sin(pitchRad);
    direction.z = glm::sin(yawRad) * glm::cos(pitchRad);

    glm::vec3 angles(glm::radians(cameraComponent->x), glm::radians(cameraComponent->y), glm::radians(cameraComponent->z));
    glm::quat rotation(angles);
    cameraComponent->orientation = glm::normalize(cameraComponent->orientation * rotation);

    glm::vec3 newWorldUp;
    newWorldUp.x = glm::cos(rollRad + PI / 2.0);
    newWorldUp.y = glm::sin(rollRad + PI / 2.0);
    newWorldUp.z = 0;
    cameraComponent->worldUp = glm::normalize(newWorldUp);

    cameraComponent->front = glm::normalize(direction);
    cameraComponent->up = cameraComponent->worldUp;
    cameraComponent->right = glm::cross(cameraComponent->front, cameraComponent->up);

    cameraComponent->x = 0;
    cameraComponent->y = 0;
    cameraComponent->z = 0;
//
//    printf("World: %s\n   Up: %s\nRight: %s\n\n", glm::to_string(cameraComponent->worldUp).c_str(), glm::to_string(cameraComponent->up).c_str(), glm::to_string(cameraComponent->right).c_str());
}
