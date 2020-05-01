#include "IcoSphere.h"


IcoSphere::IcoSphere(double radius, int subdivisions, ShaderProgram *shaderProgram)
        : radius(radius), subdivisions(subdivisions) {
    this->shaderProgram = shaderProgram;
    this->mode = GL_TRIANGLES;
    this->indexed = true;
    this->instances = 1;

    generateMesh();

    setupBuffers();
}

void IcoSphere::generateMesh() {
    const double inclination = PI / 180.0 * 72.0;
    const double azimuth = atan(0.5);
    const int vertexCount = 12;

    vertices.resize(vertexCount);

    // Top vertex
    vertices[0] = Vertex { {0.0f, radius, 0.0f} };

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

        Vertex lower{};
        lower.position = glm::vec3(xz * cos(inclinationLower), -y, xz * sin(inclinationLower));

        vertices[iUpper] = upper;
        vertices[iLower] = lower;

        // Upper row
        indices.emplace_back(0);
        indices.emplace_back(iUpperNext);
        indices.emplace_back(iUpper);

        // Lower row
        indices.emplace_back(11);
        indices.emplace_back(iLower);
        indices.emplace_back(iLowerNext);

        // Upper mid row
        indices.emplace_back(iUpper);
        indices.emplace_back(iUpperNext);
        indices.emplace_back(iLower);

        // Lower mid row
        indices.emplace_back(iLower);
        indices.emplace_back(iUpperNext);
        indices.emplace_back(iLowerNext);

        inclinationUpper += inclination;
        inclinationLower += inclination;
    }

    // Bottom vertex
    vertices[11] = Vertex { {0.0f, -radius, 0.0f} };

    for (int i = 0; i < vertices.size(); i++) {
        printf("vertices[%d] = (%f, %f, %f)\n", i, vertices[i].position.x, vertices[i].position.z, vertices[i].position.y);
    }

    // Update normals
//    vertices[0].normal = { 0.0f, 1.0f, 0.0f };
    vertices[0].normal = glm::normalize(vertices[0].position);
    for (int i = 1; i <= 5; i++) {
        int iUpper = i;
        int iUpperNext = i == 5 ? 1 : iUpper + 1;
        int iUpperPrev = i == 1 ? 5 : iUpper - 1;
        int iLower = i + 5;
        int iLowerNext = i == 5 ? 6 : iLower + 1;
        int iLowerPrev = i == 6 ? 10 : iLower - 1;

        Vertex upper = vertices[iUpper];
        Vertex lower = vertices[iLower];
//        auto origin = vertices[0].position - vertices[iUpper].position;
//        auto right = vertices[iUpperNext].position - vertices[iUpper].position;
//        auto normal = glm::normalize(glm::cross(right, origin));

//        vertices[iUpper].normal = calculateNormal(0, iUpperPrev, iUpper, iUpperNext, iLowerPrev, iLower);
        vertices[iUpper].normal = glm::normalize(upper.position);
        vertices[iLower].normal = glm::normalize(lower.position);

//        Vertex lower = vertices[iLower];
//        origin = vertices[11].position - vertices[iLower].position;
//        right = vertices[iLowerNext].position - vertices[iLower].position;
//        vertices[iLower].normal = -glm::normalize(glm::cross(origin, right));
    }
    vertices[11].normal = glm::normalize(vertices[11].position);
}

glm::vec3 IcoSphere::calculateNormal(int top, int prev, int upper, int next, int prevLower, int lower) {
    return glm::vec3();
}
