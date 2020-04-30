#ifndef CG1_PROJECT_INPUTSYSTEM_H
#define CG1_PROJECT_INPUTSYSTEM_H

#include <cg/GLHeader.h>
#include <cg/entities/System.h>
#include "../../../../SpaceshipControl.h"


class InputSystem : public System {
public:
    InputSystem(EntityManager *entityManager, GLContext *context);

    void init() override;
    void update() override;
    void createSpaceshipControl(Entity* spaceship, Entity* camera);


    bool isKeyDown(int key);
    bool isKeyUp(int key);
    bool isKeyPressed(int key);

private:
    GLContext *context;

    SpaceshipControl* spaceshipControl;
    bool processedMouse = false;
    double lastMouseX, lastMouseY;
    std::unordered_map<uint32_t, uint32_t> keysDown;

    void moveCamera(Entity *camera, CameraComponent::Direction direction, float deltaTime);
    static void mousePositionCallback(GLFWwindow* window, double x, double y);
    static void processMouseScroll(GLFWwindow* window, double xoffset, double yoffset);

};

#endif //CG1_PROJECT_INPUTSYSTEM_H
