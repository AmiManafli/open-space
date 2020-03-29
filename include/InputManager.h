#ifndef CG1_PROJECT_INPUTMANAGER_H
#define CG1_PROJECT_INPUTMANAGER_H

#include <GLHeader.h>
#include "GLContext.h"


class InputManager {
public:
    InputManager(GLContext *context);

    void init();
    void process(double deltaTime);

private:
    GLContext *context;

    double lastMouseX, lastMouseY;

    static void mousePositionCallback(GLFWwindow* window, double x, double y);
};


#endif //CG1_PROJECT_INPUTMANAGER_H
