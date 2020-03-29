#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 target) : position(position), target(target) {
    up = glm::vec3(0, 1, 0);
    updateView();
}

void Camera::yaw(float angle) {
    auto theta = glm::atan(position.z / position.x);
    auto length = glm::sqrt(position.x * position.x + position.z * position.z);
    auto newAngle = theta + glm::radians(angle);

    position.x = length * glm::cos(newAngle);
    position.z = length * glm::sin(newAngle);

    // Update position
//    position.x = length * glm::cos(oldAngle + angle);
//    position.z = length * glm::cos(oldAngle + angle);

    updateView();
}

void Camera::updateView() {
    view = glm::lookAt(position, target, up);
}
