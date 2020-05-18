#include "cg/entities/components/CameraComponent.h"

CameraComponent::CameraComponent(CameraComponent::Mode mode, CameraComponent::Type type, glm::vec3 target,
                                 glm::vec3 front, glm::vec3 up, TransformComponent *positionComponent)
        : mode(mode), type(type), target(target), zoom(1.0f), mouseSensitivity(0.05f), movementSpeedTick(10), x(0), y(0), z(0), nearDistance(0.01f), farDistance(10000000.0f) {
    // Initialize vectors
    this->worldUp = glm::normalize(up);
    this->front = glm::normalize(front);
    this->right = glm::normalize(glm::cross(this->front, worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));

    x = 0;
    y = 0;
    z = 0;

    yaw = 0;
    pitch = 0;
    roll = 0;

    glm::vec3 angles(glm::radians(0.0), glm::radians(0.0), 0);
    this->orientation = glm::quat(angles);
}

glm::mat4 CameraComponent::getView(TransformComponent *positionComponent) {
    auto position = positionComponent->position;
    if (mode == FirstPersonShip) {
        auto view = glm::mat4_cast(glm::inverse(orientation));
        return glm::translate(view, -position);
    } else if (mode == Free) {
        return glm::lookAt(position, position + front, up);
    } else if (mode == CubeMap) {
        return glm::lookAt(position, position + front, up);
    } else {
        throw std::runtime_error("camera mode not implemented!");
    }
}

glm::mat4 CameraComponent::getProjection(float aspectRatio) {
    if (type == Orthographic) {
        auto orthoZoom = zoom;
        return glm::ortho(-aspectRatio * orthoZoom, aspectRatio * orthoZoom, -1.0f * orthoZoom, 1.0f * orthoZoom,
                          -1000.0f, 100.0f);
    } else if (type == Perspective) {
        auto fov = 45.0f / zoom;
        return glm::perspective(glm::radians(fov), aspectRatio, nearDistance, farDistance);
    } else if (type == CubeMapType) {
        return glm::perspective(static_cast<float>(PI / 2.0), 1.0f, 0.000001f, 100000000.0f);
    } else {
        throw std::runtime_error("failed to get projection: unknown projection mode");
    }
}

void CameraComponent::processKeyboard(CameraComponent::Direction direction, float deltaTime,
                                      TransformComponent *transformComponent) {
    auto position = transformComponent->position;
    float velocity = movementSpeedTick * deltaTime;
    if (direction == Forward) {
        position += front * velocity;
    } else if (direction == Backward) {
        position -= front * velocity;
    } else if (direction == Left) {
        position -= right * velocity;
    } else if (direction == Right) {
        position += right * velocity;
    }
    transformComponent->position = position;
}

void CameraComponent::processMouseMovement(float offsetX, float offsetY) {
    if (type == Orthographic) return;

    offsetX *= mouseSensitivity;
    offsetY *= mouseSensitivity;

    yaw += offsetX;
    pitch = glm::clamp(pitch + offsetY, -89.0f, 89.0f);

    updateVectors();
}

void CameraComponent::updateVectors() {
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