#include "cg/entities/systems/InputSystem.h"
#include "cg/SpaceshipControl.h"

InputSystem::InputSystem(EntityManager *entityManager, GLContext *context)
        : System(entityManager), context(context), lastMouseX(-1), lastMouseY(-1) {
}

void InputSystem::createSpaceshipControl(Entity *spaceship, Entity *camera) {
    spaceshipControl = new SpaceshipControl(spaceship, camera, entityManager);
}

void InputSystem::init() {
    auto window = context->getWindow();
    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, mousePositionCallback);
    glfwSetScrollCallback(window, processMouseScroll);
    glfwSetMouseButtonCallback(window, processMouseButton);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void InputSystem::update() {
    auto deltaTime = context->getDeltaTime();
    auto window = context->getWindow();
    auto isDebug = context->displayGui || context->displayCursor;

    if (isKeyPressed(GLFW_KEY_Q) && isKeyDown(GLFW_KEY_LEFT_CONTROL)) {
        glfwSetWindowShouldClose(context->getWindow(), GLFW_TRUE);
    }

    if (!isDebug && isKeyPressed(GLFW_KEY_UP)) {
        printf("Camera view: Top\n");
        context->setActiveCamera(context->topCamera);
    } else if (!isDebug && isKeyPressed(GLFW_KEY_LEFT)) {
        printf("Camera view: Side\n");
        context->setActiveCamera(context->sideCamera);
    } else if (!isDebug && isKeyPressed(GLFW_KEY_DOWN)) {
        printf("Camera view: Perspective\n");
        context->setActiveCamera(context->perspectiveCamera);
    }

    auto camera = context->getCamera();
    CameraComponent *cameraComponent = entityManager->getCameraComponent(camera->id);
    if (!isDebug && isKeyDown(GLFW_KEY_W)) {
        if (cameraComponent->mode == CameraComponent::FirstPersonShip) {
            spaceshipControl->processKeyboard(camera, CameraComponent::Direction::Forward, deltaTime);
        } else {
            moveCamera(camera, CameraComponent::Direction::Forward, deltaTime);
        }
    } else if (!isDebug && isKeyDown(GLFW_KEY_S)) {
        if (cameraComponent->mode == CameraComponent::FirstPersonShip) {
            spaceshipControl->processKeyboard(camera, CameraComponent::Direction::Backward, deltaTime);
        } else {
            moveCamera(camera, CameraComponent::Direction::Backward, deltaTime);
        }
    }

    if (!isDebug && isKeyDown(GLFW_KEY_Z)) {
        spaceshipControl->processKeyboard(camera, CameraComponent::Direction::RollLeft, deltaTime);
    } else if (!isDebug && isKeyDown(GLFW_KEY_X)) {
        spaceshipControl->processKeyboard(camera, CameraComponent::Direction::RollRight, deltaTime);
    }

    if (!isDebug && isKeyDown(GLFW_KEY_A)) {
        if (cameraComponent->mode == CameraComponent::FirstPersonShip) {
            spaceshipControl->processKeyboard(camera, CameraComponent::Direction::Left, deltaTime);
        } else {
            moveCamera(camera, CameraComponent::Direction::Left, deltaTime);
        }
    } else if (!isDebug && isKeyDown(GLFW_KEY_D)) {
        if (cameraComponent->mode == CameraComponent::FirstPersonShip) {
            spaceshipControl->processKeyboard(camera, CameraComponent::Direction::Right, deltaTime);
        } else {
            moveCamera(camera, CameraComponent::Direction::Right, deltaTime);
        }
    }
    if (!isDebug && isKeyDown(GLFW_KEY_Q)) {
        spaceshipControl->processKeyboard(camera, CameraComponent::Direction::Down, deltaTime);
    } else if (!isDebug && isKeyDown(GLFW_KEY_E)) {
        spaceshipControl->processKeyboard(camera, CameraComponent::Direction::Up, deltaTime);
    }


    if (!isDebug && isKeyPressed(GLFW_KEY_G)) {
        context->displayGrid = !context->displayGrid;
    }

    if (isKeyPressed(GLFW_KEY_BACKSLASH)) {
        context->displayGui = !context->displayGui;
    }

    //CameraComponent* cameraComponent = entityManager->getCameraComponent(camera->id);
    if (!isDebug && isKeyDown(GLFW_KEY_EQUAL)) {
        cameraComponent->movementSpeed += 0.1;
        printf("Camera movement speed: %.1f\n", cameraComponent->movementSpeed);
    } else if (!isDebug && isKeyDown(GLFW_KEY_MINUS)) {
        cameraComponent->movementSpeed -= 0.1;
        printf("Camera movement speed: %.1f\n", cameraComponent->movementSpeed);
    }

    if (!isDebug && isKeyDown(GLFW_KEY_1)) {
        context->setActiveCamera(context->spaceshipCamera);
    } else if (!isDebug && isKeyDown(GLFW_KEY_2)) {
        context->setActiveCamera(context->perspectiveCamera);
    } else if (!isDebug && isKeyDown(GLFW_KEY_3)) {
        context->setActiveCamera(context->topCamera);
    } else if (!isDebug && isKeyDown(GLFW_KEY_4)) {
        context->setActiveCamera(context->sideCamera);
    }


    if (isKeyPressed(GLFW_KEY_GRAVE_ACCENT)) {
        context->displayCursor = !context->displayCursor;
        if (context->displayCursor) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }

    spaceshipControl->processInput();
}

bool InputSystem::isKeyDown(int key) {
    return glfwGetKey(context->getWindow(), key) == GLFW_PRESS;
}

bool InputSystem::isKeyUp(int key) {
    return glfwGetKey(context->getWindow(), key) == GLFW_RELEASE;
}

