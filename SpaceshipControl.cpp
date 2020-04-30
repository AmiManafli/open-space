#include "SpaceshipControl.h"

SpaceshipControl::SpaceshipControl(Entity* spaceship, Entity* camera, EntityManager* entityManager) {
    this->cameraTransform = entityManager->getTransformComponent(camera->id);
    this->shaceshipTransform = entityManager->getTransformComponent(spaceship->id);
  
    this->cameraComponent = entityManager->getCameraComponent(camera->id);
    this->entityManager = entityManager;
}

void SpaceshipControl::processMouseMovement(float offsetX, float offsetY) {    
    offsetX *= cameraComponent->mouseSensitivity;
    offsetY *= cameraComponent->mouseSensitivity;
    cameraComponent->pitch += offsetY;
    cameraComponent->yaw -= offsetX;
}

void SpaceshipControl::processKeyboard(Entity* camera, CameraComponent::Direction direction, float deltaTime) {

    if(direction == cameraComponent->RollLeft) {
        cameraComponent->roll -= deltaTime * 50.0f;
    }
    else if (direction == cameraComponent->RollRight) {
        cameraComponent->roll += deltaTime * 50.0f;
    }    

}

void SpaceshipControl::processInput() {
    glm::vec3 eulerAngles = { glm::radians(cameraComponent->pitch), glm::radians(cameraComponent->yaw), glm::radians(0.0) };
    glm::quat pitchYaw(eulerAngles);
    cameraComponent->front = pitchYaw * glm::vec3(0.0, 0.0, -1.0);

    glm::quat roll = glm::angleAxis(glm::radians(cameraComponent->roll), cameraComponent->front);
    glm::quat final = roll * pitchYaw;

    cameraComponent->right = final * glm::vec3(1.0, 0.0, 0.0);
    cameraComponent->up =  (final * glm::vec3(0.0, 1.0, 0.0));
 }
