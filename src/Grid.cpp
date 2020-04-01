#include "Grid.h"

Grid::Grid(int width, int height) {
    int startX = width / 2;
    int startZ = height / 2;
    for (int i = 0; i < width; i++) {
        float step = 0.01f;
        auto start = Vertex { step * glm::vec3(-startX + i, 0, -startZ) };
        auto end = Vertex { step * glm::vec3(-startX + i, 0, -startZ + height) };
        printf("(%.3f, %.3f, %.3f)  --->  (%.3f, %.3f, %.3f)\n", start.position.x, start.position.y, start.position.z, end.position.x, end.position.y, end.position.z);
        vertices.push_back(start);
        vertices.push_back(end);
    }
    for (int i = 0; i < height; i++) {
        float step = 0.01f;
        auto start = Vertex { step * glm::vec3(-startX, 0, -startZ + i) };
        auto end = Vertex { step * glm::vec3(-startX + width, 0, -startZ + i) };
        printf("(%.3f, %.3f, %.3f)  --->  (%.3f, %.3f, %.3f)\n", start.position.x, start.position.y, start.position.z, end.position.x, end.position.y, end.position.z);
        vertices.push_back(start);
        vertices.push_back(end);
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
}

void Grid::draw(ShaderProgram &shaderProgram) {
    shaderProgram.setUniform("model", model);
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, vertices.size());
    glBindVertexArray(0);
}
