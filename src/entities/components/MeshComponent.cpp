#include <cg/entities/components/MeshComponent.h>
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


MeshComponent::MeshComponent(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices,
                             std::vector<Texture>& textures, ShaderProgram *shaderProgram, GLenum mode)
                                : vertices(vertices), indices(indices), textures(textures),
                                  shaderProgram(shaderProgram), mode(mode), instances(1), indexed(!indices.empty()) {
    setupBuffers();
}

MeshComponent::~MeshComponent() {
    // TODO: Cleanup mesh buffers
}

void MeshComponent::createInstances(std::vector<glm::vec3>& transformations) {
    if (instances > 1) {
        throw std::runtime_error("instances already defined");
    }
    instances = transformations.size();

    glGenBuffers(1, &instanceVbo);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);
    glBufferData(GL_ARRAY_BUFFER, transformations.size() * sizeof(glm::vec3), transformations.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnableVertexAttribArray(AttributeInstances);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);
    glVertexAttribPointer(AttributeInstances, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(AttributeInstances, 1);

    printf("%d instance(s) created!\n", instances);
}

void MeshComponent::setupBuffers() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    if (indexed) {
        glGenBuffers(1, &ebo);
    }

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    if (indexed) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);
    }

    glEnableVertexAttribArray(AttributePosition);
    glVertexAttribPointer(AttributePosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

    glEnableVertexAttribArray(AttributeNormal);
    glVertexAttribPointer(AttributeNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));

    glEnableVertexAttribArray(AttributeTextureCoord);
    glVertexAttribPointer(AttributeTextureCoord, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, textureCoord));

    glEnableVertexAttribArray(AttributeHeight);
    glVertexAttribPointer(AttributeHeight, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, height));
}
