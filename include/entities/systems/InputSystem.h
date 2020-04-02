#ifndef CG1_PROJECT_INPUTSYSTEM_H
#define CG1_PROJECT_INPUTSYSTEM_H

#include <GLHeader.h>
#include <entities/System.h>


class InputSystem : public System {
public:
    InputSystem(EntityManager *entityManager, GLContext *context);

    void init() override;
    void update() override;

    bool isKeyDown(int key);
    bool isKeyUp(int key);
    bool isKeyPressed(int key);

private:
    GLContext *context;

    bool processedMouse = false;
    double lastMouseX, lastMouseY;
    std::unordered_map<uint32_t, uint32_t> keysDown;

    static void mousePositionCallback(GLFWwindow* window, double x, double y);
};

#endif //CG1_PROJECT_INPUTSYSTEM_H
