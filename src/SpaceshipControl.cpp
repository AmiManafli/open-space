#include "cg/SpaceshipControl.h"

SpaceshipControl::SpaceshipControl(Entity *spaceship, Entity *camera, EntityManager *entityManager, GLContext &context) : velocity(glm::vec3(0)), context(context) {
    this->cameraTransform = entityManager->getComponent<TransformComponent>(camera);
    this->cameraComponent = entityManager->getComponent<CameraComponent>(camera);
    this->velocityComponent = entityManager->getComponent<VelocityComponent>(camera);
    this->entityManager = entityManager;
    viewFrustum = new ViewFrustum(*cameraComponent, *cameraTransform);
}

SpaceshipControl::~SpaceshipControl() {
    delete viewFrustum;
}

void SpaceshipControl::processMouseMovement(float offsetX, float offsetY) {
    offsetX *= cameraComponent->mouseSensitivity;
    offsetY *= cameraComponent->mouseSensitivity;

    cameraComponent->x += offsetY;
    cameraComponent->y += offsetX;
}

double calculateRollSpeed(double x) {
    auto min = 10.0;
    auto max = 30.0;
    auto minX = 0.0;
    auto maxX = 40.0;
    auto k = (max - min) / (maxX - minX);
    return k * x + min;
}

void SpaceshipControl::processKeyboard(Entity *camera, CameraComponent::Direction direction, float deltaTime) {
    auto position = cameraTransform->position;
    auto rollSpeed = calculateRollSpeed(cameraComponent->movementSpeedTick) * deltaTime;

    if (direction == cameraComponent->RollLeft) {
        cameraComponent->z += rollSpeed;
    } else if (direction == cameraComponent->RollRight) {
        cameraComponent->z -= rollSpeed;
    }

    if (direction == cameraComponent->Forward) {
        auto translation = glm::vec3(0, 0, -1);
        velocity += translation * glm::inverse(cameraComponent->orientation);
    } else if (direction == cameraComponent->Backward) {
        auto translation = glm::vec3(0, 0, 1);
        velocity += translation * glm::inverse(cameraComponent->orientation);
    }

    if (direction == cameraComponent->Left) {
        auto translation = glm::vec3(-1, 0, 0);
        velocity += translation * glm::inverse(cameraComponent->orientation);
    } else if (direction == cameraComponent->Right) {
        auto translation = glm::vec3(1, 0, 0);
        velocity += translation * glm::inverse(cameraComponent->orientation);
    }

    if (direction == cameraComponent->Down) {
        auto translation = glm::vec3(0, -1, 0);
        velocity += translation * glm::inverse(cameraComponent->orientation);
    } else if (direction == cameraComponent->Up) {
        auto translation = glm::vec3(0, 1, 0);
        velocity += translation * glm::inverse(cameraComponent->orientation);
    }
}

void SpaceshipControl::processInput(float deltaTime) {
    glm::vec3 angles(glm::radians(cameraComponent->x), glm::radians(cameraComponent->y), glm::radians(cameraComponent->z));
    glm::quat rotation(angles);
    cameraComponent->orientation = glm::normalize(cameraComponent->orientation * rotation);
    cameraComponent->front = cameraComponent->orientation * glm::vec3(0, 0, -1);
    cameraComponent->up = cameraComponent->orientation * glm::vec3(0, 1, 0);
    cameraComponent->right = cameraComponent->orientation * glm::vec3(1, 0, 0);

    auto normalizedVelocity = glm::normalize(velocity);
    if (!(glm::isnan(normalizedVelocity.x) || glm::isnan(normalizedVelocity.y) || glm::isnan(normalizedVelocity.z))) {
        float speed = cameraComponent->getSpeed() * deltaTime;
        velocity = normalizedVelocity * speed;
    }

    viewFrustum->update(context.getAspect());

    velocityComponent->velocity = velocity;

    // Reset rotations
    velocity = glm::vec3(0);

    cameraComponent->x = 0;
    cameraComponent->y = 0;
    cameraComponent->z = 0;
}
