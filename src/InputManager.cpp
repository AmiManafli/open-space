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
}

void InputManager::mousePositionCallback(GLFWwindow *window, double x, double y) {
    auto inputManager = (InputManager *) glfwGetWindowUserPointer(window);
    auto context = inputManager->context;
    auto camera = context->getCamera();
    auto deltaTime = context->getDeltaTime();

    double mouseSpeed = 8.0;

    double diffX = inputManager->lastMouseX == -1 ? 1.0 : x - inputManager->lastMouseX;
    double diffY = inputManager->lastMouseY == -1 ? 1.0 : x - inputManager->lastMouseY;

    auto yawAngle = static_cast<float>(deltaTime * diffX * mouseSpeed);
    auto pitchAngle = static_cast<float>(deltaTime * diffY * mouseSpeed);

    camera->yaw(yawAngle);
//    camera->pitch(pitchAngle);

    inputManager->lastMouseX = x;
    inputManager->lastMouseY = y;
}
