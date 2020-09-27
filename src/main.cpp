#define ENABLE_OPENMP
#include <cg/Application.h>
#include <boost/di/extension/policies/uml_dumper.hpp>

int main() {
//    auto injector = di::make_injector<di::extension::uml_dumper>(
    auto injector = di::make_injector(
            di::bind<GLContext>.in(di::singleton)
    );
    auto app = injector.create<Application>();
    app.init("Space Simulator", 1200, 800);
    app.run();

    return EXIT_SUCCESS;
}
