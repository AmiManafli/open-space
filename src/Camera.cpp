#include "Camera.h"

Camera::Camera(Mode mode, glm::vec3 position)
        : mode(mode), position(position), yaw(-90.0f), pitch(0.0f), mouseSensitivity(0.01f), movementSpeed(2.5f) {
    front = glm::vec3(0, 0, -1);
    worldUp = glm::vec3(0, 1, 0);
    update();
}

void Camera::processKeyboard(Camera::Direction direction, float deltaTime) {
    float forwardVelocity = movementSpeed * deltaTime;
    float sideVelocity = movementSpeed * deltaTime * 0.01;
    if (direction == Forward) {
        position += front * forwardVelocity;
    } else if (direction == Backward) {
        position -= front * forwardVelocity;
    } else if (direction == Left) {
        position -= right * sideVelocity;
    } else if (direction == Right) {
        position += right * sideVelocity;
    }
}

void Camera::processMouseMovement(float offsetX, float offsetY) {
    offsetX *= mouseSensitivity;
    offsetY *= mouseSensitivity;

    yaw += offsetX;
    pitch = glm::clamp(pitch + offsetY, -89.0f, 89.0f);

    update();
}

void Camera::update() {
    glm::vec3 direction;
    direction.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    direction.y = glm::sin(glm::radians(pitch));
    direction.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    front = glm::normalize(direction);
    printf("direction: (%.3f, %.3f, %.3f)\n", front.x, front.y, front.z);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

glm::mat4 Camera::getView() {
    return glm::lookAt(position, position + front, up);
}
