#ifndef CG1_PROJECT_GLCONTEXT_H
#define CG1_PROJECT_GLCONTEXT_H

#include <GLHeader.h>
#include "Shader.h"

class GLContext {
public:
    GLContext(std::string title, uint16_t width, uint16_t height);
    ~GLContext();

    void init();

    bool shouldClose();
    void pollEvents();

    void linkProgram();
    void swapBuffers();

    void attach(Shader *shader);

private:
    const std::string title;
    uint16_t width;
    uint16_t height;

    unsigned int program;
    GLFWwindow *window;

    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
};


#endif //CG1_PROJECT_GLCONTEXT_H
