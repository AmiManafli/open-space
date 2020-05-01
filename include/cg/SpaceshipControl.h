#pragma once
#include <cg/entities/Entity.h>
#include <cg/entities/EntityManager.h>
#include <cg/GLHeader.h>

class SpaceshipControl
{
public:
    SpaceshipControl(Entity* spaceship, Entity* camera, EntityManager* entityManager);

    void processMouseMovement(float offsetX, float offsetY);
    void processKeyboard(Entity* camera, CameraComponent::Direction direction, float deltaTime);
    void processInput();

private:
    TransformComponent* cameraTransform;
    TransformComponent* shaceshipTransform;
    CameraComponent* cameraComponent;
    EntityManager* entityManager;

};

