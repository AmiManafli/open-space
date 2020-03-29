#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 target) : position(position), target(target) {
    up = glm::vec3(0, 1, 0);
    updateView();
}

void Camera::pitch(float angle) {
    auto theta = glm::atan(position.y / position.z);
    auto length = glm::sqrt(position.y * position.y + position.z * position.z);
    auto newAngle = theta + glm::radians(angle);

    // Update position
    position.y = length * glm::sin(newAngle);
    position.z = length * glm::cos(newAngle);

    updateView();
}

void Camera::yaw(float angle) {
    auto theta = glm::atan(position.z / position.x);
    auto length = glm::sqrt(position.x * position.x + position.z * position.z);
    auto newAngle = theta + glm::radians(angle);

    // Update position
    auto oldPosition = glm::vec3(position.x, position.y, position.z);
    position.x = length * glm::cos(newAngle);
    position.z = length * glm::sin(newAngle);

    updateView();
}

void Camera::updateView() {
    view = glm::lookAt(position, target, up);
}
