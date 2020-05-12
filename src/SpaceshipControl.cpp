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
}

void SpaceshipControl::processKeyboard(Entity *camera, CameraComponent::Direction direction, float deltaTime) {
    auto position = cameraTransform->position;
    float speed = cameraComponent->getSpeed() * deltaTime;

    if (direction == cameraComponent->RollLeft) {
        cameraComponent->roll += deltaTime * 50.0f;
    } else if (direction == cameraComponent->RollRight) {
        cameraComponent->roll -= deltaTime * 50.0f;
    }

    if (direction == cameraComponent->Forward) {
        velocityComponent->position += cameraComponent->front * speed;
    } else if (direction == cameraComponent->Backward) {
        velocityComponent->position -= cameraComponent->front * speed;
    }

    if (direction == cameraComponent->Left) {
        velocityComponent->position -= cameraComponent->right * speed;
    } else if (direction == cameraComponent->Right) {
        velocityComponent->position += cameraComponent->right * speed;
    }

    if (direction == cameraComponent->Down) {
        velocityComponent->position -= cameraComponent->up * speed;
    } else if (direction == cameraComponent->Up) {
        velocityComponent->position += cameraComponent->up * speed;
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

    glm::vec3 newWorldUp;
    newWorldUp.x = glm::cos(rollRad + PI / 2.0);
    newWorldUp.y = glm::sin(rollRad + PI / 2.0);
    newWorldUp.z = 0;
    cameraComponent->worldUp = newWorldUp;

    cameraComponent->front = glm::normalize(direction);
    cameraComponent->right = glm::normalize(glm::cross(cameraComponent->front, newWorldUp));
    cameraComponent->up = glm::normalize(glm::cross(cameraComponent->right, cameraComponent->front));

    printf("World: %s\n   Up: %s\nRight: %s\n\n", glm::to_string(cameraComponent->worldUp).c_str(), glm::to_string(cameraComponent->up).c_str(), glm::to_string(cameraComponent->right).c_str());
}
