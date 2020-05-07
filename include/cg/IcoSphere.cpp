#include <stb_image.h>
#include "IcoSphere.h"


IcoSphere::IcoSphere(double radius, uint16_t subdivisions, glm::vec3 color, uint16_t textureWidth, ShaderProgram *shaderProgram)
        : radius(radius) {
    if (textureWidth < 11) {
        throw std::runtime_error("Texture width needs to be greater than or equal to 11");
    }
    this->shaderProgram = shaderProgram;
    this->color = color;
    mode = GL_TRIANGLES;
    indexed = false;
    instances = 1;
    subdivision = subdivisions;

    this->textureWidth = textureWidth;
    textureHeight = textureWidth / 2.0;
    horizontalStep = static_cast<int>(textureWidth / 11.0);
    verticalStep = static_cast<int>(textureHeight / 3.0);

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
    float s = horizontalStep / textureWidth;
    float t = verticalStep / textureHeight;

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
    auto diff = a - b;
    float halfLength = glm::length(diff) / 2.0f;
    auto unit = glm::normalize(diff);

    return glm::normalize(b + unit * halfLength);
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
            newVertices.emplace_back(Vertex { d, normal, dTextureCoord });
            newVertices.emplace_back(Vertex { f, normal, fTextureCoord });

            // Middle triangle
            normal = calculateNormal(d, e, f);
            newVertices.emplace_back(Vertex { d, normal, dTextureCoord });
            newVertices.emplace_back(Vertex { e, normal, eTextureCoord });
            newVertices.emplace_back(Vertex { f, normal, fTextureCoord });

            // Bottom right triangle
            normal = calculateNormal(f, e, c);
            newVertices.emplace_back(Vertex { f, normal, fTextureCoord });
            newVertices.emplace_back(Vertex { e, normal, eTextureCoord });
            newVertices.emplace_back(Vertex { c, normal, oldVertices[i + 2].textureCoord });

            // Top triangle
            normal = calculateNormal(d, b, e);
            newVertices.emplace_back(Vertex { d, normal, dTextureCoord });
            newVertices.emplace_back(Vertex { b, normal, oldVertices[i + 1].textureCoord });
            newVertices.emplace_back(Vertex { e, normal, eTextureCoord });
        }

        vertices = newVertices;
    }

    generateTexture();
    setupBuffers();
}

void IcoSphere::generateTexture() {
    auto size = textureWidth * textureHeight * 3 * 3;
    auto data = new unsigned char[size];
    for (int i = 0; i < size; i += 3) {
        data[i] = static_cast<unsigned char>(glm::round(color.x * 255));
        data[i + 1] = static_cast<unsigned char>(glm::round(color.y * 255));
        data[i + 2] = static_cast<unsigned char>(glm::round(color.z * 255));
    }

    auto texture = Texture {};
    texture.type = "texture_diffuse";

    glGenTextures(1, &texture.id);

    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    textures.emplace_back(texture);

    delete[] data;
}

void IcoSphere::setColor(glm::vec3 color) {
    this->color = color;
    generateTexture();
}

