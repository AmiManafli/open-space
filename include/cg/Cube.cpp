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

MeshComponent::Vertex createVertex(glm::vec3 position) {
    MeshComponent::Vertex vertex {};
    vertex.position = mapCubeToSphere(position);
    vertex.normal = glm::normalize(position);
    return vertex;
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

            auto normal = glm::vec3(0, 0, 1);

            vertices.emplace_back(Vertex { upper, normal });
            vertices.emplace_back(Vertex { upperNext, normal });
            vertices.emplace_back(Vertex { lower, normal });

            vertices.emplace_back(Vertex { lower, normal });
            vertices.emplace_back(Vertex { upperNext, normal });
            vertices.emplace_back(Vertex { lowerNext, normal });
        }
    }

    // Back
    for (int r = 0; r < subdivision + 1; r++) {
        for (int c = 0; c < subdivision + 1; c++) {
            auto upper = glm::vec3(-start + (c + 0) * step, -start + (r + 1) * step, start);
            auto upperNext = glm::vec3(-start + (c + 1) * step, -start + (r + 1) * step, start);
            auto lower = glm::vec3(-start + (c + 0) * step, -start + (r + 0) * step, start);
            auto lowerNext = glm::vec3(-start + (c + 1) * step, -start + (r + 0) * step, start);

            auto normal = glm::vec3(0, 0, -1);

            vertices.emplace_back(Vertex { upper, normal });
            vertices.emplace_back(Vertex { lower, normal });
            vertices.emplace_back(Vertex { upperNext, normal });

            vertices.emplace_back(Vertex { lower, normal });
            vertices.emplace_back(Vertex { lowerNext, normal });
            vertices.emplace_back(Vertex { upperNext, normal });
        }
    }

    // Left
    for (int r = 0; r < subdivision + 1; r++) {
        for (int c = 0; c < subdivision + 1; c++) {
            auto upper = glm::vec3(-start, -start + (r + 1) * step, -start + (c + 0) * step);
            auto upperNext = glm::vec3(-start, -start + (r + 1) * step, -start + (c + 1) * step);
            auto lower = glm::vec3(-start, -start + (r + 0) * step, -start + (c + 0) * step);
            auto lowerNext = glm::vec3(-start, -start + (r + 0) * step, -start + (c + 1) * step);

            auto normal = glm::vec3(-1, 0, 0);

            vertices.emplace_back(Vertex { upper, normal });
            vertices.emplace_back(Vertex { lower, normal });
            vertices.emplace_back(Vertex { upperNext, normal });

            vertices.emplace_back(Vertex { lower, normal });
            vertices.emplace_back(Vertex { lowerNext, normal });
            vertices.emplace_back(Vertex { upperNext, normal });
        }
    }

    // Right
    for (int r = 0; r < subdivision + 1; r++) {
        for (int c = 0; c < subdivision + 1; c++) {
            auto upper = glm::vec3(start, -start + (r + 1) * step, -start + (c + 0) * step);
            auto upperNext = glm::vec3(start, -start + (r + 1) * step, -start + (c + 1) * step);
            auto lower = glm::vec3(start, -start + (r + 0) * step, -start + (c + 0) * step);
            auto lowerNext = glm::vec3(start, -start + (r + 0) * step, -start + (c + 1) * step);

            auto normal = glm::vec3(1, 0, 0);

            vertices.emplace_back(Vertex { upper, normal });
            vertices.emplace_back(Vertex { upperNext, normal });
            vertices.emplace_back(Vertex { lower, normal });

            vertices.emplace_back(Vertex { lower, normal });
            vertices.emplace_back(Vertex { upperNext, normal });
            vertices.emplace_back(Vertex { lowerNext, normal });
        }
    }

    // Up
    for (int r = 0; r < subdivision + 1; r++) {
        for (int c = 0; c < subdivision + 1; c++) {
            auto upper = glm::vec3(-start + (r + 1) * step, start, -start + (c + 0) * step);
            auto upperNext = glm::vec3(-start + (r + 1) * step, start, -start + (c + 1) * step);
            auto lower = glm::vec3(-start + (r + 0) * step, start, -start + (c + 0) * step);
            auto lowerNext = glm::vec3(-start + (r + 0) * step, start, -start + (c + 1) * step);

            auto normal = glm::vec3(0, 1, 0);

            vertices.emplace_back(Vertex { upper, normal });
            vertices.emplace_back(Vertex { lower, normal });
            vertices.emplace_back(Vertex { upperNext, normal });

            vertices.emplace_back(Vertex { lower, normal });
            vertices.emplace_back(Vertex { lowerNext, normal });
            vertices.emplace_back(Vertex { upperNext, normal });
        }
    }

    // Down
    for (int r = 0; r < subdivision + 1; r++) {
        for (int c = 0; c < subdivision + 1; c++) {
            auto upper = glm::vec3(-start + (r + 1) * step, -start, -start + (c + 0) * step);
            auto upperNext = glm::vec3(-start + (r + 1) * step, -start, -start + (c + 1) * step);
            auto lower = glm::vec3(-start + (r + 0) * step, -start, -start + (c + 0) * step);
            auto lowerNext = glm::vec3(-start + (r + 0) * step, -start, -start + (c + 1) * step);

            auto normal = glm::vec3(0, -1, 0);

            vertices.emplace_back(Vertex { upper, normal });
            vertices.emplace_back(Vertex { upperNext, normal });
            vertices.emplace_back(Vertex { lower, normal });

            vertices.emplace_back(Vertex { lower, normal });
            vertices.emplace_back(Vertex { upperNext, normal });
            vertices.emplace_back(Vertex { lowerNext, normal });
        }
    }

    setupBuffers();
}