bool InputSystem::isKeyPressed(int key) {
    auto keyStatus = glfwGetKey(context->getWindow(), key);
    bool pressed = false;
    if (keysDown[key] == GLFW_PRESS && keyStatus == GLFW_RELEASE) {
        pressed = true;
    }
    keysDown[key] = keyStatus;
    return pressed;
}

void InputSystem::mousePositionCallback(GLFWwindow *window, double x, double y) {
    auto inputManager = (InputSystem *) glfwGetWindowUserPointer(window);
    auto context = inputManager->context;

    auto offsetX = x - inputManager->lastMouseX;
    auto offsetY = inputManager->lastMouseY - y;

    inputManager->lastMouseX = x;
    inputManager->lastMouseY = y;

    if (context->displayCursor) return;

    auto camera = context->getCamera();
    auto cameraComponent = inputManager->entityManager->getCameraComponent(camera->id);

    if (!inputManager->processedMouse) {
        inputManager->lastMouseX = x;
        inputManager->lastMouseY = y;
        inputManager->processedMouse = true;
    }

    if (cameraComponent->mode == CameraComponent::FirstPersonShip) {
        inputManager->spaceshipControl->processMouseMovement(offsetX, offsetY);
    } else {
        cameraComponent->processMouseMovement(offsetX, offsetY);
    }
}

void InputSystem::processMouseScroll(GLFWwindow *window, double xoffset, double yoffset) {
    auto inputManager = (InputSystem *) glfwGetWindowUserPointer(window);
    auto context = inputManager->context;
    if (context->displayCursor) return;
    auto camera = context->getCamera();
    auto cameraComponent = inputManager->entityManager->getCameraComponent(camera->id);

    cameraComponent->movementSpeed += yoffset;
    printf("Camera movement speed: %.1f\n", cameraComponent->movementSpeed);

//zoom with the scroll
//    cameraComponent->zoom = glm::clamp(cameraComponent->zoom + (yoffset * (cameraComponent->zoom)) * 0.1, 0.5, 45.0);
}

void InputSystem::moveCamera(Entity *camera, CameraComponent::Direction direction, float deltaTime) {
    auto cameraComponent = entityManager->getCameraComponent(camera->id);
    auto transformComponent = entityManager->getTransformComponent(camera->id);
    cameraComponent->processKeyboard(direction, deltaTime, transformComponent);
}

void InputSystem::processMouseButton(GLFWwindow *window, int button, int action, int mods) {
    auto inputSystem = (InputSystem*)glfwGetWindowUserPointer(window);
    auto context = inputSystem->context;

    if (context->displayCursor) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
            double x, y;
            glfwGetCursorPos(window, &x, &y);

            auto entity = inputSystem->getClickedEntity(x, y);
            inputSystem->selectEntity(entity);
        }
    }
}

bool InputSystem::isRayInSphere(TransformComponent *transform, glm::vec3 origin, glm::vec3 ray) {
    double radius = transform->scaling.x;
    auto center = transform->position;

    double b = glm::dot(ray, origin - center);
    double c = glm::dot(origin - center, origin - center) - radius * radius;

    return b * b - c >= 0;
}

Entity *InputSystem::getClickedEntity(double mouseX, double mouseY) {
    Entity* foundEntity = nullptr;

    auto camera = entityManager->getCameraComponent(context->getCamera());
    auto cameraTransform = entityManager->getTransformComponent(context->getCamera());

    // Normalized device coordinates
    double x = (2.0 * mouseX) / context->getWidth() - 1.0;
    double y = 1.0 - (2.0 * mouseY) / context->getHeight();
    double z = 1.0;
    auto rayDevice = glm::vec3(x, y, z);

    // Clip coordinates
    auto rayClip = glm::vec4(rayDevice.x, rayDevice.y, -1.0, 1.0);

    // Eye coordinates
    auto rayEyeProjected = glm::inverse(camera->getProjection(context->getAspect())) * rayClip;
    auto rayEye = glm::vec4(rayEyeProjected.x, rayEyeProjected.y, -1.0, 0.0);

    // World coordinates
    auto viewEye = glm::inverse(camera->getView(cameraTransform)) * rayEye;
    auto rayWorld = glm::normalize(glm::vec3(viewEye));

    auto origin = cameraTransform->position;

    double entityDistance = DBL_MAX;
    for (auto& pair : entityManager->getTransformComponents()) {
        auto entityId = pair.first;
        auto transform = pair.second;

        if (entityId == context->getCamera()->id) continue;

        if (isRayInSphere(transform, origin, rayWorld)) {
            auto distance = glm::length(transform->position - origin);
            if (distance < entityDistance) {
                entityDistance = distance;
                foundEntity = entityManager->getEntity(entityId);
            }
        }
    }

    return foundEntity;
}

void InputSystem::selectEntity(Entity *entity) {
    auto previousEntity = context->selectedEntity;

    double highlightSize = 0.2;
    double highlightScale;
    if (entity) {
        auto transform = entityManager->getTransformComponent(entity);
        auto radius = transform->scaling.x / 2.0;
        highlightScale = (radius + highlightSize) / radius;
    } else {
        highlightScale = 1.1;
    }

    if (previousEntity) {
        entityManager->removeHighlightComponent(previousEntity);
        context->selectedEntity = entity;
        if (entity) {
            entityManager->addHighlightComponent(entity->id, new HighlightComponent(highlightScale, context->highlightProgram));
        }
    } else if (entity) {
        auto highlight = entityManager->getHighlightComponent(entity);

        if (!highlight) {
            entityManager->addHighlightComponent(entity->id, new HighlightComponent(highlightScale, context->highlightProgram));
        }
    }

    context->selectedEntity = entity;
}
