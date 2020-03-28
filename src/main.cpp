#include <RenderWindow.h>
#include <RenderSystem.h>
#include <InputManager.h>

int main() {
    RenderWindow window("CG1: Project", 800, 600);
    RenderSystem renderer(window);
    InputManager inputManager(window);

    while (window.isOpen()) {
        inputManager.process();
        renderer.render();
    }

    return EXIT_SUCCESS;
}