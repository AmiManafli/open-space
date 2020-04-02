#include <GLContext.h>
#include <InputManager.h>
#include <entities/EntityManager.h>
#include <entities/systems/RenderSystem.h>
#include <entities/EntityBuilder.h>

/**
 * Create example sphere.
 * @param entityBuilder entity builder.
 * @param shaderProgram shader program to use for this entity.
 */
Entity* createSphere(EntityBuilder *builder, ShaderProgram *shaderProgram) {
    return builder
        ->withMesh("../assets/models/ico-sphere.dae", shaderProgram)
        ->withPosition(0, 0, 0)
        ->build();
}

int main() {
    auto context = new GLContext("Computer Graphics: Project", 1200, 800);
    context->init();

    auto shaderProgram = new ShaderProgram();
    shaderProgram->attachShader("../assets/shaders/test.vert", ShaderType::VertexShader);
    shaderProgram->attachShader("../assets/shaders/test.frag", ShaderType::FragmentShader);
    shaderProgram->link();

    auto entityManager = new EntityManager();

    auto sphere = createSphere( new EntityBuilder(entityManager), shaderProgram);

    auto renderSystem = new RenderSystem(entityManager, context);
    renderSystem->init();

    InputManager inputManager(context);
    inputManager.init();

    while (!context->shouldClose()) {
        context->update();

        double deltaTime = context->getDeltaTime();

        inputManager.process(deltaTime);

        // Process systems
        renderSystem->update();
    }

    delete renderSystem;
    delete entityManager;
    delete context;

    return EXIT_SUCCESS;
}