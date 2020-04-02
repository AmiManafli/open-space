#ifndef CG1_PROJECT_CAMERACOMPONENT_H
#define CG1_PROJECT_CAMERACOMPONENT_H

#include <GLHeader.h>
#include <entities/Component.h>
#include "PositionComponent.h"


class CameraComponent : public Component {
public:
    enum Mode {
        Free,
    };
    enum Type {
        Orthographic,
        Perspective,
    };
    enum Direction {
        Forward,
        Backward,
        Left,
        Right,
    };

    CameraComponent(Mode mode, Type type, glm::vec3 target, glm::vec3 front, glm::vec3 up, float aspectRatio, PositionComponent *positionComponent);

    glm::mat4 getView(PositionComponent *component);
    glm::mat4 getProjection(float aspectRatio);

//    void processKeyboard(Direction direction, float deltaTime);
//    void processMouseMovement(float offsetX, float offsetY);

private:
    Mode mode;
    Type type;
    float zoom;
    glm::vec3 target;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 worldUp;
    glm::vec3 front;

    float yaw, pitch, roll;
    float aspectRatio;

    float movementSpeed;
};

#endif //CG1_PROJECT_CAMERACOMPONENT_H
