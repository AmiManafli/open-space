#ifndef CG1_PROJECT_PLANETEDITOR_H
#define CG1_PROJECT_PLANETEDITOR_H

#include <cg/GLHeader.h>
#include <cg/entities/EntityManager.h>
#include <cg/GLContext.h>
#include <cg/PlanetGenerator.h>


class PlanetEditor {
public:
    PlanetEditor(EntityManager &entityManager, GLContext &context);

    void render();

private:
    EntityManager &entityManager;
    GLContext &context;

    PlanetSettings settings;

    void updatePlanet(Entity *entity);
};

#endif //CG1_PROJECT_PLANETEDITOR_H
