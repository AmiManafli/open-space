#include "cg/terrain/Terrain.h"
#include <array>
#include <cg/terrain/PerlinNoise.h>
#include <cg/terrain/OpenSimplexNoise.h>

Terrain::Terrain(uint32_t width, uint32_t height, std::vector<Vertex> &vertices, std::vector<uint32_t> &indices,
                 std::vector<Texture> &textures, ShaderProgram *shaderProgram, GLenum mode, NoiseType noiseType)
        : MeshComponent(vertices, indices, textures, shaderProgram, mode),
          width(width), height(height), subdivisionsWidth(1), subdivisionsHeight(1), noiseType(noiseType) {
    perlinNoise = new PerlinNoise(1);
    openSimplexNoise = new OpenSimplexNoise(1);
}

Terrain::~Terrain() {
    delete perlinNoise;
    delete openSimplexNoise;
}

Noise* Terrain::getNoise() {
    switch (noiseType) {
        case OpenSimplex:
            return openSimplexNoise;
        case Perlin:
            return perlinNoise;
        default:
            return openSimplexNoise;
    }
}

Terrain *Terrain::generate(uint32_t width, uint32_t height, ShaderProgram *shaderProgram, GLenum mode, NoiseType noiseType) {
    std::vector<MeshComponent::Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<MeshComponent::Texture> textures;

    build(vertices, indices, width, height, 1, 1);

    return new Terrain(width, height, vertices, indices, textures, shaderProgram, mode, noiseType);
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

    noiseType = settings.noiseType;

    if (settings.seed != getNoise()->getSeed()) {
        getNoise()->reseed(settings.seed);
    }

    std::vector<MeshComponent::Vertex> vertices;
    std::vector<uint32_t> indices;

    build(vertices, indices, width, height, subdivisionsWidth, subdivisionsHeight);

    this->vertices = vertices;
    this->indices = indices;

    // Update y-values with noise function
    updateHeights(settings);
    updateNormals(settings);

    setupBuffers();

    printf("Terrain is now size %dx%d with %dx%d subdivisions.\n", width, height, subdivisionsWidth, subdivisionsHeight);

    return true;
}

void Terrain::updateHeights(TerrainSettings& settings) {
    // Update vertices
    printf("Redistribution: %.6f\n", settings.redistribution);
    for (uint64_t row = 0; row <= subdivisionsHeight; row++) {
        for (uint64_t col = 0; col <= subdivisionsWidth; col++) {
            auto index = row * subdivisionsWidth + col;
            double y = 0.0;
            for (uint64_t n = 0; n <= settings.octaves; n++) {
                auto frequency = settings.frequency * pow(2, n);
                auto amplitude = pow(settings.persistence, n);
                y += amplitude * getNoise()->evaluate(frequency * col, frequency * row);
            }
            vertices[index].position.y = pow(y * settings.maxAmplitude, settings.redistribution);
//            printf("pow(%.6f * %.6f, %.6f)  ===>  y = %.6f\n", y, settings.maxAmplitude, settings.redistribution, vertices[index].position.y);
        }
    }
}

void Terrain::updateNormals(TerrainSettings &settings) {
    for (uint32_t row = 0; row <= subdivisionsHeight; row++) {
        for (uint32_t col = 0; col <= subdivisionsWidth; col++) {
            auto index = row * subdivisionsWidth + col;
            auto indexRight = row * subdivisionsWidth + col + 1;
            auto indexDown = (row + 1) * subdivisionsWidth + col;

            if (col > subdivisionsWidth || row > subdivisionsHeight) {
                // TODO: use indexLeft and/or indexUp if either doesn't exist
                vertices[index].normal = glm::vec3(0, -1, 0);
                continue;
            }

            auto dx = (vertices[indexRight].position.y - vertices[index].position.y) / (width / static_cast<float>(subdivisionsWidth));
            auto dz = (vertices[indexDown].position.y - vertices[index].position.y) / (height / static_cast<float>(subdivisionsHeight));

            vertices[index].normal = glm::normalize(glm::cross(glm::vec3(0, dz, 1), glm::vec3(1, dx, 0)));
        }
    }
}
