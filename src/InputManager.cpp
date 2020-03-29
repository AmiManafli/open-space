#include "InputManager.h"

InputManager::InputManager(GLContext *context) : context(context) {
}

void InputManager::init() {
    auto window = context->getWindow();
    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, mousePositionCallback);
}

void InputManager::process(double deltaTime) {
}

void InputManager::mousePositionCallback(GLFWwindow *window, double x, double y) {
    auto inputManager = (InputManager *) glfwGetWindowUserPointer(window);
    auto context = inputManager->context;
    auto camera = context->getCamera();
    auto deltaTime = context->getDeltaTime();

    double mouseSpeed = 8.0;

    double diffX = x - inputManager->lastMouseX;
    double diffY = y - inputManager->lastMouseY;

    auto angle = static_cast<float>(deltaTime * diffX * mouseSpeed);

    camera->yaw(angle);

    inputManager->lastMouseX = x;
    inputManager->lastMouseY = y;
}
