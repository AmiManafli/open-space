#include "IcoSphere.h"


IcoSphere::IcoSphere(double radius, uint16_t subdivisions, ShaderProgram *shaderProgram)
        : radius(radius) {
    this->shaderProgram = shaderProgram;
    mode = GL_TRIANGLES;
    indexed = true;
    instances = 1;
    subdivision = subdivisions;

    subdividedIndices.resize(1);
    generateMesh();
    generateTexture();

    subdivide(subdivisions);

    setupBuffers();
}

void IcoSphere::generateMesh() {
    const double inclination = PI / 180.0 * 72.0;
    const double azimuth = atan(0.5);
    const int vertexCount = 12;

    vertices.resize(vertexCount);

    // Top vertex
    vertices[0] = Vertex { {0.0f, radius, 0.0f} };
    vertices[0].normal = glm::normalize(vertices[0].position);

    double inclinationUpper = -PI / 2.0 - inclination / 2.0;  // -126 degrees
    double inclinationLower = -PI / 2.0;                      // -90 degrees

    // Middle vertices
    for (int i = 1; i <= 5; i++) {
        int iUpper = i;
        int iUpperNext = i == 5 ? 1 : iUpper + 1;
        int iLower = i + 5;
        int iLowerNext = i == 5 ? 6 : iLower + 1;

        double y = radius * sin(azimuth);
        double xz = radius * cos(azimuth);

        Vertex upper{};
        upper.position = glm::vec3(xz * cos(inclinationUpper), y, xz * sin(inclinationUpper));
        upper.normal = glm::normalize(upper.position);

        Vertex lower{};
        lower.position = glm::vec3(xz * cos(inclinationLower), -y, xz * sin(inclinationLower));
        lower.normal = glm::normalize(lower.position);

        vertices[iUpper] = upper;
        vertices[iLower] = lower;

        // Upper row triangle
        subdividedIndices[0].emplace_back(0);
        subdividedIndices[0].emplace_back(iUpperNext);
        subdividedIndices[0].emplace_back(iUpper);

        // Lower row triangle
        subdividedIndices[0].emplace_back(11);
        subdividedIndices[0].emplace_back(iLower);
        subdividedIndices[0].emplace_back(iLowerNext);

        // Upper mid row triangle
        subdividedIndices[0].emplace_back(iUpper);
        subdividedIndices[0].emplace_back(iUpperNext);
        subdividedIndices[0].emplace_back(iLower);

        // Lower mid row triangle
        subdividedIndices[0].emplace_back(iLowerNext);
        subdividedIndices[0].emplace_back(iLower);
        subdividedIndices[0].emplace_back(iUpperNext);

        inclinationUpper += inclination;
        inclinationLower += inclination;
    }

    // Bottom vertex
    vertices[11] = Vertex { {0.0f, -radius, 0.0f} };
    vertices[11].normal = glm::normalize(vertices[11].position);
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

