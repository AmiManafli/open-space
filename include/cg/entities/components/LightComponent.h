#ifndef CG1_PROJECT_LIGHTCOMPONENT_H
#define CG1_PROJECT_LIGHTCOMPONENT_H

#include <cg/GLHeader.h>
#include <cg/shaders/ShaderProgram.h>

class LightComponent {
public:
    enum Type {
        Direction,
        Point,
    };

    LightComponent(Type type, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    void setUniforms(ShaderProgram *shaderProgram, TransformComponent *transform);

    Type type;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

#endif //CG1_PROJECT_LIGHTCOMPONENT_H
