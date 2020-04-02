#include <GLContext.h>
#include <RenderSystemOld.h>
#include <InputManager.h>
#include <entities/EntityManager.h>
#include <entities/systems/RenderSystem.h>

/**
 * Create example entities.
 */
void createEntities(EntityManager *entityManager) {
    auto sphere = entityManager->createEntity();
    auto spherePosition = new PositionComponent { glm::vec3(0, 0, 0) };
    auto sphereMeshes = createMeshComponentsFromFile("../assets/models/ico-sphere.dae");

    entityManager->addPositionComponent(sphere->id, spherePosition);
    for (auto& meshComponent : sphereMeshes) {
        entityManager->addMeshComponent(sphere->id, meshComponent);
    }
}

int main() {
    auto context = new GLContext("CG1: Project", 1200, 800);
    context->init();

    auto entityManager = new EntityManager();

    createEntities(entityManager);

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