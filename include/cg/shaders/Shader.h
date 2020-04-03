#ifndef CG1_PROJECT_SHADER_H
#define CG1_PROJECT_SHADER_H

#include <cg/GLHeader.h>

class Shader {
public:
    Shader(const std::string filename, GLenum type);
    ~Shader();

    uint32_t get() { return shader; }
    GLenum getTYpe() { return type; }

private:
    uint32_t shader;
    const GLenum type;

    uint32_t loadShader(const std::string& filename);
};

#endif //CG1_PROJECT_SHADER_H
