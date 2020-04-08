#include "cg/terrain/Terrain.h"
#include <array>

Terrain::Terrain(uint32_t width, uint32_t height, std::vector<Vertex> &vertices, std::vector<uint32_t> &indices,
                 std::vector<Texture> &textures, ShaderProgram *shaderProgram, GLenum mode)
        : MeshComponent(vertices, indices, textures, shaderProgram, mode),
    width(width), height(height), subdivisionsWidth(1), subdivisionsHeight(1) {
}

Terrain::~Terrain() {
}

Terrain *Terrain::generate(uint32_t width, uint32_t height, ShaderProgram *shaderProgram, GLenum mode) {
    std::vector<MeshComponent::Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<MeshComponent::Texture> textures;

    build(vertices, indices, width, height, 1, 1);

    return new Terrain(width, height, vertices, indices, textures, shaderProgram, mode);
}

void Terrain::build(std::vector<MeshComponent::Vertex>& vertices, std::vector<uint32_t>& indices, uint32_t width, uint32_t height, uint32_t subdivisionsWidth, uint32_t subdivisionsHeight) {
    uint32_t vertexCount = (subdivisionsWidth + 1) * (subdivisionsHeight + 1);

    vertices.resize(vertexCount);
    indices.resize(6 * subdivisionsWidth * subdivisionsHeight);

    float tileWidth = width / static_cast<float>(subdivisionsWidth);
    float tileHeight = height / static_cast<float>(subdivisionsHeight);

    float startX = width / -2.0f;
    float startZ = height / -2.0f;

    for (int j = 0; j < subdivisionsHeight; j++) {
        for (int i = 0; i < subdivisionsWidth; i++) {
            float x = startX + (i * tileWidth);
            float y = 0.0f;
            float z = startZ + (j * tileHeight);

            uint32_t topLeftIndex = j * (subdivisionsWidth + 1) + i;
            uint32_t topRightIndex = j * (subdivisionsWidth + 1) + (i + 1);
            uint32_t bottomLeftIndex = (j + 1) * (subdivisionsWidth + 1) + i;
            uint32_t bottomRightIndex = (j + 1) * (subdivisionsWidth + 1) + (i + 1);

            if (i == 0) {
                if (j == 0) {
                    vertices[topLeftIndex] = { glm::vec3(x, y, z) };
                    vertices[topRightIndex] = { glm::vec3(x + tileWidth, y, z) };
                }
                vertices[bottomLeftIndex] = { glm::vec3(x, y, z + tileHeight) };
                vertices[bottomRightIndex] = { glm::vec3(x + tileWidth, y, z + tileHeight) };
            } else {
                if (j == 0) {
                    vertices[topRightIndex] = { glm::vec3(x + tileWidth, y, z) };
                }
                vertices[bottomRightIndex] = { glm::vec3(x + tileWidth, y, z + tileHeight) };
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

bool Terrain::update(uint32_t width, uint32_t height, uint32_t subdivisionsWidth, uint32_t subdivisionsHeight) {
    this->width = width;
    this->height = height;
    this->subdivisionsWidth = subdivisionsWidth;
    this->subdivisionsHeight = subdivisionsHeight;

    std::vector<MeshComponent::Vertex> vertices;
    std::vector<uint32_t> indices;

    build(vertices, indices, width, height, subdivisionsWidth, subdivisionsHeight);

    this->vertices = vertices;
    this->indices = indices;

    setupBuffers();

    return true;
}

