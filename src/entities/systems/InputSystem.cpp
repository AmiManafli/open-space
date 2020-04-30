#include "cg/entities/systems/InputSystem.h"
#include "../../../SpaceshipControl.h"

InputSystem::InputSystem(EntityManager *entityManager, GLContext *context)
        : System(entityManager), context(context), lastMouseX(-1), lastMouseY(-1) {
}

void InputSystem::createSpaceshipControl(Entity* spaceship, Entity* camera) {
    spaceshipControl = new SpaceshipControl(spaceship, camera, entityManager);
}

void InputSystem::init() {
    auto window = context->getWindow();
    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, mousePositionCallback);
    glfwSetScrollCallback(window, processMouseScroll);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void InputSystem::update() {
    auto deltaTime = context->getDeltaTime();
    auto window = context->getWindow();
    auto isDebug = context->displayGui || context->displayCursor;
	
	if (isKeyPressed(GLFW_KEY_Q)) {
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

    //if (!isDebug && isKeyPressed(GLFW_KEY_1)) {
    //    
    //} else if (!isDebug && isKeyPressed(GLFW_KEY_2)) {
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //}

    auto camera = context->getCamera();
    CameraComponent* cameraComponent = entityManager->getCameraComponent(camera->id);
    if (!isDebug && isKeyDown(GLFW_KEY_W)) {
        if (cameraComponent->mode == CameraComponent::FirstPersonShip) {
            moveCamera(camera, CameraComponent::Direction::Forward, deltaTime);
        }
        else {
            moveCamera(camera, CameraComponent::Direction::Forward, deltaTime);
        }
    } else if (!isDebug && isKeyDown(GLFW_KEY_S)) {
        if (cameraComponent->mode == CameraComponent::FirstPersonShip) {
            moveCamera(camera, CameraComponent::Direction::Backward, deltaTime);
        }
        else {
            moveCamera(camera, CameraComponent::Direction::Backward, deltaTime);
        }
    }

    if (!isDebug && isKeyDown(GLFW_KEY_Z)) {
        spaceshipControl->processKeyboard(camera, CameraComponent::Direction::RollLeft, deltaTime);
    }
    else if (!isDebug && isKeyDown(GLFW_KEY_X)) {
        spaceshipControl->processKeyboard(camera, CameraComponent::Direction::RollRight, deltaTime);
    }

    if (!isDebug && isKeyDown(GLFW_KEY_A)) {
        if (cameraComponent->mode == CameraComponent::FirstPersonShip) {
            moveCamera(camera, CameraComponent::Direction::Left, deltaTime);
        }
        else {
            moveCamera(camera, CameraComponent::Direction::Left, deltaTime);
        }
    } else if (!isDebug && isKeyDown(GLFW_KEY_D)) {
        if (cameraComponent->mode == CameraComponent::FirstPersonShip) {
            moveCamera(camera, CameraComponent::Direction::Right, deltaTime);
        }
        else {
            moveCamera(camera, CameraComponent::Direction::Right, deltaTime);
        }
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
    if (context->displayCursor) return;

    auto camera = context->getCamera();
    auto cameraComponent = inputManager->entityManager->getCameraComponent(camera->id);

    if (!inputManager->processedMouse) {
        inputManager->lastMouseX = x;
        inputManager->lastMouseY = y;
        inputManager->processedMouse = true;
    }

    auto offsetX = x - inputManager->lastMouseX;
    auto offsetY = inputManager->lastMouseY - y;


    if (cameraComponent->mode == CameraComponent::FirstPersonShip) {
        inputManager->spaceshipControl->processMouseMovement(offsetX, offsetY);
    }
    else {
        cameraComponent->processMouseMovement(offsetX, offsetY);
    }

    inputManager->lastMouseX = x;
    inputManager->lastMouseY = y;
}

void InputSystem::processMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
    auto inputManager = (InputSystem*)glfwGetWindowUserPointer(window);
    auto context = inputManager->context;
    if (context->displayCursor) return;

    auto camera = context->getCamera();
    auto cameraComponent = inputManager->entityManager->getCameraComponent(camera->id);
    cameraComponent->zoom = glm::clamp(cameraComponent->zoom + (yoffset * (cameraComponent->zoom)) * 0.1, 0.5, 45.0);
}

void InputSystem::moveCamera(Entity *camera, CameraComponent::Direction direction, float deltaTime) {
    auto cameraComponent = entityManager->getCameraComponent(camera->id);
    auto transformComponent = entityManager->getTransformComponent(camera->id);
    cameraComponent->processKeyboard(direction, deltaTime, transformComponent);
}
