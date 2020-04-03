#ifndef CG1_PROJECT_HIGHLIGHTCOMPONENT_H
#define CG1_PROJECT_HIGHLIGHTCOMPONENT_H

#include <cg/GLHeader.h>
#include <cg/entities/Component.h>
#include <cg/shaders/ShaderProgram.h>


class HighlightComponent : public Component {
public:
    explicit HighlightComponent(float scaleFactor, ShaderProgram *shaderProgram);

    glm::mat4 getModel(glm::mat4 translated);

    ShaderProgram *shaderProgram;
    float scaleFactor;
};

#endif //CG1_PROJECT_HIGHLIGHTCOMPONENT_H
