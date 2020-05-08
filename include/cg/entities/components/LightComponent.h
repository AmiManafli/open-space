#ifndef CG1_PROJECT_LIGHTCOMPONENT_H
#define CG1_PROJECT_LIGHTCOMPONENT_H

#include <cg/GLHeader.h>
#include <cg/shaders/ShaderProgram.h>

class LightComponent : public Component {
public:
    enum Type {
        Direction,
        Point,
        Spot,
    };

    static LightComponent* createDirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    static LightComponent* createPointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);
//    static LightComponent* createSpotLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    LightComponent();

    void setUniforms(ShaderProgram *shaderProgram, TransformComponent *transform);

    Type type;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // Directional light
    glm::vec3 direction;

    // Point light
    float constant;
    float linear;
    float quadratic;
};

#endif //CG1_PROJECT_LIGHTCOMPONENT_H
