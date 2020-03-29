#include "GLContext.h"

GLContext::GLContext(std::string title, uint16_t width, uint16_t height)
        : title(title), width(width), height(height) {
}

GLContext::~GLContext() {
    glfwTerminate();
}

void GLContext::init() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("failed to create window");
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        throw std::runtime_error("failed to initialize GLAD");
    }

    program = glCreateProgram();
}

bool GLContext::shouldClose() {
    return glfwWindowShouldClose(window);
}

void GLContext::pollEvents() {
    glfwPollEvents();
}

void GLContext::linkProgram() {
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        throw std::runtime_error(infoLog);
    }
}

void GLContext::swapBuffers() {
    glfwSwapBuffers(window);
}

void GLContext::attach(Shader *shader) {
    glAttachShader(program, shader->get());
}

void GLContext::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
