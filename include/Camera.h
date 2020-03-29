#ifndef CG1_PROJECT_CAMERA_H
#define CG1_PROJECT_CAMERA_H

#include <GLHeader.h>


class Camera {
public:
    Camera(glm::vec3 position, glm::vec3 target);

    glm::mat4& getView() { return view; }

    void pitch(float angle);
    void yaw(float angle);

private:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    glm::vec3 right;

    glm::mat4 view;

    void updateView();
};


#endif //CG1_PROJECT_CAMERA_H
