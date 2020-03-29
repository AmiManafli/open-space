#include "ShaderProgram.h"
#include <fstream>

static std::string readFile(const std::string &filename) {
    std::ifstream ifs(filename);
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));
    return content;
}

ShaderProgram::ShaderProgram() {
    program = glCreateProgram();
}

void ShaderProgram::use() {
    glUseProgram(program);
}

void ShaderProgram::attachShader(const std::string filename, ShaderType type) {
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

    glAttachShader(program, shader);

    glDeleteShader(shader);
}

void ShaderProgram::link() {
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        throw std::runtime_error(infoLog);
    }
}

void ShaderProgram::setUniform(const std::string name, glm::mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
