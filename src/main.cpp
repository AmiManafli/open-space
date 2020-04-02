#include <GLContext.h>
#include <entities/EntityManager.h>
#include <entities/systems/RenderSystem.h>
#include <entities/EntityBuilder.h>
#include <entities/systems/InputSystem.h>

Entity *createPerspectiveCamera(EntityBuilder *pBuilder, float aspect);

/**
 * Create example sphere.
 * @param entityBuilder entity builder.
 * @param shaderProgram shader program to use for this entity.
 */
Entity* createSphere(EntityBuilder *entityBuilder, ShaderProgram *shaderProgram) {
    return entityBuilder
        ->withMesh("../assets/models/ico-sphere.dae", shaderProgram)
        ->withPosition(0, 0, 0)
        ->build();
}

//void setupCamera(GLContext *context) {
//    auto topCamera = new Camera(Camera::Mode::Orthographic, glm::vec3(0, 5, 0), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0));
//    auto sideCamera = new Camera(Camera::Mode::Orthographic, glm::vec3(0, 0, 5), glm::vec3(0, 1, 0), glm::vec3(0, 0, -1));
//    auto perspectiveCamera = new Camera(Camera::Mode::Perspective, glm::vec3(5, 5, 10), glm::vec3(0, 1, 0), glm::normalize(glm::vec3(-5, -5, -10)));
//    perspectiveCamera->setTarget(glm::vec3(0, 0, 0));
//    auto camera = new Camera(Camera::Mode::Perspective, glm::vec3(5, 5, 10), glm::vec3(0, 1, 0), glm::normalize(glm::vec3(-5, -5, -10)));
//    camera->setTarget(glm::vec3(0, 0, 0));
//
//    auto cameraMeshes = Model::loadModel("../assets/models/camera.dae");
//    camera->meshes = cameraMeshes;
//
//    context->addCamera(camera);
//    context->setActiveCamera(0);
//}

Entity *createPerspectiveCamera(EntityBuilder *entityBuilder, float aspectRatio) {
    auto position = glm::vec3(5, 10, 5);
    return entityBuilder
        ->withPosition(position)
        ->withCamera(CameraComponent::Mode::Free, CameraComponent::Type::Perspective, glm::vec3(0, 0, 0), glm::normalize(-position), glm::vec3(0, 1, 0), aspectRatio)
        ->build();
}

int main() {
    auto entityManager = new EntityManager();

    auto context = new GLContext(entityManager, "Computer Graphics: Project", 1200, 800);
    context->init();

    auto shaderProgram = new ShaderProgram();
    shaderProgram->attachShader("../assets/shaders/test.vert", ShaderType::VertexShader);
    shaderProgram->attachShader("../assets/shaders/test.frag", ShaderType::FragmentShader);
    shaderProgram->link();

    auto sphere = createSphere( new EntityBuilder(entityManager), shaderProgram);
    auto perspectiveCamera = createPerspectiveCamera(new EntityBuilder(entityManager), context->getAspect());

    context->addCamera(perspectiveCamera);
    context->setActiveCamera(0);

    auto renderSystem = new RenderSystem(entityManager, context);
    renderSystem->init();

    auto inputSystem = new InputSystem(entityManager, context);
    inputSystem->init();

    while (!context->shouldClose()) {
        context->update();

        // Process systems
        inputSystem->update();
        renderSystem->update();
    }

    delete renderSystem;
    delete entityManager;
    delete context;

    return EXIT_SUCCESS;
}
