#ifndef CG1_PROJECT_MESHCOMPONENT_H
#define CG1_PROJECT_MESHCOMPONENT_H

#include <GLHeader.h>

struct MeshVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoord;
};

struct MeshTexture {
    uint32_t id;
    std::string type;
};

struct MeshComponent {
    uint32_t vao, vbo, ebo;
    std::vector<MeshVertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<MeshTexture> textures;
};

std::vector<MeshComponent *> createMeshComponentsFromFile(std::string filename);

#endif //CG1_PROJECT_MESHCOMPONENT_H
