#include "cg/terrain/Terrain.h"
#include <array>

Terrain::Terrain(uint32_t width, uint32_t height, std::vector<Vertex> &vertices, std::vector<uint32_t> &indices,
                 std::vector<Texture> &textures, ShaderProgram *shaderProgram, GLenum mode, Noise *noise)
        : MeshComponent(vertices, indices, textures, shaderProgram, mode),
    width(width), height(height), subdivisionsWidth(1), subdivisionsHeight(1), noise(noise) {
}

Terrain::~Terrain() {
}

Terrain *Terrain::generate(uint32_t width, uint32_t height, ShaderProgram *shaderProgram, GLenum mode, Noise *noise) {
    std::vector<MeshComponent::Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<MeshComponent::Texture> textures;

    build(vertices, indices, width, height, 1, 1);

    return new Terrain(width, height, vertices, indices, textures, shaderProgram, mode, noise);
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
                    vertices[topLeftIndex] = { glm::vec3(x, y, z), { 0, 1, 0 } };
                    vertices[topRightIndex] = { glm::vec3(x + tileWidth, y, z), { 0, 1, 0 } };
                }
                vertices[bottomLeftIndex] = { glm::vec3(x, y, z + tileHeight), { 0, 1, 0 } };
                vertices[bottomRightIndex] = { glm::vec3(x + tileWidth, y, z + tileHeight), { 0, 1, 0 } };
            } else {
                if (j == 0) {
                    vertices[topRightIndex] = { glm::vec3(x + tileWidth, y, z), { 0, 1, 0 } };
                }
                vertices[bottomRightIndex] = { glm::vec3(x + tileWidth, y, z + tileHeight), { 0, 1, 0 } };
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

bool Terrain::update(TerrainSettings& settings) {
    this->width = settings.width;
    this->height = settings.height;
    this->subdivisionsWidth = settings.subdivisionWidth;
    this->subdivisionsHeight = settings.subdivisionHeight;

    std::vector<MeshComponent::Vertex> vertices;
    std::vector<uint32_t> indices;

    build(vertices, indices, width, height, subdivisionsWidth, subdivisionsHeight);

    this->vertices = vertices;
    this->indices = indices;

    // Update y-values with noise function
    updateHeights(settings);

    setupBuffers();

    printf("Terrain is now size %dx%d with %dx%d subdivisions.\n", width, height, subdivisionsWidth, subdivisionsHeight);

    return true;
}

void Terrain::updateHeights(TerrainSettings& settings) {
    for (uint32_t row = 0; row <= subdivisionsHeight; row++) {
        for (uint32_t col = 0; col <= subdivisionsWidth; col++) {
            auto index = row * subdivisionsWidth + col;
            auto y = noise->evaluate(col, row);
            vertices[index].position.y = y;
//            vertices[index].normal = noise->normal(row, col, zoom);
            vertices[index].normal = glm::vec3(0, 1, 0);
        }
    }
}

//glm::vec3 Terrain::calculateNormal(int row, int col, double zoom) {
//    auto dx = (noise->evaluate((col + 1) / zoom, row / zoom) - noise->evaluate(col / zoom, row / zoom)) / (1.0 / zoom);
//    auto dz = (noise->evaluate(col / zoom, (row + 1) / zoom) - noise->evaluate(col / zoom, row / zoom)) / (1.0 / zoom);
//
//    auto x = glm::vec3(1, dx, 0);
//    auto z = glm::vec3(0, dx, 1);
//
//    auto normal = glm::normalize(glm::cross(z, x));
//
//    return normal;
//}
