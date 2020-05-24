#include <stb_image.h>
#include <cg/entities/EntityBuilder.h>
#include <stb_image_write.h>
#include <cg/IcoSphere.h>
#include "cg/skybox/Skybox.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Skybox::Skybox(uint32_t size, uint32_t numStars, uint32_t numBigStars, ShaderProgram *shaderProgram) : size(size), numStars(numStars), numBigStars(numBigStars) {
    indexed = false;
    mode = GL_TRIANGLES;
    indexed = false;
    instances = 1;
    this->shaderProgram = shaderProgram;

    this->vertices.clear();
    auto vertices = createCubeMesh(size);
    for (auto vertex : vertices) {
        this->vertices.emplace_back(vertex);
    }
}

Skybox::Skybox(uint32_t size, uint32_t numStars, uint32_t numBigStars, std::string textureFilename, ShaderProgram *shaderProgram) : Skybox(size, numStars, numBigStars, shaderProgram) {
    createTexture(textureFilename);

    setupBuffers();
}

Skybox::Skybox(uint32_t size, uint32_t numStars, uint32_t numBigStars, glm::vec3 position, ShaderProgram *shaderProgram) : Skybox(size, numStars, numBigStars, shaderProgram) {
    createTexture(position);

    setupBuffers();
}

std::vector<MeshComponent::Vertex> Skybox::createCubeMesh(float size) {
    std::vector<MeshComponent::Vertex> vertices;

    float x = size / 2.0f;
    float y = size / 2.0f;
    float z = size / 2.0f;

    auto a =  glm::vec3(-x, y, z);
    auto b =  glm::vec3(x, y, z);
    auto c =  glm::vec3(-x, -y, z);
    auto d =  glm::vec3(x, -y, z);
    auto e =  glm::vec3(-x, y, -z);
    auto f =  glm::vec3(x, y, -z);
    auto g =  glm::vec3(-x, -y, -z);
    auto h =  glm::vec3(x, -y, -z);

    // Front
    createTriangle(vertices, a, b, c);
    createTriangle(vertices, b, d, c);

    // Top
    createTriangle(vertices, a, e, b);
    createTriangle(vertices, b, e, f);

    // Left side
    createTriangle(vertices, a, g, e);
    createTriangle(vertices, a, c, g);

    // Right side
    createTriangle(vertices, b, h, d);
    createTriangle(vertices, b, f, h);

    // Back side
    createTriangle(vertices, f, e, g);
    createTriangle(vertices, f, g, h);

    // Under side
    createTriangle(vertices, c, h, g);
    createTriangle(vertices, c, d, h);

    return vertices;
}

void Skybox::createTexture(glm::vec3 position) {
    throw std::runtime_error("Not implemented!");
}

void Skybox::createTexture(std::string filename) {
    std::vector<std::string> faces = {
            "./assets/textures/skybox1/right.png",
            "./assets/textures/skybox1/left.png",
            "./assets/textures/skybox1/top.png",
            "./assets/textures/skybox1/bottom.png",
            "./assets/textures/skybox1/front.png",
            "./assets/textures/skybox1/back.png",
    };

    Texture texture {};
    texture.isCubeMap = true;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);

    for (GLuint i = 0; i < faces.size(); i++) {
        int width, height, numChannels;
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &numChannels, 0);
        auto format = numChannels == 3 ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    textures.emplace_back(texture);
}

void Skybox::createTriangle(std::vector<MeshComponent::Vertex> &vertices, glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    auto normal = glm::normalize(glm::cross(b - a, c - a));

    vertices.emplace_back(Vertex { a, normal });
    vertices.emplace_back(Vertex { b, normal });
    vertices.emplace_back(Vertex { c, normal });
}

/**
 * Generate a random skybox texture based on the seed.
 */
void Skybox::generate(uint64_t seed) {
}

