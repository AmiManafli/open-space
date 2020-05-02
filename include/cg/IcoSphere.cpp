#include "IcoSphere.h"


IcoSphere::IcoSphere(double radius, uint16_t subdivisions, ShaderProgram *shaderProgram)
        : radius(radius) {
    this->shaderProgram = shaderProgram;
    mode = GL_TRIANGLES;
    indexed = false;
    instances = 1;
    subdivision = subdivisions;

    textureWidth = 2048;
    textureHeight = 1024;
    horizontalStep = 186.0;
    verticalStep = 322.0;

    subdivide(subdivisions);
}

glm::vec3 calculateNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    return glm::normalize(glm::cross(a - b, a - c));
}

void IcoSphere::generateMesh() {
    const double inclination = PI / 180.0 * 72.0;
    const double azimuth = atan(0.5);

    auto top = glm::vec3(0, radius, 0);
    auto bottom = glm::vec3(0, -radius, 0);

    double inclinationUpper = -PI / 2.0 - inclination / 2.0;  // -126 degrees
    double inclinationLower = -PI / 2.0;                      // -90 degrees

    // Texture
    auto width = 2048;
    auto height = 1024;

    float s = 186.0 / width;
    float t = 322.0 / height;

    for (int i = 1; i <= 5; i++) {
        double y = radius * sin(azimuth);
        double xz = radius * cos(azimuth);

        auto upper = glm::vec3(xz * cos(inclinationUpper), y, xz * sin(inclinationUpper));
        auto upperNext = glm::vec3(xz * cos(inclinationUpper + inclination), y, xz * sin(inclinationUpper + inclination));

        auto lower = glm::vec3(xz * cos(inclinationLower), -y, xz * sin(inclinationLower));
        auto lowerNext = glm::vec3(xz * cos(inclinationLower + inclination), -y, xz * sin(inclinationLower + inclination));

        auto upperNormal = calculateNormal(top, upper, upperNext);
        auto upperMidNormal = calculateNormal(upper, upperNext, lower);
        auto lowerMidNormal = calculateNormal(lower, lowerNext, upperNext);
        auto lowerNormal = calculateNormal(bottom, lower, lowerNext);

        // Texture coordinates
        int j = i - 1;

        // Upper row triangle
        vertices.emplace_back(Vertex {
            top, upperNormal,
            glm::vec2(s * static_cast<float>(1 + j * 2), 0)
        });
        vertices.emplace_back(Vertex {
            upperNext, upperNormal,
            glm::vec2(s * static_cast<float>(j * 2), t)
        });
        vertices.emplace_back(Vertex {
            upper, upperNormal,
            glm::vec2(s * static_cast<float>(2 + j * 2), t)
        });

        // Upper mid row triangle
        vertices.emplace_back(Vertex {
            upper, upperMidNormal,
            glm::vec2(s * static_cast<float>(j * 2), t)
        });
        vertices.emplace_back(Vertex {
            upperNext, upperMidNormal,
            glm::vec2(s * static_cast<float>(2 + j * 2), t)
        });
        vertices.emplace_back(Vertex {
            lower, upperMidNormal,
            glm::vec2(s * static_cast<float>(1 + j * 2), 2 * t)
         });

        // Lower mid row triangle
        vertices.emplace_back(Vertex {
            lowerNext, lowerMidNormal,
            glm::vec2(s * static_cast<float>(2 + j * 2), 2 * t)
        });
        vertices.emplace_back(Vertex {
            lower, lowerMidNormal,
            glm::vec2(s * static_cast<float>(1 + j * 2), 2 * t)
        });
        vertices.emplace_back(Vertex {
            upperNext, lowerMidNormal,
            glm::vec2(s * static_cast<float>(2 + j * 2), t)
        });

        // Lower row triangle
        vertices.emplace_back(Vertex {
            bottom, lowerNormal,
            glm::vec2(s * static_cast<float>(2 + j * 2), 3 * t)
        });
        vertices.emplace_back(Vertex {
            lower, lowerNormal,
            glm::vec2(s * static_cast<float>(1 + j * 2), 2 * t)
        });
        vertices.emplace_back(Vertex {
            lowerNext, lowerNormal,
            glm::vec2(s * static_cast<float>(2 + j * 2), 2 * t)
        });

        inclinationUpper += inclination;
        inclinationLower += inclination;
    }
}

glm::vec3 IcoSphere::halfPosition(glm::vec3 a, glm::vec3 b) {
    auto diff = a - b;
    float halfLength = glm::length(diff) / 2.0f;
    auto unit = glm::normalize(diff);

    return glm::normalize(b + unit * halfLength) * static_cast<float>(radius);
}

glm::vec2 IcoSphere::halfTextureCoord(glm::vec2 a, glm::vec2 b) {
    return glm::vec2();
}

void IcoSphere::subdivide(uint16_t subdivisions) {
    vertices.clear();
    generateMesh();

    for (int level = 1; level <= subdivisions; level++) {
        auto oldVertices = vertices;
        auto newVertices = std::vector<Vertex>();

        for (uint32_t i = 0; i < oldVertices.size(); i += 3) {
            // Old vertices
            auto a = oldVertices[i].position;
            auto b = oldVertices[i + 1].position;
            auto c = oldVertices[i + 2].position;

            // New vertex positions
            auto d = halfPosition(a, b);
            auto e = halfPosition(b, c);
            auto f = halfPosition(a, c);

            auto dTextureCoord = halfTextureCoord(oldVertices[i].textureCoord, oldVertices[i + 1].textureCoord);
            auto eTextureCoord = halfTextureCoord(oldVertices[i + 1].textureCoord, oldVertices[i + 2].textureCoord);
            auto fTextureCoord = halfTextureCoord(oldVertices[i].textureCoord, oldVertices[i + 2].textureCoord);

            // Bottom left triangle
            auto normal = calculateNormal(a, d, f);
            newVertices.emplace_back(Vertex { a, normal, oldVertices[i].textureCoord });
            newVertices.emplace_back(Vertex {
                d, normal,
            });
            newVertices.emplace_back(Vertex {
                f, normal,
            });

            // Middle triangle
            normal = calculateNormal(d, e, f);
            newVertices.emplace_back(Vertex {
                d, normal,
            });
            newVertices.emplace_back(Vertex {
                e, normal,
            });
            newVertices.emplace_back(Vertex {
                f, normal,
            });

            // Bottom right triangle
            normal = calculateNormal(f, e, c);
            newVertices.emplace_back(Vertex {
                f, normal,
            });
            newVertices.emplace_back(Vertex {
                e, normal,
            });
            newVertices.emplace_back(Vertex { c, normal, oldVertices[i + 2].textureCoord });

            // Top triangle
            normal = calculateNormal(d, b, e);
            newVertices.emplace_back(Vertex {
                d, normal,
            });
            newVertices.emplace_back(Vertex { b, normal, oldVertices[i + 1].textureCoord });
            newVertices.emplace_back(Vertex {
                e, normal,
            });
        }

        vertices = newVertices;
    }

    generateTexture();
    setupBuffers();
}

void IcoSphere::generateTexture() {
}

