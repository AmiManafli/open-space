#include <cg/GLContext.h>
#include <cg/entities/EntityBuilder.h>
#include <cg/terrain/Terrain.h>
#include <cg/terrain/OpenSimplexNoise.h>
#include <cg/terrain/PerlinNoise.h>
#include <cg/IcoSphere.h>
#include <cg/entities/components/OrbitComponent.h>
#include "cg/Application.h"

bool onUpdateTerrain(Terrain *terrain, TerrainSettings& settings) {
    bool success = terrain->update(settings);

    return success;
}

Application::Application(std::string title, int width, int height) {
    entityManager = new EntityManager();
    context = new GLContext(entityManager, title, width, height);

    renderSystem = new RenderSystem(entityManager, context);
    inputSystem = new InputSystem(entityManager, context);
    movementSystem = new MovementSystem(entityManager, context);
    orbitSystem = new OrbitSystem(entityManager, context);
}

Application::~Application() {
    delete inputSystem;
    delete renderSystem;
    delete movementSystem;

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

    context->highlightProgram = highlightShaderProgram;

    meshWithLightShaderProgram = new ShaderProgram();
    meshWithLightShaderProgram->attachShader("./assets/shaders/meshWithLight.vert", ShaderType::VertexShader);
    meshWithLightShaderProgram->attachShader("./assets/shaders/meshWithLight.frag", ShaderType::FragmentShader);
    meshWithLightShaderProgram->link();

    meshTestLightShaderProgram = new ShaderProgram();
    meshTestLightShaderProgram->attachShader("./assets/shaders/mesh.vert", ShaderType::VertexShader);
    meshTestLightShaderProgram->attachShader("./assets/shaders/meshTestLight.frag", ShaderType::FragmentShader);
    meshTestLightShaderProgram->link();

    createCameras();
    gravitySystem = new GravitySystem(entityManager, context->perspectiveCamera);

    renderSystem->init();
    inputSystem->init();
    movementSystem->init();
    gravitySystem->init();
    orbitSystem->init();

    auto lightPosition = glm::vec3(0, 20, 20);
    light = EntityBuilder::create()
        ->withTransform(lightPosition)
        ->withDirectionalLight(-lightPosition, {0.2f, 0.2f, 0.2f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f})
        ->build(entityManager);
    context->light = light;

    createGrid(62, 62, false);

    auto ui = renderSystem->getUserInterface();

    auto color = glm::vec3(0.576, 0.886, 1.0);

    auto sunVelocity = new VelocityComponent();
    sunVelocity->rotation = glm::vec3(0, -0.2, 0);
	auto sun = EntityBuilder::create()
		->withMesh(new IcoSphere(1.0, 3, glm::vec3(0.96), 11, meshTextureShaderProgram))
		->withTransform(0, 0, 0)
		->withScale(20.0)
		->withMass(1000.0)
		->withVelocity(sunVelocity)
		->build(entityManager);
	auto sunTransform = entityManager->getTransformComponent(sun);

	auto planetScale = 2.0;
	auto planetVelocity = new VelocityComponent();
	planetVelocity->rotation = glm::vec3(0, -0.1, 0);
    auto planet1 = EntityBuilder::create()
        ->withMesh(new IcoSphere(1.0, 3, color, 11, meshTextureShaderProgram))
        ->withTransform(0, 0, 0)
        ->withMass(200)
        ->withScale(planetScale)
        ->withOrbit(sunTransform, 40, 40, 0.1, 0.0)
        ->withVelocity(planetVelocity)
        ->build(entityManager);

    auto planetTransform = entityManager->getTransformComponent(planet1);
    auto moonScale = 0.8;
    auto moonVelocity = new VelocityComponent();
    moonVelocity->rotation = glm::vec3(0, -0.1, 0);
    auto moon1 = EntityBuilder::create()
            ->withMesh(new IcoSphere(1.0, 3, glm::vec3(0.6), 11, meshTextureShaderProgram))
            ->withTransform(0, 0, 0)
            ->withMass(200)
            ->withScale(moonScale)
            ->withOrbit(planetTransform, 8, 8, 1.5, 0.0)
            ->withVelocity(moonVelocity)
            ->build(entityManager);

//    auto terrainMesh = Terrain::generate(10, 10, meshWithLightShaderProgram, GL_TRIANGLES, NoiseType::OpenSimplex);
//    terrainMesh->setupBuffers();
//
//    ui->onUpdateTerrain(terrainMesh, onUpdateTerrain);
//
//    auto terrain = EntityBuilder::create()
//        ->withMesh(terrainMesh)
//        ->withTransform(0, 1.01, 0)
//        ->build(entityManager);

//	auto airplane = EntityBuilder::create()
//		->withMesh("./assets/models/airplaneUdemy.obj", meshTextureShaderProgram)
//		->withTransform(0, 0, 0)
//		->withOrbit(planetTransform, 12, 12, 1.9, 2.0)
//		->build(entityManager);

//    inputSystem->createSpaceshipControl(airplane, context->spaceshipCamera);
    inputSystem->createSpaceshipControl(nullptr, context->spaceshipCamera);

//    auto nanoSuit = EntityBuilder::create()
//        ->withMesh("./assets/models/nanosuit.obj", meshTextureShaderProgram)
//        ->withTransform(0, 0, 0)
//        ->withScale(0.3)
//		->withVelocity(new VelocityComponent(glm::vec3(0, 0.5, 0), glm::vec3(0, 0, 0)))
//        ->build(entityManager);

  //  auto nanoSuit = EntityBuilder::create()
  //      ->withMesh("./assets/models/nanosuit.obj", meshTextureShaderProgram)
  //      ->withTransform(0, 0, 0)
  //      ->withScale(0.3)
		//->withVelocity(new VelocityComponent(glm::vec3(0, 0.5, 0), glm::vec3(0, 0, 0)))
  //      ->build(entityManager);

    //auto floor = EntityBuilder::create()
    //    ->withMesh("./assets/models/plane.dae", meshWithLightShaderProgram)
    //    ->withTransform(0, 0, 0)
    //    ->withScale(30.0)
    //    ->build(entityManager);
}

void Application::run() {
    while (!context->shouldClose()) {
        context->update();

        // Update shader with light info
        auto lightComponent = entityManager->getLightComponent(light->id);
        auto lightTransform = entityManager->getTransformComponent(light->id);
        lightComponent->setUniforms(meshTextureShaderProgram, lightTransform);

        // Process systems
        gravitySystem->update();
        orbitSystem->update();
        inputSystem->update();
        movementSystem->update();
        renderSystem->update();
    }
}

void Application::createCameras() {
    auto target = glm::vec3(0, 0, 0);

    /// Spaceship camera
    auto position = glm::vec3(2.3, 80.3, 60.0);

    context->spaceshipCamera = EntityBuilder::create()
        ->withTransform(position)
        ->withCamera(CameraComponent::Mode::FirstPersonShip, CameraComponent::Type::Perspective, target, glm::normalize(-position), glm::vec3(0, 1, 0), context->getAspect())
        ->build(entityManager);

    /// Perspective camera
    position = glm::vec3(2.3, 40.0, 80.0);
    context->perspectiveCamera = EntityBuilder::create()
        ->withTransform(position)
        ->withVelocity(new VelocityComponent())
        ->withMass(1.0)
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

    context->setActiveCamera(context->spaceshipCamera);
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

    return EntityBuilder::create()
        ->withTransform(0, 0, 0)
        ->withMesh(vertices, indices, textures, gridShaderProgram, GL_LINES)
        ->build(entityManager);
}
