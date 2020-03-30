#ifndef CG1_PROJECT_CAMERA_H
#define CG1_PROJECT_CAMERA_H

#include <GLHeader.h>


class Camera {
public:
    enum Mode {
        Free,
        Target,
        Follow
    };

public:
    Camera(Mode mode, glm::vec3 position);

    glm::mat4& getView() { return view; }

    void forward(float amount);
    void backward(float amount);
    void strafeLeft(float amount);
    void strafeRight(float amount);

    void rotate(float yaw, float pitch, float roll);

private:
    Mode mode;
    float yawAngle;
    float pitchAngle;
    float rollAngle;
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 direction;
    glm::vec3 up;
    glm::vec3 right;

    glm::mat4 view;

    void updateView();
};


#endif //CG1_PROJECT_CAMERA_H
