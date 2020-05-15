#include <cg/GLContext.h>
#include <cg/entities/EntityBuilder.h>
#include <cg/terrain/Terrain.h>
#include <cg/terrain/OpenSimplexNoise.h>
#include <cg/terrain/PerlinNoise.h>
#include <cg/IcoSphere.h>
#include <cg/entities/components/OrbitComponent.h>
#include <cg/skybox/SkyboxStar.h>
#include <cg/collision/BoundingSphere.h>
#include <cg/PlanetGenerator.h>
#include "cg/Application.h"

bool onUpdateTerrain(Terrain *terrain, TerrainSettings &settings) {
    bool success = terrain->update(settings);

    return success;
}

Application::Application(std::string title, int width, int height) {
    entityManager = new EntityManager();
    context = new GLContext(entityManager, title, width, height);

    universeEntityFactory = new UniverseEntityFactory(*entityManager, universe, *context);

    renderSystem = new RenderSystem(entityManager, context);
    inputSystem = new InputSystem(entityManager, context, renderSystem);
    movementSystem = new MovementSystem(entityManager, context);
    orbitSystem = new OrbitSystem(entityManager, context);
}

Application::~Application() {
    delete inputSystem;
    delete renderSystem;
    delete movementSystem;

    delete context;
    delete entityManager;
}

void Application::init() {
    context->init();

    context->meshProgram = new ShaderProgram();
    context->meshProgram->attachShader("./assets/shaders/mesh.vert", ShaderType::VertexShader);
    context->meshProgram->attachShader("./assets/shaders/mesh.frag", ShaderType::FragmentShader);
    context->meshProgram->link();

    context->meshTextureProgram = new ShaderProgram();
    context->meshTextureProgram->attachShader("./assets/shaders/meshTexture.vert", ShaderType::VertexShader);
    context->meshTextureProgram->attachShader("./assets/shaders/meshTexture.frag", ShaderType::FragmentShader);
    context->meshTextureProgram->link();

    context->starProgram = new ShaderProgram();
    context->starProgram->attachShader("./assets/shaders/starTexture.vert", ShaderType::VertexShader);
    context->starProgram->attachShader("./assets/shaders/starTexture.frag", ShaderType::FragmentShader);
    context->starProgram->link();

    context->planetProgram = new ShaderProgram();
    context->planetProgram->attachShader("./assets/shaders/planet.vert", ShaderType::VertexShader);
    context->planetProgram->attachShader("./assets/shaders/planet.frag", ShaderType::FragmentShader);
    context->planetProgram->link();

    context->gridProgram = new ShaderProgram();
    context->gridProgram->attachShader("./assets/shaders/grid.vert", ShaderType::VertexShader);
    context->gridProgram->attachShader("./assets/shaders/grid.frag", ShaderType::FragmentShader);
    context->gridProgram->link();

    context->highlightProgram = new ShaderProgram();
    context->highlightProgram->attachShader("./assets/shaders/highlight.vert", ShaderType::VertexShader);
    context->highlightProgram->attachShader("./assets/shaders/highlight.frag", ShaderType::FragmentShader);
    context->highlightProgram->link();

    context->meshWithLightProgram = new ShaderProgram();
    context->meshWithLightProgram->attachShader("./assets/shaders/meshWithLight.vert", ShaderType::VertexShader);
    context->meshWithLightProgram->attachShader("./assets/shaders/meshWithLight.frag", ShaderType::FragmentShader);
    context->meshWithLightProgram->link();

    context->meshTestLightProgram = new ShaderProgram();
    context->meshTestLightProgram->attachShader("./assets/shaders/mesh.vert", ShaderType::VertexShader);
    context->meshTestLightProgram->attachShader("./assets/shaders/meshTestLight.frag", ShaderType::FragmentShader);
    context->meshTestLightProgram->link();

    context->skyboxProgram = new ShaderProgram();
    context->skyboxProgram->attachShader("./assets/shaders/skybox.vert", ShaderType::VertexShader);
    context->skyboxProgram->attachShader("./assets/shaders/skybox.frag", ShaderType::FragmentShader);
    context->skyboxProgram->link();

    context->bloomProgram = new ShaderProgram();
    context->bloomProgram->attachShader("./assets/shaders/finalBloom.vert", ShaderType::VertexShader);
    context->bloomProgram->attachShader("./assets/shaders/finalBloom.frag", ShaderType::FragmentShader);
    context->bloomProgram->link();

    context->blurProgram = new ShaderProgram();
    context->blurProgram->attachShader("./assets/shaders/gaussianBlur.vert", ShaderType::VertexShader);
    context->blurProgram->attachShader("./assets/shaders/gaussianBlur.frag", ShaderType::FragmentShader);
    context->blurProgram->link();

    sky = new Skybox(10000, 100000, 20, "./assets/textures/skybox1", context->skyboxProgram);
    auto playerPosition = glm::vec3(0, 0, 0);
    context->player = EntityBuilder::create()
            ->withTransform(playerPosition)
            ->withVelocity(new VelocityComponent())
            ->withMesh(sky)
//            ->withMass(10)
            ->withCamera(CameraComponent::Mode::FirstPersonShip, CameraComponent::Type::Perspective, glm::vec3(0, 0, 0),
                         glm::normalize(-playerPosition), glm::vec3(0, 1, 0), context->getAspect())
            ->build(entityManager);
    context->setActiveCamera(context->player);

    createCameras();
    BoundingSphere playerBoundingSphere = BoundingSphere(0.01f, *entityManager->getComponent<TransformComponent>(
            context->player));

    gravitySystem = new GravitySystem(entityManager, context->player);
    collisionSystem = new CollisionSystem(entityManager, context->player, playerBoundingSphere);

    collisionSystem->init();
    renderSystem->init();
    inputSystem->init();
    movementSystem->init();
    gravitySystem->init();
    orbitSystem->init();

    context->grid = createGrid(62, 62, false);

    auto ui = renderSystem->getUserInterface();

    auto color = glm::vec3(0.576, 0.886, 1.0);

    auto galaxy = universe.getGalaxy(0, 0, 0);
    universeEntityFactory->createEntities(galaxy.getSolarSystems(0, 0, 0));

//    auto settings = PlanetSettings {
//            1.0,
//            80,
//            0,
//            MeshComponent::Material {
//                    planet.waterColor,
//                    glm::vec3(0.9),
//                    8,
//            },
//            MeshComponent::Material {
//                    planet.groundColor,
//                    glm::vec3(0.1),
//                    8,
//            },
//            MeshComponent::Material {
//                    planet.peakColor,
//                    glm::vec3(0.1),
//                    8,
//            },
//            FrontFace,
//            {
//                    {
//                            Simple,
//                            true,
//                            false,
//                            0.06,
//                            0.6,
//                            2.5,
//                            0.7,
//                            1.3,
//                            0.0,
//                            4,
//                            glm::vec3(0.7, 1.1, 1.8),
//                    },
//                    {
//                            Ridged,
//                            true,
//                            true,
//                            1.67,
//                            1.0,
//                            5.7,
//                            0.6,
//                            0.1,
//                            1.0,
//                            4,
//                            glm::vec3(0.7, 1.7, 0.7),
//                    },
//            }
//    };
//    auto sun = EntityBuilder::create()
//            ->withTransform(0, 0, 10)
//            ->withPointLight(glm::vec3(0.05), glm::vec3(0.95), glm::vec3(1.0), 1.0, 0.0014, 0.000007)
//            ->isSelectable()
//            ->build(entityManager);
//
//    auto planetGenerator = new PlanetGenerator(settings, *context->planetProgram);
//    auto test = EntityBuilder::create()
//            ->withTransform(0, 0, 0)
//            ->withMesh(planetGenerator->getMeshes())
//            ->isSelectable()
//            ->build(entityManager);

//    auto planetGenerator2 = new PlanetGenerator(settings, *context->meshProgram);
//    auto test2 = EntityBuilder::create()
//            ->withTransform(3, 0, 0)
//            ->withMesh(planetGenerator2->getMeshes())
//            ->isSelectable()
//            ->build(entityManager);

    inputSystem->createSpaceshipControl(nullptr, context->player);
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
        context->setActiveCamera(context->player);
        glViewport(0, 0, context->getWidth(), context->getHeight());
        glEnable(GL_CULL_FACE);
    }

    while (!context->shouldClose()) {
        context->update();

        // Update shader with light info
        int index = 0;
        std::vector<std::pair<LightComponent *, TransformComponent *>> lights;
        for (auto &pair : entityManager->getComponents<LightComponent>()) {
            // TODO: Take the MAX_LIGHTS closes to the camera
            if (index++ >= MAX_LIGHTS) {
                break;
            }
            auto entity = pair.first;
            auto light = dynamic_cast<LightComponent *>(pair.second);
            auto transform = entityManager->getComponent<TransformComponent>(entity);
            lights.emplace_back(std::make_pair(light, transform));
        }
        setLightUniforms(context->planetProgram, lights);

        // Process systems
        gravitySystem->update();
        orbitSystem->update();
        inputSystem->update();
        movementSystem->update();
        collisionSystem->update();
        renderSystem->update();
    }
}

