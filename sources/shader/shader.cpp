#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <array>
#include <algorithm>
#include <numeric>

#include "shader.h"

Shader::Shader(const std::string& vertexShaderPath, const std::string fragmentShaderPath, const std::string& geometryShaderPath) {
    createProgram(vertexShaderPath, fragmentShaderPath, geometryShaderPath);
  
    glUseProgram(program);

    int count;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
    uniforms.resize(count);

    std::array<char, BUFFER_SIZE> data;
    int length;
    for (unsigned int i = 0; i < count; i++) {
        auto& uniform = uniforms[i];
        glGetActiveUniform(program, i, BUFFER_SIZE, &length, &uniform.size, &uniform.type, data.data());
        uniform.name = std::string(data.data());
        uniformLocations.insert({ uniform.name, glGetUniformLocation(program, uniform.name.c_str()) });
    }

    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);
    attributes.resize(count);

    // TODO create helper function
    for (unsigned int i = 0; i < count; i++) {
        auto& attribute = attributes[i];
        glGetActiveUniform(program, i, BUFFER_SIZE, &length, &attribute.size, &attribute.type, data.data());
        attribute.name = std::string(data.data());
    }
}

Shader::~Shader() {
    glDeleteProgram(program);
}

std::string Shader::parseShader(const std::string& path) {
    std::fstream source(path);
    std::stringstream file;
    std::string line;
    std::string currentStruct;
    while (getline(source, line)) {
        file << line << '\n';
    }
    return file.str();
}

unsigned int Shader::compileShader(const std::string& path, int shaderType) {
    unsigned int id = glCreateShader(shaderType);
    const std::string vertexSource = parseShader(path);
    const char* sourcePointer = &vertexSource[0];
    glShaderSource(id, 1, &sourcePointer, nullptr);
    glCompileShader(id);
    int shaderCompiled;
    glGetShaderiv(id, GL_COMPILE_STATUS, &shaderCompiled);
    if (shaderCompiled == GL_FALSE) {
        std::cout << "Shader compilation error in file " << path << std::endl;
        int logLength;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
        char* message = new char[logLength];
        glGetShaderInfoLog(id, logLength, nullptr, message);
        std::cout << message << std::endl;
        delete[] message;
    }
    return id;
}

bool Shader::linkValidateError() {
    int programLinked;
    int programValidated;
    glGetProgramiv(program, GL_LINK_STATUS, &programLinked);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &programValidated);

    if (programLinked == GL_FALSE || programValidated == GL_FALSE) {
        int logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        char* message = new char[logLength];
        glGetProgramInfoLog(program, logLength, nullptr, message);
        std::cout << message << std::endl;
        delete[] message;
        return true;
    }
    return false;
}

void Shader::createProgram(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath) {
    unsigned int vertexShader = compileShader(vertexPath, GL_VERTEX_SHADER);
    unsigned int geometryShader = (geometryPath != "") ? compileShader(geometryPath, GL_GEOMETRY_SHADER) : 0;
    unsigned int fragmentShader = compileShader(fragmentPath, GL_FRAGMENT_SHADER);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    if (geometryPath != "")
        glAttachShader(program, geometryShader);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if (geometryPath != "")
        glDeleteShader(geometryShader);
    linkValidateError();
}

//template<typename T>
//void Shader::modifyUniform(const std::string& name, const T& uniformVariable) const {
//    static_assert(false, "Unrecognized uniform variable!");
//}

// #define uniform

template<>
void Shader::modifyUniform<glm::mat4>(const std::string& name, const glm::mat4& matrix) const {
#ifdef uniform
    glUniformMatrix4fv(uniformLocations[name], 1, GL_FALSE, glm::value_ptr(matrix));
#else
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
#endif 
}

template<>
void Shader::modifyUniform<glm::mat3>(const std::string& name, const glm::mat3& matrix) const {
#ifdef uniform
    glUniformMatrix3fv(uniformLocations[name], 1, GL_FALSE, glm::value_ptr(matrix));
#else
    glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
#endif 
}

template<>
void Shader::modifyUniform<glm::vec3>(const std::string& name, const glm::vec3& vector) const {
#ifdef uniform
    glUniform3f(uniformLocations[name], vector.x, vector.y, vector.z);
#else
    glUniform3f(glGetUniformLocation(program, name.c_str()), vector.x, vector.y, vector.z);
#endif
}

template<>
void Shader::modifyUniform<int>(const std::string& name, const int& number) const {
#ifdef uniform
    glUniform1i(uniformLocations[name], number);
#else
    glUniform1i(glGetUniformLocation(program, name.c_str()), number);
#endif
}

template<>
void Shader::modifyUniform<float>(const std::string& name, const float& number) const {
#ifdef uniform
    glUniform1f(uniformLocations[name], number);
#else
    glUniform1f(glGetUniformLocation(program, name.c_str()), number);
#endif
}

template<>
void Shader::modifyUniform<bool>(const std::string& name, const bool& term) const {
#ifdef uniform
    glUniform1i(uniformLocations[name], term);
#else
    glUniform1i(glGetUniformLocation(program, name.c_str()), term);
#endif
}

unsigned int Shader::getProgram() const {
    return program;
}

void Shader::use() const {
    glUseProgram(program);
}

void Shader::unUse() const {
    glUseProgram(0);
}