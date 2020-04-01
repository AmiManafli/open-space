#ifndef CG1_PROJECT_CAMERA_H
#define CG1_PROJECT_CAMERA_H

#include <GLHeader.h>
#include <meshes/Model.h>

class Camera {
public:
    enum Mode {
        Orthographic,
        Perspective,
    };
    enum TargetingMode {
        Free,
        Target,
        Follow
    };
    enum Direction {
        Forward,
        Backward,
        Left,
        Right,
    };

public:
    Camera(Mode mode, glm::vec3 position);
    Camera(Mode mode, glm::vec3 position, glm::vec3 up, glm::vec3 front);

    void setTarget(glm::vec3 target) { this->target = target; }

    std::vector<Mesh> meshes;

    glm::mat4 getView();
    glm::mat4 getProjection(float aspectRatio);

    void draw(ShaderProgram& shaderProgram);

    void processKeyboard(Direction direction, float deltaTime);
    void processMouseMovement(float offsetX, float offsetY);

private:
    Mode mode;
    TargetingMode targetingMode;
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 worldUp;
    glm::vec3 right;
    glm::vec3 target;
    float movementSpeed;
    float mouseSensitivity;
    float yaw;
    float pitch;
    float roll;

    glm::mat4 view;

    void update();
};


#endif //CG1_PROJECT_CAMERA_H
