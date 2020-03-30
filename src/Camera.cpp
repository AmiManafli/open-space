#include "Camera.h"

Camera::Camera(Mode mode, glm::vec3 position)
        : mode(mode), position(position), yawAngle(-90), pitchAngle(0), rollAngle(0) {
    up = glm::vec3(0, 1, 0);
    direction = glm::vec3(0, 0, -1);
    updateView();
}

void Camera::forward(float amount) {
    position = position + amount * direction;

    updateView();
}

void Camera::backward(float amount) {
    position = position - amount * direction;

    updateView();
}

void Camera::strafeLeft(float amount) {
    right = glm::normalize(glm::cross(direction, up));
    position = position - amount * right;

    printf("Position: (%.1f, %.1f, %.1f)\n", position.x, position.y, position.z);

    updateView();
}

void Camera::strafeRight(float amount) {
    right = glm::normalize(glm::cross(direction, up));
    position = position + amount * right;

    updateView();
}

void Camera::rotate(float yaw, float pitch, float roll) {
    yawAngle += yaw;
    pitchAngle = glm::clamp(pitchAngle + pitch, -89.0f, 89.0f);
    rollAngle += roll;

    direction.x = glm::cos(glm::radians(yawAngle)) * glm::cos(glm::radians(pitchAngle));
    direction.y = glm::sin(glm::radians(pitchAngle));
    direction.z = glm::sin(glm::radians(yawAngle)) * glm::cos(glm::radians(pitchAngle));
    direction = glm::normalize(direction);

    updateView();
}

void Camera::updateView() {
    if (mode == Free) {
        view = glm::lookAt(position, position + direction, up);
    } else {
        throw std::runtime_error("camera mode not implemented");
    }
}
