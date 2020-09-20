#define ENABLE_OPENMP
#include <cg/Application.h>

int main() {
    auto injector = di::make_injector(
            di::bind<GLContext>.in(di::singleton)
    );
    auto app = injector.create<Application>();
    app.init("Space Simulator", 1200, 800);
    app.run();

    return EXIT_SUCCESS;
}
