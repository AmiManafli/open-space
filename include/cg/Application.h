#ifndef CG1_PROJECT_APPLICATION_H
#define CG1_PROJECT_APPLICATION_H

#include <cg/GLHeader.h>
#include <cg/entities/EntityManager.h>
#include <cg/entities/systems/RenderSystem.h>
#include <cg/entities/systems/InputSystem.h>


class Application {
public:
    Application(std::string title, int width, int height);
    ~Application();

    void init();
    void run();

private:
    EntityManager *entityManager;
    GLContext *context;

    // Shaders
    ShaderProgram *meshShaderProgram;
    ShaderProgram *gridShaderProgram;
    ShaderProgram *highlightShaderProgram;

    // Systems
    RenderSystem *renderSystem;
    InputSystem *inputSystem;

    Entity* createModel(std::string filename, glm::vec3 position, ShaderProgram *shaderProgram, bool highlight);
    void createCameras();
};

#endif //CG1_PROJECT_APPLICATION_H
