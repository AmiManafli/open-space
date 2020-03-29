#ifndef CG1_PROJECT_GLCONTEXT_H
#define CG1_PROJECT_GLCONTEXT_H

#include <GLHeader.h>
#include "shaders/Shader.h"

class GLContext {
public:
    GLContext(std::string title, uint16_t width, uint16_t height);
    ~GLContext();

    uint16_t getWidth() { return width; }
    uint16_t getHeight() { return height; }

    void init();

    bool shouldClose();
    void pollEvents();

    void swapBuffers();

private:
    const std::string title;
    uint16_t width;
    uint16_t height;

    GLFWwindow *window;

    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
};


#endif //CG1_PROJECT_GLCONTEXT_H
