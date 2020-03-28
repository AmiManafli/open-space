#ifndef CG1_PROJECT_INPUTMANAGER_H
#define CG1_PROJECT_INPUTMANAGER_H

#include <GLHeader.h>
#include "RenderWindow.h"


class InputManager {
public:
    InputManager(RenderWindow &window);

    void process();

private:
    RenderWindow window;
};


#endif //CG1_PROJECT_INPUTMANAGER_H
