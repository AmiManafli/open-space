#include "IcoSphere.h"


IcoSphere::IcoSphere(double radius, int maxSubdivisions, ShaderProgram *shaderProgram)
        : radius(radius) {
    this->shaderProgram = shaderProgram;
    this->mode = GL_TRIANGLES;
    this->indexed = true;
    this->instances = 1;
    this->subdivisions = maxSubdivisions;
    this->maxSubdivisions = maxSubdivisions;

    generateMesh();
    for (int level = 1; level <= maxSubdivisions; level++) {
        subdivide(level);
    }
    this->indices = subdividedIndices[subdivisions];

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

    // Middle vertices
    double inclinationUpper = -PI / 2.0 - inclination / 2.0;  // -126 degrees
    double inclinationLower = -PI / 2.0;                      // -90 degrees

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

void IcoSphere::subdivide(uint16_t level) {
    int max = 5 * level;

    indices.clear();
    for (int i = 1; i <= max; i++) {
        int iUpper = i;
        int iUpperNext = i == max ? 1 : iUpper + 1;
        int iLower = i + max;
        int iLowerNext = i == max ? max + 1 : iLower + 1;

        // Upper row triangle
        auto upperLeft = halfPosition(vertices[0].position, vertices[iUpper].position);
        auto upperRight = halfPosition(vertices[0].position, vertices[iUpperNext].position);
        auto upperLower = halfPosition(vertices[iUpper].position, vertices[iUpperNext].position);
        auto upperStartIndex = vertices.size();

        vertices.emplace_back(Vertex { upperLeft, glm::normalize(upperLeft) });
        vertices.emplace_back(Vertex { upperRight, glm::normalize(upperRight) });
        vertices.emplace_back(Vertex { upperLower, glm::normalize(upperLower) });

        subdividedIndices[level].emplace_back(0);
        subdividedIndices[level].emplace_back(upperStartIndex + 1);
        subdividedIndices[level].emplace_back(upperStartIndex);

        subdividedIndices[level].emplace_back(upperStartIndex);
        subdividedIndices[level].emplace_back(upperStartIndex + 2);
        subdividedIndices[level].emplace_back(iUpper);

        subdividedIndices[level].emplace_back(upperStartIndex);
        subdividedIndices[level].emplace_back(upperStartIndex + 1);
        subdividedIndices[level].emplace_back(upperStartIndex + 2);

        subdividedIndices[level].emplace_back(upperStartIndex + 1);
        subdividedIndices[level].emplace_back(iUpperNext);
        subdividedIndices[level].emplace_back(upperStartIndex + 2);

        // Lower row triangle
        auto lowerLeft = halfPosition(vertices[11].position, vertices[iLower].position);
        auto lowerRight = halfPosition(vertices[11].position, vertices[iLowerNext].position);
        auto lowerUpper = halfPosition(vertices[iLower].position, vertices[iLowerNext].position);
        auto lowerStartIndex = vertices.size();

        vertices.emplace_back(Vertex { lowerLeft, glm::normalize(lowerLeft) });
        vertices.emplace_back(Vertex { lowerRight, glm::normalize(lowerRight) });
        vertices.emplace_back(Vertex { lowerUpper, glm::normalize(lowerUpper) });

        subdividedIndices[level].emplace_back(11);
        subdividedIndices[level].emplace_back(lowerStartIndex + 0);
        subdividedIndices[level].emplace_back(lowerStartIndex + 1);

        subdividedIndices[level].emplace_back(lowerStartIndex + 0);
        subdividedIndices[level].emplace_back(lowerStartIndex + 2);
        subdividedIndices[level].emplace_back(lowerStartIndex + 1);

        subdividedIndices[level].emplace_back(lowerStartIndex + 0);
        subdividedIndices[level].emplace_back(iLower);
        subdividedIndices[level].emplace_back(lowerStartIndex + 2);

        subdividedIndices[level].emplace_back(lowerStartIndex + 1);
        subdividedIndices[level].emplace_back(lowerStartIndex + 2);
        subdividedIndices[level].emplace_back(iLowerNext);

        // Upper mid row triangle
        lowerLeft = halfPosition(vertices[iUpper].position, vertices[iLower].position);
        lowerRight = halfPosition(vertices[iUpperNext].position, vertices[iLower].position);
        lowerUpper = halfPosition(vertices[iUpper].position, vertices[iUpperNext].position);
        lowerStartIndex = vertices.size();

        vertices.emplace_back(Vertex { lowerLeft, glm::normalize(lowerLeft) });
        vertices.emplace_back(Vertex { lowerRight, glm::normalize(lowerRight) });
        vertices.emplace_back(Vertex { lowerUpper, glm::normalize(lowerUpper) });

        subdividedIndices[level].emplace_back(iLower);
        subdividedIndices[level].emplace_back(lowerStartIndex + 0);
        subdividedIndices[level].emplace_back(lowerStartIndex + 1);

        subdividedIndices[level].emplace_back(lowerStartIndex + 0);
        subdividedIndices[level].emplace_back(lowerStartIndex + 2);
        subdividedIndices[level].emplace_back(lowerStartIndex + 1);

        subdividedIndices[level].emplace_back(lowerStartIndex + 0);
        subdividedIndices[level].emplace_back(iUpper);
        subdividedIndices[level].emplace_back(lowerStartIndex + 2);

        subdividedIndices[level].emplace_back(lowerStartIndex + 1);
        subdividedIndices[level].emplace_back(lowerStartIndex + 2);
        subdividedIndices[level].emplace_back(iUpperNext);

        // Lower mid row triangle
        upperLeft = halfPosition(vertices[iUpperNext].position, vertices[iLower].position);
        upperRight = halfPosition(vertices[iUpperNext].position, vertices[iLowerNext].position);
        upperLower = halfPosition(vertices[iLower].position, vertices[iLowerNext].position);
        upperStartIndex = vertices.size();

        vertices.emplace_back(Vertex { upperLeft, glm::normalize(upperLeft) });
        vertices.emplace_back(Vertex { upperRight, glm::normalize(upperRight) });
        vertices.emplace_back(Vertex { upperLower, glm::normalize(upperLower) });

        subdividedIndices[level].emplace_back(iUpperNext);
        subdividedIndices[level].emplace_back(upperStartIndex + 1);
        subdividedIndices[level].emplace_back(upperStartIndex + 0);

        subdividedIndices[level].emplace_back(upperStartIndex + 0);
        subdividedIndices[level].emplace_back(upperStartIndex + 2);
        subdividedIndices[level].emplace_back(iLower);

        subdividedIndices[level].emplace_back(upperStartIndex + 0);
        subdividedIndices[level].emplace_back(upperStartIndex + 1);
        subdividedIndices[level].emplace_back(upperStartIndex + 2);

        subdividedIndices[level].emplace_back(upperStartIndex + 1);
        subdividedIndices[level].emplace_back(iLowerNext);
        subdividedIndices[level].emplace_back(upperStartIndex + 2);
    }
}
