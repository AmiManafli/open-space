#ifndef CG1_PROJECT_GLCONTEXT_H
#define CG1_PROJECT_GLCONTEXT_H

#include <GLHeader.h>
#include "shaders/Shader.h"
#include "Camera.h"

class GLContext {
public:
    GLContext(std::string title, uint16_t width, uint16_t height);
    ~GLContext();

    float getAspect() { return (float) width / (float) height; }
    uint16_t getWidth() { return width; }
    uint16_t getHeight() { return height; }

    GLFWwindow* getWindow() { return window; }

    bool showGrid() { return displayGrid; }
    void setShowGrid(bool display) { displayGrid = display; }

    Camera* getCamera() { return cameras[activeCamera]; }
    std::vector<Camera *> getCameras() { return cameras; }
    uint16_t getActiveCamera() { return activeCamera; }
    void setActiveCamera(uint16_t cameraIndex) { activeCamera = cameraIndex; }
    void addCamera(Camera *camera) { cameras.push_back(camera); }

    glm::mat4 getProjection();
    glm::mat4 getView() { return getCamera()->getView(); }

    double getDeltaTime() { return deltaTime; }

    void init();

    bool shouldClose();
    void update();

    void swapBuffers();

private:
    const std::string title;
    uint16_t width;
    uint16_t height;

    bool displayGrid = false;

    GLFWwindow *window;

    uint16_t activeCamera = 0;
    std::vector<Camera *> cameras;

    double lastTime;
    double deltaTime;

    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
};


#endif //CG1_PROJECT_GLCONTEXT_H
