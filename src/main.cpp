#include <vld.h>
#include <cg/Application.h>

int main() {
    auto app = new Application("Computer Graphics: Project", 1200, 800);
    app->init();
    app->run();
    delete app;

    return EXIT_SUCCESS;
}
