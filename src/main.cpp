#include <GLContext.h>
#include <RenderSystem.h>
#include <InputManager.h>

int main() {
    auto context = new GLContext("CG1: Project", 800, 600);
    context->init();

    RenderSystem renderer(context);
    renderer.setClearColor(0.2, 0.2, 0.2, 1.0);
    renderer.init();

    InputManager inputManager(context);
    inputManager.init();

    while (!context->shouldClose()) {
        context->pollEvents();
        inputManager.process();

        renderer.render();
    }

    delete context;

    return EXIT_SUCCESS;
}