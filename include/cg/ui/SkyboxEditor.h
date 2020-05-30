#ifndef CG1_PROJECT_SKYBOXEDITOR_H
#define CG1_PROJECT_SKYBOXEDITOR_H

#include <cg/GLHeader.h>
#include <cg/entities/EntityManager.h>
#include <cg/GLContext.h>
#include <cg/skybox/Skybox.h>


class SkyboxEditor {
public:
    SkyboxEditor(EntityManager &entityManager, GLContext &context);

    void render();

private:
    EntityManager &entityManager;
    GLContext &context;

//    SkyboxSettings settings;
};


#endif //CG1_PROJECT_SKYBOXEDITOR_H
