#ifndef CG1_PROJECT_VIEWFRUSTUM_H
#define CG1_PROJECT_VIEWFRUSTUM_H

#include <cg/GLHeader.h>
#include <cg/entities/components/CameraComponent.h>
#include "ViewPlane.h"


class ViewFrustum {
public:
    ViewFrustum(CameraComponent &camera, TransformComponent &cameraTransform);

    bool isInside(BoundingSphere &sphere);

    void update(float ratio);

private:
    CameraComponent &camera;
    TransformComponent &cameraTransform;

    float nearHeight, nearWidth;
    float farHeight, farWidth;

    glm::vec3 nearTopLeft, nearTopRight, nearBottomLeft, nearBottomRight;
    glm::vec3 farTopLeft, farTopRight, farBottomLeft, farBottomRight;

    ViewPlane planes[6];
};


#endif //CG1_PROJECT_VIEWFRUSTUM_H