void
Skybox::render(RenderSystem *renderSystem, EntityManager *entityManager, CameraComponent *camera, bool saveToDisk) {
    std::vector<std::vector<glm::vec3>> directions = {
            // Front, right, up
            // X - pos / neg
            { glm::vec3(1, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0) },
            { glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, -1, 0) },
            // Y - pos / neg
            { glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1) },
            { glm::vec3(0, -1, 0), glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1) },
            // Z - pos / neg
            { glm::vec3(0, 0, 1), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0) },
            { glm::vec3(0, 0, -1), glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0) },
    };

    uint32_t framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glViewport(0, 0, resolution, resolution);

    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    auto starShaderProgram = new ShaderProgram();
    starShaderProgram->attachShader("./assets/shaders/skyboxStar.vert", ShaderType::VertexShader);
    starShaderProgram->attachShader("./assets/shaders/skyboxStar.frag", ShaderType::FragmentShader);
    starShaderProgram->link();

    auto nebularShaderProgram = new ShaderProgram();
    nebularShaderProgram->attachShader("./assets/shaders/nebula.vert", ShaderType::VertexShader);
    nebularShaderProgram->attachShader("./assets/shaders/nebula.frag", ShaderType::FragmentShader);
    nebularShaderProgram->link();

    // Normal stars
    createStars(entityManager, starShaderProgram, 0.92 * numStars, 0.03, 110, 1);
    createStars(entityManager, starShaderProgram, 0.06 * numStars, 0.05, 100, 1);
    createStars(entityManager, starShaderProgram, 0.02 * numStars, 0.05, 90, 1);

    // Big stars
    createStars(entityManager, starShaderProgram, numBigStars, 0.05, 40, 1);

    // Nebulas
    createNebulae(entityManager, nebularShaderProgram);

    const char *filenames[] = {
            "right.png",
            "left.png",
            "top.png",
            "bottom.png",
            "front.png",
            "back.png",
    };

    glEnable(GL_DEPTH);
    for (uint16_t i = 0; i < 6; i++) {
        // Rotate camera
        camera->front = directions[i][0];
        camera->right = directions[i][1];
        camera->up = directions[i][2];

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, resolution, resolution, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texture, 0);

        float diff = 5.0 / 255.0;
        auto clearColor = glm::vec4(0, 0, 0, 1.0);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderEntities(renderSystem, entityManager);

        /// Save the cubemap texture to files
        if (saveToDisk) {
            auto data = new uint8_t[resolution][resolution][3];
            glReadPixels(0, 0, resolution, resolution, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_write_png(filenames[i], resolution, resolution, 3, data, 0);
            delete[] data;
        }
    }

    textures[0].id = texture;

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("Failed to generate skybox!\n");
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &framebuffer);

    delete nebularShaderProgram;
    delete starShaderProgram;
}

void Skybox::createStars(EntityManager *entityManager, ShaderProgram *shaderProgram, uint32_t count, float starSize, float distance, uint16_t subdivisions) {
    std::vector<glm::vec3> transformations;
    for (int i = 0; i < count; i++) {
        auto position = distance * glm::normalize(glm::sphericalRand(1.0f));
        transformations.emplace_back(position);
    }
    auto builder = EntityBuilder::create();
    builder
            ->withTransform(0, 0, 0)
            ->withMesh(new IcoSphere(starSize, subdivisions, glm::vec3(1), 11, shaderProgram))
            ->withInstances(transformations)
            ->build(entityManager);
    delete builder;
}

void Skybox::renderEntities(RenderSystem *renderSystem, EntityManager *entityManager) {
    for (auto& pair : entityManager->getComponents<TransformComponent>()) {
        auto entity = pair.first;
        auto transform = dynamic_cast<TransformComponent *>(pair.second);
        auto meshes = entityManager->getMultiComponents<MeshComponent>(entity);

        for (auto it = meshes.first; it != meshes.second; it++) {
            auto mesh = dynamic_cast<MeshComponent *>(it->second);
            mesh->shaderProgram->use();
            mesh->shaderProgram->setUniform("uColor", glm::vec3(1, 0, 0));
            mesh->shaderProgram->setUniform("uOffset", glm::vec3(0));
            mesh->shaderProgram->setUniform("uScale", 0.25f);
            mesh->shaderProgram->setUniform("uIntensity", 0.9f);
            mesh->shaderProgram->setUniform("uFalloff", 3.0f);
            renderSystem->renderMesh(mesh, mesh->shaderProgram, transform->getModel());
        }
    }
}

void Skybox::createNebulae(EntityManager *entityManager, ShaderProgram *shaderProgram) {
    struct NebulaParams {
        glm::vec3 color;
        glm::vec3 offset;
        float scale;
        float intensity;
        float falloff;
    };
    std::vector<NebulaParams> nebulae;

    nebulae.push_back({
        glm::vec3(1, 0, 0),
        glm::vec3(0, 0, 0),
        0.25f,
        0.9f,
        3.0f,
    });

    auto nebulaVertices = createCubeMesh(size);
    std::vector<uint32_t> nebulaIndices;
    std::vector<MeshComponent::Texture> nebulaTextures;

    auto builder = EntityBuilder::create();
    auto entity = builder
            ->withTransform(glm::vec3(10, 0, 0))
            ->withMesh(nebulaVertices, nebulaIndices, nebulaTextures, shaderProgram, GL_TRIANGLES)
            ->build(entityManager);
    delete builder;
}
