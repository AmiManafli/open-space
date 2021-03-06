#include "cg/shaders/ShaderProgram.h"
#include <filesystem>
#include <fstream>

static std::string readFile(const std::string &filename) {
    auto path = std::filesystem::absolute(filename);
    std::ifstream ifs(path);
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
    if (source.empty()) {
        throw std::runtime_error("empty source code for shader");
    }
    auto code = source.c_str();

    glShaderSource(shader, 1, &code, nullptr);
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

void ShaderProgram::setUniform(const std::string name, int value) const {
	glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void ShaderProgram::setUniform(const std::string name, float value) const {
    glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void ShaderProgram::setUniform(const std::string name, glm::vec3 value) const {
    glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, glm::value_ptr(value));
}

void ShaderProgram::setUniform(const std::string name, glm::mat4 value) const {
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}



