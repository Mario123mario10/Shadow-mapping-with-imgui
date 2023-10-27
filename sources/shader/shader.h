#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

class Shader {
    std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> structFields;
    mutable std::unordered_map<std::string, int> uniformLocations;
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
    void checkForStruct(std::string line, std::string& currentStruct);
    void checkForUniforms(std::string line);
    unsigned int compileShader(const std::string& path, int shaderType);
    void createProgram(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryShaderPath);
    void createProgram(const std::string& computePath);
    bool linkValidateError();
};