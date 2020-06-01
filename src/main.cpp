#define ENABLE_OPENMP
#include <cg/Application.h>

int main() {
//    auto app = new Application("Computer Graphics: Project", 2880, 1800);
    auto app = new Application("Computer Graphics: Project", 1920, 1080);
    app->init();
    app->run();
    delete app;

    return EXIT_SUCCESS;
}
