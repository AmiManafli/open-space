#ifndef CG1_PROJECT_CAMERACOMPONENT_H
#define CG1_PROJECT_CAMERACOMPONENT_H

#include <cg/GLHeader.h>
#include <cg/entities/Component.h>
#include "TransformComponent.h"


class CameraComponent : public Component {
public:
    enum Mode {
        FirstPersonShip,
        Free,
        CubeMap,
    };
    enum Type {
        Orthographic,
        Perspective,
        CubeMapType,
    };
    enum Direction {
        Forward,
        Backward,
        Left,
        Right,
        RollLeft,
        RollRight,
        Up,
        Down,
    };

    CameraComponent(Mode mode, Type type, glm::vec3 target, glm::vec3 front, glm::vec3 up, TransformComponent *positionComponent);

    glm::mat4 getView(TransformComponent *component);
    glm::mat4 getProjection(float aspectRatio);

    void processKeyboard(CameraComponent::Direction direction, float deltaTime, TransformComponent *transformComponent);
    void processMouseMovement(float offsetX, float offsetY);

    Mode mode;
    Type type;
    float zoom;
    glm::vec3 target;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 worldUp;
    glm::vec3 front;

    float yaw, pitch, roll;
  
    float movementSpeed;
    float mouseSensitivity;

//private:
    void updateVectors();
};

#endif //CG1_PROJECT_CAMERACOMPONENT_H
