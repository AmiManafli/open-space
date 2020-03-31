#include "InputManager.h"

InputManager::InputManager(GLContext *context) : context(context) {
    topCamera = new Camera(Camera::Mode::Free, glm::vec3(0, 15, 0), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0));
    sideCamera = new Camera(Camera::Mode::Free, glm::vec3(0, 0, 15));

    context->setCamera(topCamera);
}

void InputManager::init() {
    auto window = context->getWindow();
    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, mousePositionCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    lastMouseX = -1;
    lastMouseY = -1;
}

void InputManager::process(double deltaTime) {
    auto window = context->getWindow();
    auto camera = context->getCamera();
    float moveSpeed = 100.0f;
    float strafeSpeed = 5.0f;

    if (isKeyPressed(GLFW_KEY_Q)) {
        glfwSetWindowShouldClose(window, true);
    }

    if (isKeyPressed(GLFW_KEY_UP)) {
        printf("Switched top top-view camera!\n");
        context->setCamera(topCamera);
    } else if (isKeyPressed(GLFW_KEY_LEFT)) {
        printf("Switched top side-view camera!\n");
        context->setCamera(sideCamera);
    }

    if (isKeyPressed(GLFW_KEY_1)) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else if (isKeyPressed(GLFW_KEY_2)) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    if (isKeyDown(GLFW_KEY_W)) {
        camera->processKeyboard(Camera::Direction::Forward, deltaTime);
    } else if (isKeyDown(GLFW_KEY_S)) {
        camera->processKeyboard(Camera::Direction::Backward, deltaTime);
    }

    if (isKeyDown(GLFW_KEY_A)) {
        camera->processKeyboard(Camera::Direction::Left, deltaTime);
    } else if (isKeyDown(GLFW_KEY_D)) {
        camera->processKeyboard(Camera::Direction::Right, deltaTime);
    }
}

bool InputManager::isKeyDown(int key) {
    return glfwGetKey(context->getWindow(), key) == GLFW_PRESS;
}

bool InputManager::isKeyUp(int key) {
    return glfwGetKey(context->getWindow(), key) == GLFW_RELEASE;
}

bool InputManager::isKeyPressed(int key) {
    auto keyStatus = glfwGetKey(context->getWindow(), key);
    bool pressed = false;
    if (keysDown[key] == GLFW_PRESS && keyStatus == GLFW_RELEASE) {
        pressed = true;
    }
    keysDown[key] = keyStatus;
    return pressed;
}

void InputManager::mousePositionCallback(GLFWwindow *window, double x, double y) {
    auto inputManager = (InputManager *) glfwGetWindowUserPointer(window);
    auto context = inputManager->context;
    auto camera = context->getCamera();
    auto deltaTime = context->getDeltaTime();

    if (!inputManager->processedMouse) {
        inputManager->lastMouseX = x;
        inputManager->lastMouseY = y;
        inputManager->processedMouse = true;
    }

    auto offsetX = x - inputManager->lastMouseX;
    auto offsetY = inputManager->lastMouseY - y;

    camera->processMouseMovement(offsetX, offsetY);

    inputManager->lastMouseX = x;
    inputManager->lastMouseY = y;
}
