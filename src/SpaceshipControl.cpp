#include "cg/SpaceshipControl.h"

SpaceshipControl::SpaceshipControl(Entity *spaceship, Entity *camera, EntityManager *entityManager)
{
    this->cameraTransform = entityManager->getComponent<TransformComponent>(camera);
    this->cameraComponent = entityManager->getComponent<CameraComponent>(camera);
    this->velocityComponent = entityManager->getComponent<VelocityComponent>(camera);
    this->entityManager = entityManager;
}

void SpaceshipControl::processMouseMovement(float offsetX, float offsetY)
{
    offsetX *= cameraComponent->mouseSensitivity;
    offsetY *= cameraComponent->mouseSensitivity;
    cameraComponent->pitch = glm::clamp(cameraComponent->pitch + offsetY, -89.0f, 89.0f);
    cameraComponent->yaw -= offsetX;
}

void SpaceshipControl::processKeyboard(Entity *camera, CameraComponent::Direction direction, float deltaTime)
{
    auto position = cameraTransform->position;
    float speed = cameraComponent->getSpeed() * deltaTime;

    if (direction == cameraComponent->RollLeft)
    {
        cameraComponent->roll -= deltaTime * 50.0f;
    }
    else if (direction == cameraComponent->RollRight)
    {
        cameraComponent->roll += deltaTime * 50.0f;
    }

    if (direction == cameraComponent->Forward)
    {
        velocityComponent->position += cameraComponent->front * speed;
    }
    else if (direction == cameraComponent->Backward)
    {
        velocityComponent->position -= cameraComponent->front * speed;
    }

    if (direction == cameraComponent->Left)
    {
        velocityComponent->position -= cameraComponent->right * speed;
    }
    else if (direction == cameraComponent->Right)
    {
        velocityComponent->position += cameraComponent->right * speed;
    }

    if (direction == cameraComponent->Down)
    {
        velocityComponent->position -= cameraComponent->up * speed;
    }
    else if (direction == cameraComponent->Up)
    {
        velocityComponent->position += cameraComponent->up * speed;
    }
}

void SpaceshipControl::processInput()
{
    glm::vec3 eulerAngles = {glm::radians(cameraComponent->pitch), glm::radians(cameraComponent->yaw), glm::radians(0.0)};
    glm::quat pitchYaw(eulerAngles);

    cameraComponent->front = pitchYaw * glm::vec3(0.0, 0.0, -1.0);

    glm::quat roll = glm::angleAxis(glm::radians(cameraComponent->roll), cameraComponent->front);
    glm::quat final = roll * pitchYaw;

    cameraComponent->right = final * glm::vec3(1.0, 0.0, 0.0);
    cameraComponent->up = glm::cross(cameraComponent->right, cameraComponent->front);
}
