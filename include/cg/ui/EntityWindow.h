#ifndef CG1_PROJECT_ENTITYWINDOW_H
#define CG1_PROJECT_ENTITYWINDOW_H

#include <cg/GLContext.h>

class EntityWindow {
public:
    EntityWindow(EntityManager *entityManager, GLContext *context);

    void render();

private:
    EntityManager *entityManager;
    GLContext *context;

    uint16_t width;
    uint16_t height;

    void renderEmpty();
};


#endif //CG1_PROJECT_ENTITYWINDOW_H
