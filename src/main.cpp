#include <GLContext.h>
#include <InputManager.h>
#include <entities/EntityManager.h>
#include <entities/systems/RenderSystem.h>

/**
 * Create example sphere.
 * @param entityManager entity manager instance.
 * @param shaderProgram shader program to use for this entity.
 */
void createSphere(EntityManager *entityManager, ShaderProgram *shaderProgram) {
    auto sphere = entityManager->createEntity();
    auto spherePosition = new PositionComponent { glm::vec3(2, 0, 0) };
    auto sphereMeshes = MeshComponent::createMeshComponentsFromFile("../assets/models/ico-sphere.dae", shaderProgram);

    entityManager->addPositionComponent(sphere->id, spherePosition);
    for (auto& meshComponent : sphereMeshes) {
        entityManager->addMeshComponent(sphere->id, meshComponent);
    }
}

int main() {
    auto context = new GLContext("Computer Graphics: Project", 1200, 800);
    context->init();

    auto shaderProgram = new ShaderProgram();
    shaderProgram->attachShader("../assets/shaders/test.vert", ShaderType::VertexShader);
    shaderProgram->attachShader("../assets/shaders/test.frag", ShaderType::FragmentShader);
    shaderProgram->link();

    auto entityManager = new EntityManager();

    createSphere(entityManager, shaderProgram);

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