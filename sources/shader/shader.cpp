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
    for (auto& uniform : uniformLocations) {
        uniform.second = glGetUniformLocation(program, uniform.first.c_str());
    }
}

Shader::~Shader() {
    glDeleteProgram(program);
}

void cutLastCharacter(std::string& word, uint8_t sign) {
    if (word[word.size() - 1] == sign)
        word = word.substr(0, word.size() - 1);
}

const static std::array<std::string, 37> typesDict = { 
     "float", "double", "int", "uint", "bool", "vec2", "vec3", "vec4", "ivec2", "ivec3", "ivec4",
     "bvec2", "bvec3", "bvec4", "mat2", "mat3", "mat4", "sampler1D", "sampler2D",
     "sampler3D", "samplerCube", "sampler1DShadow", "sampler2DShadow",
     "samplerCubeShadow", "mat2x2", "mat2x3", "mat2x4", "mat3x2", "mat3x3",
     "mat3x4", "mat4x2", "mat4x3", "mat4x4", "sampler2DRect", "samplerBuffer",
     "sampler2DMS", "sampler2DMSArray" 
};

void Shader::checkForStruct(std::string line, std::string& currentStruct) {
    std::stringstream ss;
    if (line.find("//") != std::string::npos)
        return;
    if (line.find("struct") != std::string::npos) {
        ss << line;
        while(line != "struct")
            ss >> line;
        ss >> line;
        cutLastCharacter(line, '{');
        structFields.insert({ line, {} });
        currentStruct = line;
    }
    else if (!currentStruct.empty() && line.find("}") != std::string::npos) {
        currentStruct.clear();
    }
    else if (!currentStruct.empty()) {
        ss << line;
        ss >> line;
        if (std::any_of(typesDict.cbegin(), typesDict.cend(), [&line](const std::string& type) { return type == line; })) {
            std::string type = line;
            ss >> line;
            cutLastCharacter(line, ';');
            structFields[currentStruct].emplace_back(type, line);
        }
    }
}

void Shader::checkForUniforms(std::string line) {
    std::stringstream ss;
    if (line.find("//") != std::string::npos)
        return;
    if (line.find("uniform") != std::string::npos) {
        ss << line;
        while (line != "uniform")
            ss >> line;
        ss >> line;
        std::string type = line;
        ss >> line;
        std::string variable = line;
        cutLastCharacter(variable, ';');
        if (std::none_of(typesDict.cbegin(), typesDict.cend(), [&type](const std::string& glslType) { return type == glslType; })) {
            const auto& structVars = structFields[type];
            for (const auto& var : structVars) {
                uniformLocations.insert({ variable + '.' + var.second, -1 });
            }
        }
        else {
            uniformLocations.insert({ variable, -1 });
        }
    }
}

std::string Shader::parseShader(const std::string& path) {
    std::fstream source(path);
    std::stringstream file;
    std::string line;
    std::string currentStruct;
    while (getline(source, line)) {
        file << line << '\n';
        checkForStruct(line, currentStruct);
        checkForUniforms(line);
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