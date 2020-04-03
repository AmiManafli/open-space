#ifndef CG1_PROJECT_GLCONTEXT_H
#define CG1_PROJECT_GLCONTEXT_H

#include <GLHeader.h>
#include <entities/Entity.h>
#include <entities/EntityManager.h>
#include "shaders/Shader.h"

class GLContext {
public:
    GLContext(EntityManager *entityManager, std::string title, uint16_t width, uint16_t height);
    ~GLContext();

    float getAspect() { return (float) width / (float) height; }
    uint16_t getWidth() { return width; }
    uint16_t getHeight() { return height; }

    GLFWwindow* getWindow() { return window; }

    Entity* getCamera() { return cameras[activeCameraIndex]; }
    std::vector<Entity *> getCameras() { return cameras; }
    uint16_t getActiveCamera() { return activeCameraIndex; }
    void setActiveCamera(uint16_t cameraIndex) { activeCameraIndex = cameraIndex; }
    void addCamera(Entity *camera) { cameras.push_back(camera); }

    glm::mat4 getProjection();
    glm::mat4 getView();

    double getDeltaTime() { return deltaTime; }

    void init();

    bool shouldClose();
    void update();

    void swapBuffers();

    bool debug = false;
    bool displayGui = false;
    bool displayGrid = true;

private:
    EntityManager *entityManager;
    const std::string title;
    uint16_t width;
    uint16_t height;

    GLFWwindow *window;

    uint16_t activeCameraIndex = 0;
    std::vector<Entity *> cameras;

    double lastTime;
    double deltaTime;

    void setupImgui();

    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
};


#endif //CG1_PROJECT_GLCONTEXT_H
