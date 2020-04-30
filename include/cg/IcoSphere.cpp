#include "IcoSphere.h"


IcoSphere::IcoSphere(double radius, int subdivisions, ShaderProgram *shaderProgram) : radius(radius), subdivisions(subdivisions) {
    this->shaderProgram = shaderProgram;
    this->mode = GL_TRIANGLES;
    this->indexed = true;

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

        printf("y = %f\n", y);

        Vertex upper{};
        upper.position = glm::vec3(xz * cos(inclinationUpper), y, xz * sin(inclinationUpper));

        Vertex lower{};
        lower.position = glm::vec3(xz * cos(inclinationLower), -y, xz * sin(inclinationLower));

        vertices[iUpper] = upper;
        vertices[iLower] = lower;

        // Upper row
        indices.emplace_back(0);
        indices.emplace_back(iUpper);
        indices.emplace_back(iUpperNext);

        // Upper mid row
        indices.emplace_back(iUpper);
        indices.emplace_back(iUpperNext);
        indices.emplace_back(iLower);

        // Lower mid row
        indices.emplace_back(iLower);
        indices.emplace_back(iLowerNext);
        indices.emplace_back(iUpperNext);

        // Lower row
        indices.emplace_back(11);
        indices.emplace_back(iLowerNext);
        indices.emplace_back(iLower);

        inclinationUpper += inclination;
        inclinationLower += inclination;
    }

    // Bottom vertex
    vertices[11] = Vertex { {0.0f, -radius, 0.0f} };

    // Update normals
    vertices[0].normal = { 0.0f, 1.0f, 0.0f };
    for (int i = 1; i <= 5; i++) {
        int iUpper = i;
        int iUpperNext = i == 5 ? 1 : iUpper + 1;
        int iLower = i + 5;
        int iLowerNext = i == 5 ? 6 : iLower + 1;

        Vertex upper = vertices[iUpper];
        auto origin = vertices[iUpper].position - vertices[0].position;
        auto right = vertices[iUpperNext].position - vertices[iUpper].position;
        vertices[iUpper].normal = glm::normalize(glm::cross(right, origin));

        Vertex lower = vertices[iLower];
        origin = vertices[iLower].position - vertices[0].position;
        right = vertices[iLowerNext].position - vertices[iLower].position;
        vertices[iLower].normal = glm::normalize(glm::cross(right, origin));
    }
    vertices[11].normal = { 0.0f, -1.0f, 0.0f };
}
