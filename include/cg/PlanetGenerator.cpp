#include <cg/terrain/OpenSimplexNoise.h>
#include "PlanetGenerator.h"

PlanetSettings getDefaultPlanetSettings() {
    PlanetSettings settings;
    settings.seed = 0;
    settings.radius = 1;
    settings.subdivision = 0;
    settings.strength = 1;
    settings.baseRoughness = 1;
    settings.roughness = 2;
    settings.persistence = 0.5;
    settings.minValue = 0.1;
    settings.center = glm::vec3(0);
    settings.layers = 0;
    return settings;
}

PlanetGenerator::PlanetGenerator(uint16_t subdivisions, ShaderProgram &shaderProgram)
        : settings(getDefaultPlanetSettings()), noise(createNoise(getDefaultPlanetSettings())) {
    this->shaderProgram = &shaderProgram;
    this->subdivision = subdivisions;
    indexed = false;
    mode = GL_TRIANGLES;
    instances = 1;

    createMesh();
}

PlanetGenerator::~PlanetGenerator() {
}

glm::vec3 mapCubeToSphere(glm::vec3 position) {
    auto sphere = position;

    auto x2 = position.x * position.x;
    auto y2 = position.y * position.y;
    auto z2 = position.z * position.z;

    sphere.x = position.x * glm::sqrt(1 - 0.5 * y2 - 0.5 * z2 + y2 * z2 / 3.0);
    sphere.y = position.y * glm::sqrt(1 - 0.5 * z2 - 0.5 * x2 + z2 * x2 / 3.0);
    sphere.z = position.z * glm::sqrt(1 - 0.5 * x2 - 0.5 * y2 + x2 * y2 / 3.0);

    return sphere;
}

glm::vec3 calculateFaceNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    return glm::normalize(glm::cross(a - b, a - c));
}

void PlanetGenerator::createTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    a = mapCubeToSphere(a);
    b = mapCubeToSphere(b);
    c = mapCubeToSphere(c);

    vertices.emplace_back(Vertex { a });
    vertices.emplace_back(Vertex { b });
    vertices.emplace_back(Vertex { c });
}

void PlanetGenerator::createMesh() {
    subdivide(subdivision);
    applyNoise();
    updateNormals();

    setupBuffers();
}

void PlanetGenerator::subdivide(uint16_t subdivisions) {
    this->subdivision = subdivisions;

    double start = 1.0;
    double step = 2.0 / (subdivision + 1);

    vertices.clear();

    // Front
    for (int r = 0; r < subdivision + 1; r++) {
        for (int c = 0; c < subdivision + 1; c++) {
            auto upper = glm::vec3(-start + (c + 0) * step, -start + (r + 1) * step, -start);
            auto upperNext = glm::vec3(-start + (c + 1) * step, -start + (r + 1) * step, -start);
            auto lower = glm::vec3(-start + (c + 0) * step, -start + (r + 0) * step, -start);
            auto lowerNext = glm::vec3(-start + (c + 1) * step, -start + (r + 0) * step, -start);

            createTriangle(upper, upperNext, lower);
            createTriangle(lower, upperNext, lowerNext);
        }
    }

    // Back
    for (int r = 0; r < subdivision + 1; r++) {
        for (int c = 0; c < subdivision + 1; c++) {
            auto upper = glm::vec3(-start + (c + 0) * step, -start + (r + 1) * step, start);
            auto upperNext = glm::vec3(-start + (c + 1) * step, -start + (r + 1) * step, start);
            auto lower = glm::vec3(-start + (c + 0) * step, -start + (r + 0) * step, start);
            auto lowerNext = glm::vec3(-start + (c + 1) * step, -start + (r + 0) * step, start);

            createTriangle(upper, lower, upperNext);
            createTriangle(lower, lowerNext, upperNext);
        }
    }

    // Left
    for (int r = 0; r < subdivision + 1; r++) {
        for (int c = 0; c < subdivision + 1; c++) {
            auto upper = glm::vec3(-start, -start + (r + 1) * step, -start + (c + 0) * step);
            auto upperNext = glm::vec3(-start, -start + (r + 1) * step, -start + (c + 1) * step);
            auto lower = glm::vec3(-start, -start + (r + 0) * step, -start + (c + 0) * step);
            auto lowerNext = glm::vec3(-start, -start + (r + 0) * step, -start + (c + 1) * step);

            createTriangle(upper, lower, upperNext);
            createTriangle(lower, lowerNext, upperNext);
        }
    }

    // Right
    for (int r = 0; r < subdivision + 1; r++) {
        for (int c = 0; c < subdivision + 1; c++) {
            auto upper = glm::vec3(start, -start + (r + 1) * step, -start + (c + 0) * step);
            auto upperNext = glm::vec3(start, -start + (r + 1) * step, -start + (c + 1) * step);
            auto lower = glm::vec3(start, -start + (r + 0) * step, -start + (c + 0) * step);
            auto lowerNext = glm::vec3(start, -start + (r + 0) * step, -start + (c + 1) * step);

            createTriangle(upper, upperNext, lower);
            createTriangle(lower, upperNext, lowerNext);
        }
    }

    // Up
    for (int r = 0; r < subdivision + 1; r++) {
        for (int c = 0; c < subdivision + 1; c++) {
            auto upper = glm::vec3(-start + (r + 1) * step, start, -start + (c + 0) * step);
            auto upperNext = glm::vec3(-start + (r + 1) * step, start, -start + (c + 1) * step);
            auto lower = glm::vec3(-start + (r + 0) * step, start, -start + (c + 0) * step);
            auto lowerNext = glm::vec3(-start + (r + 0) * step, start, -start + (c + 1) * step);

            createTriangle(upper, lower, upperNext);
            createTriangle(lower, lowerNext, upperNext);
        }
    }

    // Down
    for (int r = 0; r < subdivision + 1; r++) {
        for (int c = 0; c < subdivision + 1; c++) {
            auto upper = glm::vec3(-start + (r + 1) * step, -start, -start + (c + 0) * step);
            auto upperNext = glm::vec3(-start + (r + 1) * step, -start, -start + (c + 1) * step);
            auto lower = glm::vec3(-start + (r + 0) * step, -start, -start + (c + 0) * step);
            auto lowerNext = glm::vec3(-start + (r + 0) * step, -start, -start + (c + 1) * step);

            createTriangle(upper, upperNext, lower);
            createTriangle(lower, upperNext, lowerNext);
        }
    }
}

void PlanetGenerator::updateNormals() {
    for (int i = 0; i < vertices.size(); i += 3) {
        auto a = vertices[i];
        auto b = vertices[i + 1];
        auto c = vertices[i + 2];

        auto normal = calculateFaceNormal(a.position, b.position, c.position);

        vertices[i].normal = normal;
        vertices[i + 1].normal = normal;
        vertices[i + 2].normal = normal;
    }
}

Noise *PlanetGenerator::createNoise(PlanetSettings settings) {
    return new OpenSimplexNoise(settings);
}

void PlanetGenerator::updateSettings(PlanetSettings &settings) {
    this->settings = settings;
    subdivision = settings.subdivision;
    delete noise;
    noise = createNoise(settings);

    createMesh();
}

void PlanetGenerator::applyNoise() {
    for (auto &vertex : vertices) {
        float height = noise->evaluate(vertex.position.x, vertex.position.y, vertex.position.z);
        vertex.position = vertex.position * (1 + height) * settings.radius;
    }
}
