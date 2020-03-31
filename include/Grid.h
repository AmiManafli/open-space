#ifndef CG1_PROJECT_GRID_H
#define CG1_PROJECT_GRID_H

#include <GLHeader.h>
#include <meshes/Mesh.h>


class Grid {
public:
    Grid(int width, int height);

    void draw(ShaderProgram& shaderProgram);

private:
    uint32_t vao, vbo, ebo;
    glm::mat4 model;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};


#endif //CG1_PROJECT_GRID_H
