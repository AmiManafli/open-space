#ifndef CG1_PROJECT_APPLICATION_H
#define CG1_PROJECT_APPLICATION_H

#include <cg/GLHeader.h>
#include <cg/entities/EntityManager.h>
#include <cg/entities/systems/RenderSystem.h>
#include <cg/entities/systems/InputSystem.h>
#include <cg/entities/systems/MovementSystem.h>
#include <cg/entities/systems/GravitySystem.h>
#include <cg/entities/systems/OrbitSystem.h>


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
	ShaderProgram* meshTextureShaderProgram;
    ShaderProgram *gridShaderProgram;
    ShaderProgram *highlightShaderProgram;
    ShaderProgram *meshWithLightShaderProgram;
    ShaderProgram *meshTestLightShaderProgram;
    ShaderProgram *skyboxShaderProgram;

    // Lights
    Entity *light;

    // Systems
    RenderSystem *renderSystem;
    InputSystem *inputSystem;
    MovementSystem *movementSystem;
    GravitySystem *gravitySystem;
    OrbitSystem *orbitSystem;

    std::vector<glm::vec3> instanceTransformations;

    void createCameras();
    Entity* createGrid(int width, int height, bool showYAxis);
};

#endif //CG1_PROJECT_APPLICATION_H
