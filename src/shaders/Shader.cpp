#include <cg/shaders/Shader.h>
#include <fstream>

static std::string readFile(const std::string &filename) {
    std::ifstream ifs(filename);
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));
    return content;
}

Shader::Shader(const std::string filename, GLenum type) : type(type) {
    shader = loadShader(filename);
}

unsigned int Shader::loadShader(const std::string& filename) {
    unsigned int shader = glCreateShader(type);
    auto source = readFile(filename);
    const char *sourceCode = source.c_str();

    glShaderSource(shader, 1, &sourceCode, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        throw std::runtime_error(infoLog);
    }

    return shader;
}

Shader::~Shader() {
    glDeleteShader(shader);
}
