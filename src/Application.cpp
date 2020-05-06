#include <cg/GLContext.h>
#include <cg/entities/EntityBuilder.h>
#include <cg/terrain/Terrain.h>
#include <cg/terrain/OpenSimplexNoise.h>
#include <cg/terrain/PerlinNoise.h>
#include <cg/IcoSphere.h>
#include <cg/entities/components/OrbitComponent.h>
#include <cg/skybox/SkyboxStar.h>
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

    starTextureShaderProgram = new ShaderProgram();
    starTextureShaderProgram->attachShader("./assets/shaders/starTexture.vert", ShaderType::VertexShader);
    starTextureShaderProgram->attachShader("./assets/shaders/starTexture.frag", ShaderType::FragmentShader);
    starTextureShaderProgram->link();

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

    skyboxShaderProgram = new ShaderProgram();
    skyboxShaderProgram->attachShader("./assets/shaders/skybox.vert", ShaderType::VertexShader);
    skyboxShaderProgram->attachShader("./assets/shaders/skybox.frag", ShaderType::FragmentShader);
    skyboxShaderProgram->link();

    shaderProgram = new ShaderProgram();
    shaderProgram->attachShader("./assets/shaders/skyboxStar.vert", ShaderType::VertexShader);
    shaderProgram->attachShader("./assets/shaders/skyboxStar.frag", ShaderType::FragmentShader);
    shaderProgram->link();

    createCameras();
    gravitySystem = new GravitySystem(entityManager, context->perspectiveCamera);

    renderSystem->init();
    inputSystem->init();
    movementSystem->init();
    gravitySystem->init();
    orbitSystem->init();

    context->grid = createGrid(62, 62, false);

    auto ui = renderSystem->getUserInterface();

    auto color = glm::vec3(0.576, 0.886, 1.0);

    auto sunVelocity = new VelocityComponent();
    sunVelocity->rotation = glm::vec3(0, -0.2, 0);
	auto sun = EntityBuilder::create()
		->withMesh(new IcoSphere(1.0, 3, glm::vec3(0.96), 11, starTextureShaderProgram))
		->withTransform(0, 0, 0)
        ->withPointLight(glm::vec3(0.2), glm::vec3(1.0), glm::vec3(1.0), 1.0, 0.07, 0.017)
		->isSelectable()
		->withScale(1.0)
		->withMass(1000.0)
		->withVelocity(sunVelocity)
		->build(entityManager);
	auto sunTransform = entityManager->getComponent<TransformComponent>(sun);
    context->light = sun;

	auto planetScale = 0.4;
	auto planetVelocity = new VelocityComponent();
	planetVelocity->rotation = glm::vec3(0, -0.8, 0);
    auto planet1 = EntityBuilder::create()
        ->withMesh(new IcoSphere(1.0, 2, color, 11, meshTextureShaderProgram))
        ->withTransform(0, 0, 0)
        ->isSelectable()
        ->withMass(200)
        ->withScale(planetScale)
        ->withOrbit(sunTransform, 4.2, 4, 0.5, 0.0)
        ->withVelocity(planetVelocity)
        ->build(entityManager);

    auto planetTransform = entityManager->getComponent<TransformComponent>(planet1);
    auto moonScale = 0.1;
    auto moonVelocity = new VelocityComponent();
    moonVelocity->rotation = glm::vec3(0, -3.2, 0);
    auto moon1 = EntityBuilder::create()
            ->withMesh(new IcoSphere(1.0, 1, glm::vec3(1.0), 11, meshTextureShaderProgram))
            ->withTransform(0, 0, 0)
            ->isSelectable()
            ->withMass(200)
            ->withScale(moonScale)
            ->withOrbit(planetTransform, 1.1, 1.0, 1.4, 0.0)
            ->withVelocity(moonVelocity)
            ->build(entityManager);

    color = glm::vec3(0.886, 0.576, 1.0);
    planetScale = 0.6;
    planetVelocity = new VelocityComponent();
    planetVelocity->rotation = glm::vec3(0, -0.9, 0);
    auto planet2 = EntityBuilder::create()
            ->withMesh(new IcoSphere(1.0, 2, color, 11, meshTextureShaderProgram))
            ->withTransform(0, 0, 0)
            ->isSelectable()
            ->withMass(200)
            ->withScale(planetScale)
            ->withOrbit(sunTransform, 12, 12, 0.1, 1.0)
            ->withVelocity(planetVelocity)
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

    inputSystem->createSpaceshipControl(nullptr, context->spaceshipCamera);
}

