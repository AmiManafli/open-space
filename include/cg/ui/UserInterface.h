#ifndef CG1_PROJECT_USERINTERFACE_H
#define CG1_PROJECT_USERINTERFACE_H

#include <cg/GLHeader.h>
#include <cg/GLContext.h>

class UserInterface {
public:
    explicit UserInterface(GLContext *context);

    void render();

private:
    GLContext *context;

    bool showDemoWindow = false;

    char* currentView = nullptr;
    std::vector<const char *> views;

    void renderMainMenu();
    void renderSceneInfoWindow();
    void renderCameraInfoWindow();
};

#endif //CG1_PROJECT_USERINTERFACE_H
