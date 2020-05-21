#ifndef CG1_PROJECT_GLCONTEXT_H
#define CG1_PROJECT_GLCONTEXT_H

#include <cg/GLHeader.h>
#include <cg/entities/Entity.h>
#include <cg/entities/EntityManager.h>
#include <cg/shaders/Shader.h>
#include "ViewFrustum.h"

class GLContext {
public:
    GLContext(EntityManager *entityManager, std::string title, uint16_t width, uint16_t height);
    ~GLContext();

    float getAspect() { return (float) width / (float) height; }
    uint16_t getWidth() { return width; }
    uint16_t getHeight() { return height; }

    GLFWwindow* getWindow() { return window; }

    Entity* getCamera() { return activeCamera; }
    void setActiveCamera(Entity *entity);

    glm::mat4 getProjection();
    glm::mat4 getView();

    double getDeltaTime() { return deltaTime; }
    double getFrameTime() { return timePerFrame; }
    double getTime() { return glfwGetTime(); }
    void init();

    bool shouldClose();
    void update();

    void swapBuffers();

    void updateViewFrustum() { if (viewFrustum) { viewFrustum->update(getAspect()); } }

    uint16_t width;
    uint16_t height;

    bool displayCursor = false;
    bool displayGui = false;
    bool bloomEnabled = false;

    bool showEntityNames = true;

    ImFont *uiRegularFont;
    ImFont *uiBlackFont;
    ImFont *uiEntityTitleFont;
    ImFont *uiEntityFont;

    ViewFrustum *viewFrustum;

    uint32_t triangleCount = 0;

    Entity *skybox = nullptr;
    Entity *grid = nullptr;

    Entity *player = nullptr;

    Entity *spaceshipCamera = nullptr;
    Entity *skyboxCamera = nullptr;
    Entity *perspectiveCamera = nullptr;
    Entity *topCamera = nullptr;
    Entity *sideCamera = nullptr;

    Entity *light = nullptr;

    Entity *selectedEntity = nullptr;

    // Shaders
    ShaderProgram *meshProgram;
    ShaderProgram *meshTextureProgram;
    ShaderProgram *starProgram;
    ShaderProgram *planetProgram;
    ShaderProgram *gridProgram;
    ShaderProgram *highlightProgram;
    ShaderProgram *meshWithLightProgram;
    ShaderProgram *meshTestLightProgram;
    ShaderProgram *skyboxProgram;
    ShaderProgram *bloomProgram;
    ShaderProgram *blurProgram;

    double startTime;

private:
    EntityManager *entityManager;
    const std::string title;

    GLFWwindow *window;

    Entity *activeCamera = nullptr;

    double lastTime;
    double lastFrameTime;
    double deltaTime;
    uint32_t frameCount;

    double timePerFrame;

    void setupImgui();
};


#endif //CG1_PROJECT_GLCONTEXT_H
