#include "IcoSphere.h"


IcoSphere::IcoSphere(double radius, uint16_t subdivisions, ShaderProgram *shaderProgram)
        : radius(radius) {
    this->shaderProgram = shaderProgram;
    mode = GL_TRIANGLES;
    indexed = false;
    instances = 1;
    subdivision = subdivisions;

    subdividedIndices.resize(1);
    generateMesh();
    generateTexture();

    subdivide(subdivisions);

    setupBuffers();
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

        // Upper row triangle
        vertices.emplace_back(Vertex { top, upperNormal });
        vertices.emplace_back(Vertex { upperNext, upperNormal });
        vertices.emplace_back(Vertex { upper, upperNormal });

        // Upper mid row triangle
        vertices.emplace_back(Vertex { upper, upperMidNormal });
        vertices.emplace_back(Vertex { upperNext, upperMidNormal });
        vertices.emplace_back(Vertex { lower, upperMidNormal });

        // Lower mid row triangle
        vertices.emplace_back(Vertex { lowerNext, lowerMidNormal });
        vertices.emplace_back(Vertex { lower, lowerMidNormal });
        vertices.emplace_back(Vertex { upperNext, lowerMidNormal });

        // Lower row triangle
        vertices.emplace_back(Vertex { bottom, lowerNormal });
        vertices.emplace_back(Vertex { lower, lowerNormal });
        vertices.emplace_back(Vertex { lowerNext, lowerNormal });

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

void IcoSphere::subdivide(uint16_t subdivisions) {
    if (subdivisions < subdividedIndices.size()) {
        indices = subdividedIndices[subdivisions];
        setupBuffers();
        printf("Loaded subdivisions for level: %d\n", subdivisions);
        return;
    }

    for (int level = subdividedIndices.size(); level <= subdivisions; level++) {
        subdividedIndices.emplace_back(std::vector<uint32_t>());
        auto oldIndices = subdividedIndices[level - 1];

        for (uint32_t i = 0; i < oldIndices.size(); i += 3) {
            auto a = vertices[oldIndices[i]].position; auto aIndex = oldIndices[i];
            auto b = vertices[oldIndices[i + 1]].position; auto bIndex = oldIndices[i + 1];
            auto c = vertices[oldIndices[i + 2]].position; auto cIndex = oldIndices[i + 2];
            auto d = halfPosition(a, b); auto dIndex = vertices.size();
            auto e = halfPosition(b, c); auto eIndex = dIndex + 1;
            auto f = halfPosition(a, c); auto fIndex = dIndex + 2;

            auto dVertex = Vertex { d, glm::normalize(d) };
            auto eVertex = Vertex { e, glm::normalize(e) };
            auto fVertex = Vertex { f, glm::normalize(f) };

            vertices.emplace_back(dVertex);
            vertices.emplace_back(eVertex);
            vertices.emplace_back(fVertex);

            subdividedIndices[level].emplace_back(aIndex);
            subdividedIndices[level].emplace_back(dIndex);
            subdividedIndices[level].emplace_back(fIndex);

            subdividedIndices[level].emplace_back(dIndex);
            subdividedIndices[level].emplace_back(eIndex);
            subdividedIndices[level].emplace_back(fIndex);

            subdividedIndices[level].emplace_back(fIndex);
            subdividedIndices[level].emplace_back(eIndex);
            subdividedIndices[level].emplace_back(cIndex);

            subdividedIndices[level].emplace_back(dIndex);
            subdividedIndices[level].emplace_back(bIndex);
            subdividedIndices[level].emplace_back(eIndex);
        }
    }
    indices = subdividedIndices[subdivisions];
    setupBuffers();
    printf("Created subdivisions for level: %d\n", subdivisions);
}

void IcoSphere::generateTexture() {
    auto width = 2048;
    auto height = 1024;

    float s = 186.0 / width;
    float t = 322.0 / height;

    vertices[0].textureCoord = glm::vec2(0, 0);
    for (int i = 1; i <= 5; i++) {
        int iUpper = i;
        int iUpperNext = i == 5 ? 1 : iUpper + 1;
        int iLower = i + 5;
        int iLowerNext = i == 5 ? 6 : iLower + 1;

        vertices[iUpper].textureCoord = glm::vec2(0, 0);
        vertices[iLower].textureCoord = glm::vec2(0, 0);
    }
    vertices[11].textureCoord = glm::vec2(0, 0);

//    auto texture = Texture {};
//    texture.id =
//    textures.emplace_back(texture);
}

