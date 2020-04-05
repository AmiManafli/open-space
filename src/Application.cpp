#include <cg/GLContext.h>
#include <cg/entities/EntityBuilder.h>
#include "cg/Application.h"

Application::Application(std::string title, int width, int height) {
    entityManager = new EntityManager();
    context = new GLContext(entityManager, title, width, height);

    renderSystem = new RenderSystem(entityManager, context);
    inputSystem = new InputSystem(entityManager, context);

    int instanceRows = 20;
    int instanceCols = 20;

    int startRow = -instanceRows / 2;
    int startCol = -instanceCols / 2;

    double offset = 2.5;
    for (int r = startRow; r < instanceRows - 1; r++) {
        for (int c = startCol; c < instanceCols - 1; c++) {
            instanceTransformations.emplace_back(glm::vec3(offset * c, 15.0 * glm::sin(0.005 * r * c), offset * r));
        }
    }
}

Application::~Application() {
    delete inputSystem;
    delete renderSystem;

    delete highlightShaderProgram;
    delete gridShaderProgram;
    delete meshShaderProgram;
	delete meshTextureShaderProgram;

    delete context;
    delete entityManager;
}

void Application::init() {
    context->init();

    meshShaderProgram = new ShaderProgram();
    meshShaderProgram->attachShader("./assets/shaders/mesh.vert", ShaderType::VertexShader);
    meshShaderProgram->attachShader("./assets/shaders/mesh.frag", ShaderType::FragmentShader);
    meshShaderProgram->link();

	meshTextureShaderProgram = new ShaderProgram();
	meshTextureShaderProgram->attachShader("./assets/shaders/meshTexture.vert", ShaderType::VertexShader);
	meshTextureShaderProgram->attachShader("./assets/shaders/meshTexture.frag", ShaderType::FragmentShader);
	meshTextureShaderProgram->link();

    gridShaderProgram = new ShaderProgram();
    gridShaderProgram->attachShader("./assets/shaders/grid.vert", ShaderType::VertexShader);
    gridShaderProgram->attachShader("./assets/shaders/grid.frag", ShaderType::FragmentShader);
    gridShaderProgram->link();

    highlightShaderProgram = new ShaderProgram();
    highlightShaderProgram->attachShader("./assets/shaders/mesh.vert", ShaderType::VertexShader);
    highlightShaderProgram->attachShader("./assets/shaders/highlight.frag", ShaderType::FragmentShader);
    highlightShaderProgram->link();

    meshWithLightShaderProgram = new ShaderProgram();
    meshWithLightShaderProgram->attachShader("./assets/shaders/mesh.vert", ShaderType::VertexShader);
    meshWithLightShaderProgram->attachShader("./assets/shaders/meshWithLight.frag", ShaderType::FragmentShader);
    meshWithLightShaderProgram->link();

    meshTestLightShaderProgram = new ShaderProgram();
    meshTestLightShaderProgram->attachShader("./assets/shaders/mesh.vert", ShaderType::VertexShader);
    meshTestLightShaderProgram->attachShader("./assets/shaders/meshTestLight.frag", ShaderType::FragmentShader);
    meshTestLightShaderProgram->link();

    renderSystem->init();
    inputSystem->init();

    createCameras();
    createGrid(62, 62, false);

    auto sphere = EntityBuilder::create()
        ->withMesh("./assets/models/ico-sphere.dae", meshWithLightShaderProgram)
        ->withTransform(0, 0, 0)
        ->withScale(1.0)
        ->build(entityManager);

    auto light = EntityBuilder::create()
        ->withMesh("./assets/models/ico-sphere.dae", meshTestLightShaderProgram)
        ->withTransform(-5, 5, 0)
        ->withScale(0.2)
        ->build(entityManager);
}

void Application::run() {
    while (!context->shouldClose()) {
        context->update();

        // Process systems
        inputSystem->update();
        renderSystem->update();
    }
}

void Application::createCameras() {
    auto target = glm::vec3(0, 0, 0);

    /// Perspective camera
    auto position = glm::vec3(2.3, 7.3, 11.4);
    context->perspectiveCamera = EntityBuilder::create()
        ->withTransform(position)
        ->withCamera(CameraComponent::Mode::Free, CameraComponent::Type::Perspective, target, glm::normalize(-position), glm::vec3(0, 1, 0), context->getAspect())
        ->build(entityManager);

    /// Top camera
    position = glm::vec3(0, 5, 0);
    context->topCamera = EntityBuilder::create()
        ->withTransform(position)
        ->withCamera(CameraComponent::Mode::Free, CameraComponent::Type::Orthographic, target, glm::normalize(-position), glm::vec3(0, 0, -1), context->getAspect())
        ->build(entityManager);

    /// Side camera
    position = glm::vec3(0, 0, 5);
    context->sideCamera = EntityBuilder::create()
        ->withTransform(position)
        ->withCamera(CameraComponent::Mode::Free, CameraComponent::Type::Orthographic, target, glm::normalize(-position), glm::vec3(0, 1, 0), context->getAspect())
        ->build(entityManager);

    context->setActiveCamera(context->perspectiveCamera);
}

Entity* Application::createGrid(int width, int height, bool showYAxis) {
    int startX = width / 2;
    int startZ = height / 2;
    std::vector<MeshComponent::Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<MeshComponent::Texture> textures;
    uint32_t index = 0;
    for (int i = 0; i < width; i++) {
        float step = 1.0f;
        auto start = MeshComponent::Vertex { step * glm::vec3(-startX + i, 0, -startZ) };
        auto end = MeshComponent::Vertex { step * glm::vec3(-startX + i, 0, -startZ + height) };
        vertices.push_back(start);
        vertices.push_back(end);
        indices.push_back(index++);
        indices.push_back(index++);
    }
    for (int i = 0; i < height; i++) {
        float step = 1.0f;
        auto start = MeshComponent::Vertex { step * glm::vec3(-startX, 0, -startZ + i) };
        auto end = MeshComponent::Vertex { step * glm::vec3(-startX + width, 0, -startZ + i) };
        vertices.push_back(start);
        vertices.push_back(end);
        indices.push_back(index++);
        indices.push_back(index++);
    }

    if (showYAxis) {
        vertices.push_back(MeshComponent::Vertex { glm::vec3(0, 0, 0) });
        vertices.push_back(MeshComponent::Vertex { glm::vec3(0, 10, 0) });
        indices.push_back(index++);
        indices.push_back(index++);
    }

    std::vector<MeshComponent::Texture> test;

    return EntityBuilder::create()
        ->withTransform(0, 0, 0)
        ->withMesh(vertices, indices, textures, gridShaderProgram, GL_LINES)
        ->build(entityManager);
}
