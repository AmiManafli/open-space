#ifndef CG1_PROJECT_INPUTMANAGER_H
#define CG1_PROJECT_INPUTMANAGER_H

#include <GLHeader.h>
#include "GLContext.h"
#include <unordered_map>


class InputManager {
public:
    InputManager(GLContext *context);

    void init();
    void process(double deltaTime);

    bool isKeyDown(int key);
    bool isKeyUp(int key);
    bool isKeyPressed(int key);

private:
    GLContext *context;

    std::unordered_map<int, int> keysDown;

    bool processedMouse = false;
    double lastMouseX, lastMouseY;

    static void mousePositionCallback(GLFWwindow* window, double x, double y);
};


#endif //CG1_PROJECT_INPUTMANAGER_H
