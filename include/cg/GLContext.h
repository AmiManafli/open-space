#ifndef CG1_PROJECT_GLCONTEXT_H
#define CG1_PROJECT_GLCONTEXT_H

#include <cg/GLHeader.h>
#include <cg/entities/Entity.h>
#include <cg/entities/EntityManager.h>
#include <cg/shaders/Shader.h>

class GLContext {
public:
    GLContext(EntityManager *entityManager, std::string title, uint16_t width, uint16_t height);
    ~GLContext();

    float getAspect() { return (float) width / (float) height; }
    uint16_t getWidth() { return width; }
    uint16_t getHeight() { return height; }

    GLFWwindow* getWindow() { return window; }

    Entity* getCamera() { return activeCamera; }
    void setActiveCamera(Entity *entity) { activeCamera = entity; }

    glm::mat4 getProjection();
    glm::mat4 getView();

    double getDeltaTime() { return deltaTime; }
    double getFrameTime() { return timePerFrame; }

    void init();

    bool shouldClose();
    void update();

    void swapBuffers();

    bool displayCursor = false;
    bool displayGui = false;
    bool displayGrid = true;

    uint32_t triangleCount = 0;

    Entity *spaceshipCamera = nullptr;
    Entity *perspectiveCamera = nullptr;
    Entity *topCamera = nullptr;
    Entity *sideCamera = nullptr;

    Entity *light = nullptr;

    Entity *selectedEntity = nullptr;

    ShaderProgram *highlightProgram = nullptr;

    double startTime;

private:
    EntityManager *entityManager;
    const std::string title;
    uint16_t width;
    uint16_t height;

    GLFWwindow *window;

    Entity *activeCamera = nullptr;

    double lastTime;
    double lastFrameTime;
    double deltaTime;
    uint32_t frameCount;

    double timePerFrame;

    void setupImgui();

    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
};


#endif //CG1_PROJECT_GLCONTEXT_H
