#include "cg/terrain/Terrain.h"
#include <array>

Terrain::Terrain(uint32_t width, uint32_t height, std::vector<Vertex> &vertices, std::vector<uint32_t> &indices,
                 std::vector<Texture> &textures, uint32_t subdivisionLevel, ShaderProgram *shaderProgram, GLenum mode)
        : MeshComponent(vertices, indices, textures, shaderProgram, mode),
    width(width), height(height), subdivisionLevel(subdivisionLevel) {
}

Terrain::~Terrain() {
}

Terrain *Terrain::generate(uint32_t width, uint32_t height, ShaderProgram *shaderProgram, GLenum mode) {
    std::vector<MeshComponent::Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<MeshComponent::Texture> textures;

    uint32_t subdivisionLevel = 0;

    build(vertices, indices, width, height, subdivisionLevel);

    return new Terrain(width, height, vertices, indices, textures, subdivisionLevel, shaderProgram, mode);
}

void Terrain::build(std::vector<MeshComponent::Vertex>& vertices, std::vector<uint32_t>& indices, uint32_t width, uint32_t height, uint32_t subdivisionLevel) {
    uint32_t n = glm::pow(2, subdivisionLevel);
    uint32_t vertexCount = (n + 1) * (n + 1);

    vertices.resize(vertexCount);
    indices.resize(6 * n * n);

    float tileSize = width / static_cast<float>(n);
    float start = width / -2.0f;

    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            float x = start + (i * tileSize);
            float y = 0.0f;
            float z = start + (j * tileSize);

            uint32_t topLeftIndex = j * (n + 1) + i;
            uint32_t topRightIndex = j * (n + 1) + (i + 1);
            uint32_t bottomLeftIndex = (j + 1) * (n + 1) + i;
            uint32_t bottomRightIndex = (j + 1) * (n + 1) + (i + 1);

            if (i == 0) {
                if (j == 0) {
                    vertices[topLeftIndex] = { glm::vec3(x, y, z) };
                    vertices[topRightIndex] = { glm::vec3(x + tileSize, y, z) };
                }
                vertices[bottomLeftIndex] = { glm::vec3(x, y, z + tileSize) };
                vertices[bottomRightIndex] = { glm::vec3(x + tileSize, y, z + tileSize) };
            } else {
                if (j == 0) {
                    vertices[topRightIndex] = { glm::vec3(x + tileSize, y, z) };
                }
                vertices[bottomRightIndex] = { glm::vec3(x + tileSize, y, z + tileSize) };
            }

            indices.push_back(topLeftIndex);
            indices.push_back(bottomRightIndex);
            indices.push_back(topRightIndex);

            indices.push_back(topLeftIndex);
            indices.push_back(bottomLeftIndex);
            indices.push_back(bottomRightIndex);
        }
    }
}

void Terrain::subdivide(uint32_t level) {
    subdivisionLevel = level;
}
