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

    yaw = 180.0f + glm::degrees(glm::atan((position.z - target.z) / (position.x - target.x)));

    auto q = glm::vec3(position.x, 0, position.z);
    pitch = -glm::degrees(glm::acos(glm::dot(position, q) / (glm::length(position) * glm::length(q))));
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

   update();
}

void Camera::update() {
    auto yawRad = glm::radians(yaw);
    auto pitchRad = glm::radians(pitch);

    glm::vec3 direction;
    direction.x = glm::cos(yawRad) * glm::cos(pitchRad);
    direction.y = glm::sin(pitchRad);
    direction.z = glm::sin(yawRad) * glm::cos(pitchRad);

    front = glm::normalize(direction);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

glm::mat4 Camera::getView() {
    if (targetingMode == Free) {
        return glm::lookAt(position, position + front, up);
    } else if (targetingMode == Target) {
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
        return glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
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
