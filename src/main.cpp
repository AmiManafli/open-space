#include <GLContext.h>
#include <RenderSystemOld.h>
#include <InputManager.h>
#include <entities/EntityManager.h>
#include <entities/systems/RenderSystem.h>

int main() {
    auto context = new GLContext("CG1: Project", 1200, 800);
    context->init();

    EntityManager *entityManager = new EntityManager();
    auto renderSystem = new RenderSystem(entityManager, context);
    renderSystem->init();

//    RenderSystemOld renderer(context);
//    renderer.setClearColor(0.2, 0.2, 0.2, 1.0);
//    renderer.init();

    InputManager inputManager(context);
    inputManager.init();

    while (!context->shouldClose()) {
        context->update();

        double deltaTime = context->getDeltaTime();

        inputManager.process(deltaTime);

        // Process systems
        renderSystem->update();
    }

    delete entityManager;
    delete context;

    return EXIT_SUCCESS;
}