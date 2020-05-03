#include <stb_image.h>
#include "cg/Skybox.h"

Skybox::Skybox(glm::vec3 size, ShaderProgram *shaderProgram) : size(size) {
    indexed = false;
    mode = GL_TRIANGLES;
    indexed = false;
    instances = 1;
    this->shaderProgram = shaderProgram;

    createMesh();
}

Skybox::Skybox(glm::vec3 size, std::string textureFilename, ShaderProgram *shaderProgram) : Skybox(size, shaderProgram) {
    createTexture(textureFilename);

    setupBuffers();
}

Skybox::Skybox(glm::vec3 size, glm::vec3 position, ShaderProgram *shaderProgram) : Skybox(size, shaderProgram) {
    createTexture(position);

    setupBuffers();
}

void Skybox::createMesh() {
    float x = size.x / 2.0f;
    float y = size.y / 2.0f;
    float z = size.z / 2.0f;

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

void Skybox::render(RenderSystem *renderSystem) {
    std::vector<glm::vec4> clearColors = {
            glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
            glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),
            glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
            glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
            glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
    };

    uint32_t fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    for (uint16_t i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, 800, 800, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texture, 0);

        auto clearColor = clearColors[i];
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    textures[0].id = texture;

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("Failed to generate skybox!\n");
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &fbo);
}
