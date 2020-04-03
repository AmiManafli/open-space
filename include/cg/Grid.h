#ifndef CG1_PROJECT_GRID_H
#define CG1_PROJECT_GRID_H

#include <cg/GLHeader.h>
#include <cg/shaders/ShaderProgram.h>
#include <cg/entities/components/MeshComponent.h>


class Grid {
public:
    Grid(int width, int height);

    void draw(ShaderProgram& shaderProgram);

private:
    uint32_t vao, vbo;
    glm::mat4 model;

    std::vector<MeshComponent::Vertex> vertices;

    bool showAxisLinesY = false;
};


#endif //CG1_PROJECT_GRID_H
