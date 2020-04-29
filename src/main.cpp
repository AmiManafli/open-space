#include <cg/Application.h>

int main() {
    auto app = new Application("Computer Graphics: Project", 1000, 800);
    app->init();
    app->run();

    return EXIT_SUCCESS;
}
