#include <cg/Application.h>

int main() {
#ifdef ENABLE_OPENMP
    printf("Hello!\n");
#endif

    auto app = new Application("Computer Graphics: Project", 1200, 800);
    app->init();
    app->run();
    delete app;

    return EXIT_SUCCESS;
}
