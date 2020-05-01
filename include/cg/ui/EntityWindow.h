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

    std::vector<const char *> meshModes;

    void renderEmpty();

    void renderInfo(Entity *entity);

    // Components
    void renderTransformComponent(TransformComponent *component);
    void renderMassComponent(MassComponent *component);
    void renderOrbitComponent(OrbitComponent *component);
    void renderVelocityComponent(VelocityComponent *component);
    void renderCameraComponent(CameraComponent *component);
    void renderMeshComponents(std::vector<MeshComponent *> components);
    void renderHighlightComponent(HighlightComponent *component);
};


#endif //CG1_PROJECT_ENTITYWINDOW_H
