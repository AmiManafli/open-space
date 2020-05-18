#include <cg/terrain/OpenSimplexNoise.h>


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

PlanetSide::PlanetSide(PlanetSettings settings, ShaderProgram &shaderProgram) : settings(settings) {
    this->shaderProgram = &shaderProgram;
    indexed = false;
    mode = GL_TRIANGLES;
    instances = 1;

    updateSettings(settings);
    setupBuffers();
}

PlanetSide::~PlanetSide() {
    for (auto &noise : noises) {
        delete noise;
    }
    noises.clear();
}

void PlanetSide::createTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    a = mapCubeToSphere(a);
    b = mapCubeToSphere(b);
    c = mapCubeToSphere(c);

    vertices.emplace_back(Vertex { a });
    vertices.emplace_back(Vertex { b });
    vertices.emplace_back(Vertex { c });
}

void PlanetSide::createMesh() {
    maxHeight = 0.0f;

    subdivide(subdivision);
    applyNoise();
    updateNormals();

    materials.emplace_back(settings.waterMaterial);
    materials.emplace_back(settings.groundMaterial);
    materials.emplace_back(settings.peakMaterial);
}

void PlanetSide::subdivide(uint16_t subdivisions) {
    this->subdivision = subdivisions;

    double start = 1.0;
    double step = 2.0 / (subdivision + 1);

    vertices.clear();

    if (settings.direction == FrontFace) {
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
    } else if (settings.direction == BackFace) {
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
    } else if (settings.direction == LeftFace) {
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
    } else if (settings.direction == RightFace) {
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
    } else if (settings.direction == UpFace) {
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
    } else if (settings.direction == DownFace) {
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
}

glm::vec3 calculateFaceNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    return glm::normalize(glm::cross(a - b, a - c));
}

void PlanetSide::updateNormals() {
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

void PlanetSide::applyNoise() {
    auto firstLayerHeight = 0.0;
    for (auto &vertex : vertices) {
        double noiseValue = 0.0;

        if (!noises.empty()) {
            firstLayerHeight = noises[0]->evaluate(vertex.position.x, vertex.position.y, vertex.position.z);
            if (settings.noiseSettings[0].enabled) {
                noiseValue = firstLayerHeight;
            }
        }

        for (int i = 1; i < noises.size(); i++) {
            if (settings.noiseSettings[i].enabled) {
                float mask = settings.noiseSettings[i].useFirstLayerAsMask ? firstLayerHeight : 1.0f;
                noiseValue += noises[i]->evaluate(vertex.position.x, vertex.position.y, vertex.position.z) * mask;
            }
        }

        auto oldPosition = vertex.position;
        vertex.position = vertex.position * (1 + static_cast<float>(noiseValue)) * settings.radius;
        vertex.height = glm::length(oldPosition - vertex.position);

        if (vertex.height > maxHeight) {
            maxHeight = vertex.height;
        }
    }
}

void PlanetSide::updateSettings(PlanetSettings settings) {
    this->settings = settings;
    subdivision = settings.subdivision;

    for (int i = 0; i < noises.size(); i++) {
        delete noises[i];
    }
    noises.clear();

    for (auto &noiseSettings : settings.noiseSettings) {
        noises.emplace_back(new OpenSimplexNoise(settings, noiseSettings));
    }

    vertices.clear();
    materials.clear();
    textures.clear();

    createMesh();
}
