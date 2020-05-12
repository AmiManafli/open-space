#include "Cube.h"

Cube::Cube(uint16_t subdivisions, ShaderProgram &shaderProgram) {
    this->shaderProgram = &shaderProgram;
    this->subdivision = subdivisions;
    indexed = false;
    mode = GL_TRIANGLES;
    instances = 1;

    createMesh();
}

Cube::~Cube() {
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

void Cube::createMesh() {
    subdivide(subdivision);
}

glm::vec3 calculateFaceNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    return glm::normalize(glm::cross(a - b, a - c));
}

MeshComponent::Vertex createCubeVertex(glm::vec3 position, glm::vec3 normal) {
    MeshComponent::Vertex vertex {};
    vertex.position = mapCubeToSphere(position);
    vertex.normal = normal;
    return vertex;
}

void Cube::createTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    a = mapCubeToSphere(a);
    b = mapCubeToSphere(b);
    c = mapCubeToSphere(c);

    auto normal = calculateFaceNormal(a, b, c);

    vertices.emplace_back(Vertex { a, normal });
    vertices.emplace_back(Vertex { b, normal });
    vertices.emplace_back(Vertex { c, normal });
}

void Cube::subdivide(uint16_t subdivisions) {
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

    setupBuffers();
}
