#ifndef CG1_PROJECT_GLCONTEXT_H
#define CG1_PROJECT_GLCONTEXT_H

#include <GLHeader.h>
#include "shaders/Shader.h"
#include "Camera.h"

class GLContext {
public:
    GLContext(std::string title, uint16_t width, uint16_t height);
    ~GLContext();

    uint16_t getWidth() { return width; }
    uint16_t getHeight() { return height; }

    GLFWwindow* getWindow() { return window; }
    Camera* getCamera() { return camera; }

    void setCamera(Camera* camera) { this->camera = camera; }

    glm::mat4 getProjection();
    glm::mat4 getView() { return camera->getView(); }

    double getDeltaTime() { return deltaTime; }

    void init();

    bool shouldClose();
    void update();

    void swapBuffers();

private:
    const std::string title;
    uint16_t width;
    uint16_t height;

    GLFWwindow *window;

    Camera *camera;

    double lastTime;
    double deltaTime;

    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
};


#endif //CG1_PROJECT_GLCONTEXT_H
