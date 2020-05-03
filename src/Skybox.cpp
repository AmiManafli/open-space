#include <stb_image.h>
#include "cg/Skybox.h"

Skybox::Skybox(ShaderProgram *shaderProgram) {
    indexed = false;
    mode = GL_TRIANGLES;
    indexed = false;
    instances = 1;
    this->shaderProgram = shaderProgram;

    createMesh();
}

Skybox::Skybox(std::string textureFilename, ShaderProgram *shaderProgram) : Skybox(shaderProgram) {
    createTexture(textureFilename);

    setupBuffers();
}

Skybox::Skybox(glm::vec3 position, ShaderProgram *shaderProgram) : Skybox(shaderProgram) {
    createTexture(position);

    setupBuffers();
}

void Skybox::createMesh() {
    auto a =  glm::vec3(-0.5, 0.5, 0.5);
    auto b =  glm::vec3(0.5, 0.5, 0.5);
    auto c =  glm::vec3(-0.5, -0.5, 0.5);
    auto d =  glm::vec3(0.5, -0.5, 0.5);
    auto e =  glm::vec3(-0.5, 0.5, -0.5);
    auto f =  glm::vec3(0.5, 0.5, -0.5);
    auto g =  glm::vec3(-0.5, -0.5, -0.5);
    auto h =  glm::vec3(0.5, -0.5, -0.5);

    // Front
    createTriangle(a, c, b);
    createTriangle(b, c, d);

    // Top
    createTriangle(a, b, e);
    createTriangle(b, f, e);

    // Left side
    createTriangle(a, e, g);
    createTriangle(a, g, c);

    // Right side
    createTriangle(b, d, h);
    createTriangle(b, h, f);

    // Back side
    createTriangle(f, g, e);
    createTriangle(f, h, g);

    // Under side
    createTriangle(c, g, h);
    createTriangle(c, h, d);
}

void Skybox::createTexture(glm::vec3 position) {
    throw std::runtime_error("Not implemented!");
}

void Skybox::createTexture(std::string filename) {
    std::vector<std::string> faces(6);
    Texture texture {};

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);

    for (GLuint i = 0; i < faces.size(); i++) {
        int width, height, numChannels;
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &numChannels, 0);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
