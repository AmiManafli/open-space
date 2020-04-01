#include "InputManager.h"

InputManager::InputManager(GLContext *context) : context(context) {
    auto topCamera = new Camera(Camera::Mode::Orthographic, glm::vec3(0, 5, 0), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0));
    auto sideCamera = new Camera(Camera::Mode::Orthographic, glm::vec3(0, 0, 5), glm::vec3(0, 1, 0), glm::vec3(0, 0, -1));
    auto perspectiveCamera = new Camera(Camera::Mode::Perspective, glm::vec3(5, 5, 10), glm::vec3(0, 1, 0), glm::normalize(glm::vec3(-5, -5, -10)));
    perspectiveCamera->setTarget(glm::vec3(0, 0, 0));

    auto cameraMeshes = Model::loadModel("../assets/models/camera.dae");
    topCamera->meshes = cameraMeshes;
    sideCamera->meshes = cameraMeshes;
    perspectiveCamera->meshes = cameraMeshes;

    context->addCamera(topCamera);
    context->addCamera(sideCamera);
    context->addCamera(perspectiveCamera);
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
    auto cameras = context->getCameras();
    float moveSpeed = 100.0f;
    float strafeSpeed = 5.0f;

    if (isKeyPressed(GLFW_KEY_Q)) {
        glfwSetWindowShouldClose(window, true);
    }

    if (isKeyPressed(GLFW_KEY_UP)) {
        printf("Switched top top-view camera!\n");
        context->setActiveCamera(0);
    } else if (isKeyPressed(GLFW_KEY_LEFT)) {
        printf("Switched top side-view camera!\n");
        context->setActiveCamera(1);
    } else if (isKeyPressed(GLFW_KEY_DOWN)) {
        printf("Switched top perspective-view camera!\n");
        context->setActiveCamera(2);
    }

    if (isKeyPressed(GLFW_KEY_1)) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else if (isKeyPressed(GLFW_KEY_2)) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    auto moveCamera = cameras[0];
    if (isKeyDown(GLFW_KEY_W)) {
        moveCamera->processKeyboard(Camera::Direction::Forward, deltaTime);
    } else if (isKeyDown(GLFW_KEY_S)) {
        moveCamera->processKeyboard(Camera::Direction::Backward, deltaTime);
    }

    if (isKeyDown(GLFW_KEY_A)) {
        moveCamera->processKeyboard(Camera::Direction::Left, deltaTime);
    } else if (isKeyDown(GLFW_KEY_D)) {
        moveCamera->processKeyboard(Camera::Direction::Right, deltaTime);
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
    auto camera = context->getCameras()[2];
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
