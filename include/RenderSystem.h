#ifndef CG1_PROJECT_RENDERSYSTEM_H
#define CG1_PROJECT_RENDERSYSTEM_H

#include <GLHeader.h>
#include "RenderWindow.h"


class RenderSystem {
public:
    RenderSystem(RenderWindow &window);

    void render();

private:
    RenderWindow window;
};


#endif //CG1_PROJECT_RENDERSYSTEM_H
