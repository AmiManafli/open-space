#include "Camera.h"

Camera::Camera(Mode mode, glm::vec3 position)
        : Camera(mode, position, glm::vec3(0, 1, 0), glm::vec3(0, 0, -1)) {
}

Camera::Camera(Mode mode, glm::vec3 position, glm::vec3 up, glm::vec3 front)
        : mode(mode), position(position), target(glm::vec3(0, 0, 0)),
          mouseSensitivity(0.1f), movementSpeed(2.5f) {
    this->targetingMode = Free;
    this->up = glm::normalize(up);
    this->worldUp = this->up;
    this->front = glm::normalize(front);
    this->right = glm::normalize(glm::cross(front, up));
    target = glm::vec3(0, 0, 0);
    view = glm::lookAt(position, front, up);

    yaw = glm::degrees(glm::atan(position.z / position.x));
}

void Camera::processKeyboard(Camera::Direction direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    if (direction == Forward) {
        position += front * velocity;
    } else if (direction == Backward) {
        position -= front * velocity;
    } else if (direction == Left) {
        position -= right * velocity;
        target -= right * velocity;
    } else if (direction == Right) {
        position += right * velocity;
        target += right * velocity;
    }
}

void Camera::processMouseMovement(float offsetX, float offsetY) {
    offsetX *= mouseSensitivity;
    offsetY *= mouseSensitivity;

    yaw += offsetX;
    pitch = glm::clamp(pitch + offsetY, -89.0f, 89.0f);

    auto pos = glm::vec2(position.x, position.z);
    auto length = glm::length(pos);
    auto angle = glm::atan(position.z / position.x) + glm::radians(offsetX);
    position.x = length * cos(angle);
    position.z = length * sin(angle);
}

void Camera::update() {
}

glm::mat4 Camera::getView() {
    if (mode == Free) {
        return glm::lookAt(position, target, up);
    } else if (mode == Target) {
        return glm::lookAt(position, target, up);
    } else {
        throw std::runtime_error("camera mode not implemented!");
    }
}

glm::mat4 Camera::getProjection(float aspectRatio) {
    if (mode == Orthographic) {
        auto zoomOut = 1.5f;
        return glm::ortho(-aspectRatio * zoomOut, aspectRatio * zoomOut, -1.0f * zoomOut, 1.0f * zoomOut, -1000.0f, 100.0f);
    } else if (mode == Perspective) {
        return glm::perspective(45.0f, aspectRatio, 0.1f, 100.0f);
    } else {
        throw std::runtime_error("failed to get projection: unknown projection mode");
    }
}

void Camera::draw(ShaderProgram& shaderProgram) {
    float scaleFactor = 1.0f;
    auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor, scaleFactor, scaleFactor));
    auto translation = glm::translate(scale, position);
//    auto model = glm::rotate(translation, yaw, front);
    meshes[0].draw(shaderProgram, translation);
}
