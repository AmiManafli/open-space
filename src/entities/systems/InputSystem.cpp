#include "cg/entities/systems/InputSystem.h"

InputSystem::InputSystem(EntityManager *entityManager, GLContext *context)
        : System(entityManager), context(context), lastMouseX(-1), lastMouseY(-1) {
}

void InputSystem::init() {
    auto window = context->getWindow();
    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, mousePositionCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void InputSystem::update() {
    auto deltaTime = context->getDeltaTime();
    auto isDebug = context->debug;
    auto window = context->getWindow();
    auto cameras = context->getCameras();

    if (!isDebug && isKeyPressed(GLFW_KEY_Q)) {
        glfwSetWindowShouldClose(window, true);
    }

    if (isKeyPressed(GLFW_KEY_UP)) {
        printf("Camera view: Top\n");
        context->setActiveCamera(0);
    } else if (isKeyPressed(GLFW_KEY_LEFT)) {
        printf("Camera view: Side\n");
        context->setActiveCamera(1);
    } else if (isKeyPressed(GLFW_KEY_DOWN)) {
        printf("Camera view: Perspective\n");
        context->setActiveCamera(2);
    }

    if (isKeyPressed(GLFW_KEY_1)) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else if (isKeyPressed(GLFW_KEY_2)) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    auto camera = context->getCamera();
    if (!isDebug && isKeyDown(GLFW_KEY_W)) {
        moveCamera(camera, CameraComponent::Direction::Forward, deltaTime);
    } else if (!isDebug && isKeyDown(GLFW_KEY_S)) {
        moveCamera(camera, CameraComponent::Direction::Backward, deltaTime);
    }

    if (!isDebug && isKeyDown(GLFW_KEY_A)) {
        moveCamera(camera, CameraComponent::Direction::Left, deltaTime);
    } else if (!isDebug && isKeyDown(GLFW_KEY_D)) {
        moveCamera(camera, CameraComponent::Direction::Right, deltaTime);
    }

    if (isKeyPressed(GLFW_KEY_G)) {
        context->displayGrid = !context->displayGrid;
    }

    if (isKeyPressed(GLFW_KEY_GRAVE_ACCENT)) {
        context->debug = !isDebug;
        isDebug = !isDebug;

        if (isDebug) {
            printf("Debug mode: On\n");
            context->displayGui = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            printf("Debug mode: Off\n");
            context->displayGui = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
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
    if (context->debug) return;

    auto camera = context->getCamera();
    auto cameraComponent = inputManager->entityManager->getCameraComponent(camera->id);

    if (!inputManager->processedMouse) {
        inputManager->lastMouseX = x;
        inputManager->lastMouseY = y;
        inputManager->processedMouse = true;
    }

    auto offsetX = x - inputManager->lastMouseX;
    auto offsetY = inputManager->lastMouseY - y;

    cameraComponent->processMouseMovement(offsetX, offsetY);

    inputManager->lastMouseX = x;
    inputManager->lastMouseY = y;
}

void InputSystem::moveCamera(Entity *camera, CameraComponent::Direction direction, float deltaTime) {
    auto cameraComponent = entityManager->getCameraComponent(camera->id);
    auto positionComponent = entityManager->getPositionComponent(camera->id);

    cameraComponent->processKeyboard(direction, deltaTime, positionComponent);
}