void Application::run() {
    if (sky != nullptr) {
        auto skyboxEntityManager = new EntityManager();
        context->update();

        glDisable(GL_CULL_FACE);
        context->setActiveCamera(context->skyboxCamera);
        auto camera = entityManager->getComponent<CameraComponent>(context->skyboxCamera);
        sky->render(renderSystem, skyboxEntityManager, camera);

        // Cleanup
        delete skyboxEntityManager;
        context->setActiveCamera(context->spaceshipCamera);
        glViewport(0, 0, context->getWidth(), context->getHeight());
        glEnable(GL_CULL_FACE);
    }

    while (!context->shouldClose()) {
        context->update();

        // Update shader with light info
        int index = 0;
        std::vector<std::pair<LightComponent *, TransformComponent *>> lights;
        for (auto& pair : entityManager->getComponents<LightComponent>()) {
            // TODO: Take the MAX_LIGHTS closes to the camera
            if (index++ >= MAX_LIGHTS) {
                break;
            }
            auto entity = pair.first;
            auto light = dynamic_cast<LightComponent *>(pair.second);
            auto transform = entityManager->getComponent<TransformComponent>(entity);
            lights.emplace_back(std::make_pair(light, transform));
        }
        setLightUniforms(meshTextureShaderProgram, lights);

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
    auto position = glm::vec3(0, 3, 10);

    sky = new Skybox(10000, 100000, 20, "./assets/textures/skybox1", skyboxShaderProgram);
    context->spaceshipCamera = EntityBuilder::create()
        ->withTransform(position)
        ->withMesh(sky)
        ->withCamera(CameraComponent::Mode::FirstPersonShip, CameraComponent::Type::Perspective, target, glm::normalize(-position), glm::vec3(0, 1, 0), context->getAspect())
        ->build(entityManager);

    context->skyboxCamera = EntityBuilder::create()
            ->withTransform(0, 0, 0)
            ->withCamera(CameraComponent::Mode::CubeMap, CameraComponent::Type::CubeMapType, target, glm::normalize(-position), glm::vec3(0, 1, 0), 1)
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
        ->withMesh(vertices, indices, textures, gridShaderProgram, GL_LINES)
        ->build(entityManager);
}

void Application::setLightUniforms(ShaderProgram *shaderProgram, std::vector<std::pair<LightComponent *, TransformComponent *>> lights) {
    for (int i = 0; i < lights.size(); i++) {
        auto light = lights[i].first;
        auto transform = lights[i].second;
        std::string name = "lights[" + std::to_string(i) + "]";
        switch (light->type) {
            case LightComponent::Direction:
                shaderProgram->setUniform(name + ".type", light->type);
                shaderProgram->setUniform(name + ".direction", light->direction);
                shaderProgram->setUniform(name + ".ambient", light->ambient);
                shaderProgram->setUniform(name + ".diffuse", light->diffuse);
                shaderProgram->setUniform(name + ".specular", light->specular);
                break;
            case LightComponent::Point:
                shaderProgram->setUniform(name + ".type", light->type);
                shaderProgram->setUniform(name + ".position", transform->position);

                shaderProgram->setUniform(name + ".ambient", light->ambient);
                shaderProgram->setUniform(name + ".diffuse", light->diffuse);
                shaderProgram->setUniform(name + ".specular", light->specular);

                shaderProgram->setUniform(name + ".constant", light->constant);
                shaderProgram->setUniform(name + ".linear", light->linear);
                shaderProgram->setUniform(name + ".quadratic", light->quadratic);
                break;
            default:
                throw std::runtime_error("Unsupported light type");
        }
    }
}
