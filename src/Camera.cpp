#include "Camera.h"

Camera::Camera(Mode mode, glm::vec3 position)
        : Camera(mode, position, glm::vec3(0, 1, 0), glm::vec3(0, 0, -1)) {
}

Camera::Camera(Mode mode, glm::vec3 position, glm::vec3 up, glm::vec3 front)
        : mode(mode), position(position),
          mouseSensitivity(0.1f), movementSpeed(2.5f) {
    yaw = 0.0f;
    pitch = 0.0f;

//    update();
    this->up = glm::normalize(up);
    this->worldUp = this->up;
    this->front = glm::normalize(front);
    this->right = glm::normalize(glm::cross(front, up));
}

void Camera::processKeyboard(Camera::Direction direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    if (direction == Forward) {
        position += front * velocity;
    } else if (direction == Backward) {
        position -= front * velocity;
    } else if (direction == Left) {
        position -= right * velocity;
    } else if (direction == Right) {
        position += right * velocity;
    }
}

void Camera::processMouseMovement(float offsetX, float offsetY) {
    offsetX *= mouseSensitivity;
    offsetY *= mouseSensitivity;

    yaw += offsetX;
//    pitch = glm::clamp(pitch + offsetY, -89.0f, 89.0f);

    update();
}

void Camera::update() {
    glm::vec3 direction;
    direction.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    direction.y = glm::sin(glm::radians(pitch));
    direction.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));

    front = glm::normalize(direction);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));

    printf("Yaw: %.3f degrees\n", yaw);
}

glm::mat4 Camera::getView() {
    if (mode == Free) {
        return glm::lookAt(position, position + front, up);
    } else if (mode == Target) {
        return glm::lookAt(position, target, up);
    } else {
        throw std::runtime_error("camera mode not implemented!");
    }
}

void Camera::draw(ShaderProgram &shaderProgram) {
//    printf("Position: (%.3f, %.3f, %.3f)\n", position.x, position.y, position.z);
    auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.01, 0.01, 0.01));
    auto translation = glm::translate(scale, position);
    auto model = glm::rotate(translation, yaw, up);
    meshes[0].draw(shaderProgram, model);
}
