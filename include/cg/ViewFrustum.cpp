#include "ViewFrustum.h"

ViewFrustum::ViewFrustum(CameraComponent &camera, TransformComponent &cameraTransform)
        : camera(camera), cameraTransform(cameraTransform) {
}

void ViewFrustum::update(float ratio) {
    auto fov = 45.0f / camera.zoom;

    nearHeight = 2 * glm::tan(fov / 2) * camera.nearDistance;
    nearWidth = nearHeight * ratio;

    farHeight = 2 * glm::tan(fov / 2) * camera.farDistance;
    farWidth = farHeight * ratio;

    auto nearCenter = cameraTransform.position + camera.front * camera.nearDistance;
    auto farCenter = cameraTransform.position + camera.front * camera.farDistance;

    nearTopLeft = nearCenter + camera.up * (nearHeight / 2.0f) - camera.right * (nearWidth / 2.0f);
    nearTopRight = nearCenter + camera.up * (nearHeight / 2.0f) + camera.right * (nearWidth / 2.0f);
    nearBottomLeft = nearCenter - camera.up * (nearHeight / 2.0f) - camera.right * (nearWidth / 2.0f);
    nearBottomRight = nearCenter - camera.up * (nearHeight / 2.0f) + camera.right * (nearWidth / 2.0f);

    farTopLeft = farCenter + camera.up * (farHeight / 2.0f) - camera.right * (farWidth / 2.0f);
    farTopRight = farCenter + camera.up * (farHeight / 2.0f) + camera.right * (farWidth / 2.0f);
    farBottomLeft = farCenter - camera.up * (farHeight / 2.0f) - camera.right * (farWidth / 2.0f);
    farBottomRight = farCenter - camera.up * (farHeight / 2.0f) + camera.right * (farWidth / 2.0f);

    // Near plane
    planes[0] = ViewPlane(nearTopLeft, nearTopRight, nearBottomLeft);
    // Far plane
    planes[1] = ViewPlane(farTopRight, farTopLeft, farBottomRight);
    // Left plane
    planes[2] = ViewPlane(farTopLeft, nearTopLeft, farBottomLeft);
    // Right plane
    planes[3] = ViewPlane(nearTopRight, farTopRight, nearBottomRight);
    // Top plane
    planes[4] = ViewPlane(nearTopLeft, farTopLeft, nearTopRight);
    // Bottom plane
    planes[5] = ViewPlane(nearBottomRight, farBottomRight, nearBottomLeft);
}

bool ViewFrustum::isInside(BoundingSphere &sphere) {
    float distanceNear = planes[0].distance(sphere);
    float distanceFar = planes[1].distance(sphere);
    float distanceLeft = planes[2].distance(sphere);
    float distanceRight = planes[3].distance(sphere);
    float distanceTop = planes[4].distance(sphere);
    float distanceBottom = planes[5].distance(sphere);
    printf("Distances:\n  near: %f\n  far: %f\n  left: %f\n  right: %f\n  top: %f\n  bottom: %f\n\n", distanceNear, distanceFar, distanceLeft, distanceRight, distanceTop, distanceBottom);
    for (int i = 0; i < 6; i++) {
        auto distance = planes[i].distance(sphere);
        if (distance < -sphere.getRadius()) {
            return false;
        }
    }
    return true;
}
