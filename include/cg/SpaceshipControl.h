#pragma once
#include <cg/entities/Entity.h>
#include <cg/entities/EntityManager.h>
#include <cg/GLHeader.h>
#include "ViewFrustum.h"
#include "GLContext.h"

class SpaceshipControl
{
public:
    SpaceshipControl(Entity* spaceship, Entity* camera, EntityManager* entityManager, GLContext &context);
    ~SpaceshipControl();

    void processMouseMovement(float offsetX, float offsetY);
    void processKeyboard(Entity* camera, CameraComponent::Direction direction, float deltaTime);
    void processInput(float deltaTime);

private:
    TransformComponent* cameraTransform;
    TransformComponent* shaceshipTransform;
    CameraComponent* cameraComponent;
    VelocityComponent* velocityComponent;
    EntityManager* entityManager;
    ViewFrustum *viewFrustum;
    GLContext &context;

    glm::vec3 velocity;
};

