#ifndef CG1_PROJECT_USERINTERFACE_H
#define CG1_PROJECT_USERINTERFACE_H

#include <cg/GLHeader.h>
#include <cg/GLContext.h>

class UserInterface {
public:
    explicit UserInterface(EntityManager *entityManager, GLContext *context);

    void render();

private:
    EntityManager *entityManager;
    GLContext *context;

    bool showDemoWindow = false;

    char* currentView = nullptr;
    std::vector<const char *> views;

    ImVec2 cameraWindowSize;

    void renderMainMenu();
    void renderSceneInfoWindow();
    void renderCameraInfoWindow();
};

#endif //CG1_PROJECT_USERINTERFACE_H
