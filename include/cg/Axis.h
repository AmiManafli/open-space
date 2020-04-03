#ifndef CG1_PROJECT_AXIS_H
#define CG1_PROJECT_AXIS_H

#include <cg/GLHeader.h>
#include <cg/shaders/ShaderProgram.h>

class Axis {
public:
    Axis(glm::vec3 position);
    Axis(glm::vec3 position, glm::mat4 rotation);
    Axis(glm::vec3 position, glm::mat4 rotation, float scale);

    bool isVisible() { return visible; }
    void setVisible(bool visible) { this->visible = visible; }

    void draw(ShaderProgram& shaderProgram);

private:
    uint32_t vao, vbo;
    std::vector<glm::vec3> vertices;

    glm::vec3 position;
    glm::mat4 rotation;

    glm::mat4 model;

    bool visible = true;
    float scale;
};


#endif //CG1_PROJECT_AXIS_H
