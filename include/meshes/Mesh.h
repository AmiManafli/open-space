#ifndef CG1_PROJECT_MESH_H
#define CG1_PROJECT_MESH_H

#include <GLHeader.h>
#include "shaders/ShaderProgram.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoord;
};

struct Texture {
    uint32_t id;
    std::string type;
};

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures);
    ~Mesh();

    void draw(ShaderProgram& program);

private:
    uint32_t vao, vbo, ebo;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture> textures;

    glm::mat4 model, view, projection;

    void setup();
};


#endif //CG1_PROJECT_MESH_H