void Application::createCameras() {
    auto target = glm::vec3(50, 50, 50);

    /// Spaceship camera
    auto position = glm::vec3(0, 200, 0);

    context->skyboxCamera = EntityBuilder::create()
            ->withTransform(0, 0, 0)
            ->withCamera(CameraComponent::Mode::CubeMap, CameraComponent::Type::CubeMapType, target,
                         glm::normalize(-position), glm::vec3(0, 1, 0), 1)
            ->build(entityManager);
}

Entity *Application::createGrid(int width, int height, bool showYAxis) {
    int startX = width / 2;
    int startZ = height / 2;
    std::vector<MeshComponent::Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<MeshComponent::Texture> textures;
    uint32_t index = 0;
    for (int i = 0; i < width; i++) {
        float step = 1.0f;
        auto start = MeshComponent::Vertex{step * glm::vec3(-startX + i, 0, -startZ)};
        auto end = MeshComponent::Vertex{step * glm::vec3(-startX + i, 0, -startZ + height)};
        vertices.push_back(start);
        vertices.push_back(end);
        indices.push_back(index++);
        indices.push_back(index++);
    }
    for (int i = 0; i < height; i++) {
        float step = 1.0f;
        auto start = MeshComponent::Vertex{step * glm::vec3(-startX, 0, -startZ + i)};
        auto end = MeshComponent::Vertex{step * glm::vec3(-startX + width, 0, -startZ + i)};
        vertices.push_back(start);
        vertices.push_back(end);
        indices.push_back(index++);
        indices.push_back(index++);
    }

    if (showYAxis) {
        vertices.push_back(MeshComponent::Vertex{glm::vec3(0, 0, 0)});
        vertices.push_back(MeshComponent::Vertex{glm::vec3(0, 10, 0)});
        indices.push_back(index++);
        indices.push_back(index++);
    }

    return EntityBuilder::create()
            ->withMesh(vertices, indices, textures, context->gridProgram, GL_LINES)
            ->build(entityManager);
}

void Application::setLightUniforms(ShaderProgram *shaderProgram,
                                   std::vector<std::pair<LightComponent *, TransformComponent *>> lights) {
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
