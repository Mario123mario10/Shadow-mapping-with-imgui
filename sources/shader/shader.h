#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <fstream>
#include <sstream>

class Shader {
    unsigned int program;
public:
    Shader(const std::string& vertexShaderPath, const std::string fragmentShaderPath, const std::string& geometryShaderPath = "");
    Shader(const std::string& computeShaderPath);
    ~Shader();
    unsigned int getProgram() const;
    void use() const;
    void unUse() const;
    template<typename T>
    void modifyUniform(const std::string& name, const T& uniformVariable) const;
private:
    std::string parseShader(const std::string& path);
    unsigned int compileShader(const std::string& path, int shaderType);
    void createProgram(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryShaderPath);
    void createProgram(const std::string& computePath);
    bool linkValidateError();
};