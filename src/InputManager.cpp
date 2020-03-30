#include "InputManager.h"

InputManager::InputManager(GLContext *context) : context(context) {
}

void InputManager::init() {
    auto window = context->getWindow();
    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, mousePositionCallback);

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

    if (isKeyPressed(GLFW_KEY_1)) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else if (isKeyPressed(GLFW_KEY_2)) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    if (isKeyDown(GLFW_KEY_W)) {
        camera->forward(deltaTime * moveSpeed);
    } else if (isKeyDown(GLFW_KEY_S)) {
        camera->backward(deltaTime * moveSpeed);
    }

    if (isKeyDown(GLFW_KEY_A)) {
        camera->strafeLeft(deltaTime * strafeSpeed);
    } else if (isKeyDown(GLFW_KEY_D)) {
        camera->strafeRight(deltaTime * strafeSpeed);
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

    double mouseSpeed = 0.5;

    double diffX = inputManager->lastMouseX == -1 ? 1.0 : x - inputManager->lastMouseX;
    double diffY = inputManager->lastMouseY == -1 ? 1.0 : inputManager->lastMouseY - y;

    auto yawAngle = static_cast<float>(deltaTime * diffX * mouseSpeed);
    auto pitchAngle = static_cast<float>(deltaTime * diffY * mouseSpeed);

    camera->rotate(yawAngle, pitchAngle, 0);

    inputManager->lastMouseX = x;
    inputManager->lastMouseY = y;
}
