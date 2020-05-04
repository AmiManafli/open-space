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

    createMesh();
}

Skybox::Skybox(uint32_t size, uint32_t numStars, uint32_t numBigStars, std::string textureFilename, ShaderProgram *shaderProgram) : Skybox(size, numStars, numBigStars, shaderProgram) {
    createTexture(textureFilename);

    setupBuffers();
}

Skybox::Skybox(uint32_t size, uint32_t numStars, uint32_t numBigStars, glm::vec3 position, ShaderProgram *shaderProgram) : Skybox(size, numStars, numBigStars, shaderProgram) {
    createTexture(position);

    setupBuffers();
}

void Skybox::createMesh() {
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
    createTriangle(a, b, c);
    createTriangle(b, d, c);

    // Top
    createTriangle(a, e, b);
    createTriangle(b, e, f);

    // Left side
    createTriangle(a, g, e);
    createTriangle(a, c, g);

    // Right side
    createTriangle(b, h, d);
    createTriangle(b, f, h);

    // Back side
    createTriangle(f, e, g);
    createTriangle(f, g, h);

    // Under side
    createTriangle(c, h, g);
    createTriangle(c, d, h);
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

void Skybox::createTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
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

void Skybox::render(RenderSystem *renderSystem, EntityManager *entityManager, CameraComponent *camera) {
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

    // Normal stars
    createStars(entityManager, starShaderProgram, 0.92 * numStars, 0.03, 110, 1);
    createStars(entityManager, starShaderProgram, 0.06 * numStars, 0.05, 100, 1);
    createStars(entityManager, starShaderProgram, 0.02 * numStars, 0.05, 90, 1);

    // Big stars
    createStars(entityManager, starShaderProgram, numBigStars, 0.05, 40, 1);

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
//        auto clearColor = glm::vec4(i * diff, i * diff, i * diff, 1.0);
        auto clearColor = glm::vec4(0, 0, 0, 1.0);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderEntities(renderSystem, entityManager, starShaderProgram);

        /// Save the cubemap texture to files
//        auto data = new uint8_t[resolution][resolution][3];
//        glReadPixels(0, 0, resolution, resolution, GL_RGB, GL_UNSIGNED_BYTE, data);
//        stbi_write_png(filenames[i], resolution, resolution, 3, data, 0);
//        delete[] data;
    }

    textures[0].id = texture;

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("Failed to generate skybox!\n");
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &framebuffer);
}

glm::vec3 getStarRotation(glm::vec3 position) {
    auto pos = glm::normalize(position);
    auto forward = glm::vec3(0, 0, -1);
    auto angle = glm::acos(glm::dot(glm::normalize(pos), forward));

    auto up = glm::normalize(glm::cross(forward, pos));
    return up * angle;
}

void Skybox::createStars(EntityManager *entityManager, ShaderProgram *shaderProgram, uint32_t count, float starSize, float distance, uint16_t subdivisions) {
    std::vector<glm::vec3> transformations;
    for (int i = 0; i < count; i++) {
        auto position = distance * glm::normalize(glm::sphericalRand(1.0f));
        auto transform = new TransformComponent(position);
        transform->rotate(getStarRotation(position));
        transformations.emplace_back(position);
    }
    EntityBuilder::create()
            ->withTransform(0, 0, 0)
            ->withMesh(new IcoSphere(starSize, subdivisions, glm::vec3(1), 11, shaderProgram))
            ->withInstances(transformations)
            ->build(entityManager);
}

void Skybox::renderEntities(RenderSystem *renderSystem, EntityManager *entityManager, ShaderProgram *shaderProgram) {
    for (auto& pair : entityManager->getTransformComponents()) {
        auto entityId = pair.first;
        auto transform = pair.second;
        auto meshes = entityManager->getMeshComponents(entityId);

        for (auto it = meshes.first; it != meshes.second; it++) {
            auto mesh = it->second;
            renderSystem->renderMesh(mesh, shaderProgram, transform->getModel());
        }
    }
}
