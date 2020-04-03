#include <cg/Axis.h>

Axis::Axis(glm::vec3 position) : Axis(position, glm::mat4()) {
}

Axis::Axis(glm::vec3 position, glm::mat4 rotation) : Axis(position, rotation, 1.0f) {
}

Axis::Axis(glm::vec3 position, glm::mat4 rotation, float scale)
        : position(position), rotation(rotation), scale(scale) {
    // Add axis lines
    vertices.emplace_back(glm::vec3(0, 0, 0));
    vertices.emplace_back(glm::vec3(2, 0, 0));
    vertices.emplace_back(glm::vec3(0, 0, 0));
    vertices.emplace_back(glm::vec3(0, 2, 0));
    vertices.emplace_back(glm::vec3(0, 0, 0));
    vertices.emplace_back(glm::vec3(0, 0, 2));

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

    auto translated = glm::translate(glm::mat4(), position);
    model = glm::scale(translated, glm::vec3(scale, scale, scale));
}

void Axis::draw(ShaderProgram &shaderProgram) {
    shaderProgram.use();
    shaderProgram.setUniform("model", model);
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, vertices.size());
    glBindVertexArray(0);
}
