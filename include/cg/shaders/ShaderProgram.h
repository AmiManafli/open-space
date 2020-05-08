#ifndef CG1_PROJECT_SHADERPROGRAM_H
#define CG1_PROJECT_SHADERPROGRAM_H

#include <cg/GLHeader.h>

enum ShaderType {
    VertexShader = GL_VERTEX_SHADER,
    FragmentShader = GL_FRAGMENT_SHADER,
};

class ShaderProgram {
public:
    ShaderProgram();

    void attachShader(const std::string filename, ShaderType type);
    void link();

	void setUniform(const std::string name, int value) const;
    void setUniform(const std::string name, float value) const;
    void setUniform(const std::string name, glm::vec3 value) const;
    void setUniform(const std::string name, glm::mat4 value) const;

    void use();

private:
    uint32_t program;
};


#endif //CG1_PROJECT_SHADERPROGRAM_H
