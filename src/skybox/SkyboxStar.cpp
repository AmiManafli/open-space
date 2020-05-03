#include "cg/skybox/SkyboxStar.h"

SkyboxStar::SkyboxStar(float size, ShaderProgram *shaderProgram) {
    this->shaderProgram = shaderProgram;
    indexed = false;
    mode = GL_TRIANGLES;
    instances = 1;

    float x = size / 2.0f;
    float y = size / 2.0f;

    auto a = glm::vec2(-x, y);
    auto b = glm::vec2(x, y);
    auto c = glm::vec2(-x, -y);
    auto d = glm::vec2(x, -y);

    vertices.emplace_back(Vertex { glm::vec3(a, 0) });
    vertices.emplace_back(Vertex { glm::vec3(c, 0) });
    vertices.emplace_back(Vertex { glm::vec3(b, 0) });
    vertices.emplace_back(Vertex { glm::vec3(b, 0) });
    vertices.emplace_back(Vertex { glm::vec3(c, 0) });
    vertices.emplace_back(Vertex { glm::vec3(d, 0) });

    setupBuffers();
}
