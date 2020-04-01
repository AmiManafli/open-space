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

    context->setActiveCamera(2);
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

    auto moveCamera = context->getCamera();
    if (!isDebug && isKeyDown(GLFW_KEY_W)) {
        moveCamera->processKeyboard(Camera::Direction::Forward, deltaTime);
    } else if (!isDebug && isKeyDown(GLFW_KEY_S)) {
        moveCamera->processKeyboard(Camera::Direction::Backward, deltaTime);
    }

    if (!isDebug && isKeyDown(GLFW_KEY_A)) {
        moveCamera->processKeyboard(Camera::Direction::Left, deltaTime);
    } else if (!isDebug && isKeyDown(GLFW_KEY_D)) {
        moveCamera->processKeyboard(Camera::Direction::Right, deltaTime);
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
    if (context->debug) return;

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
