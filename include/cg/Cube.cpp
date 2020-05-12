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

void Cube::createMesh() {
    double start = 0.5;
    double step = 1.0 / (subdivision + 1);

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
