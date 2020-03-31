#ifndef CG1_PROJECT_CAMERA_H
#define CG1_PROJECT_CAMERA_H

#include <GLHeader.h>
#include <meshes/Model.h>


class Camera {
public:
    enum Mode {
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

    void loadMesh();

    glm::mat4 getView();

    void draw(ShaderProgram& shaderProgram);

    void processKeyboard(Direction direction, float deltaTime);
    void processMouseMovement(float offsetX, float offsetY);

private:
    Mode mode;
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 worldUp;
    glm::vec3 right;
    float movementSpeed;
    float mouseSensitivity;
    float yaw;
    float pitch;
    float roll;
    std::vector<Mesh> meshes;

    void update();
};


#endif //CG1_PROJECT_CAMERA_H
