#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <array>

class Shader {
public:
    struct Uniform {
        std::string name;
        unsigned int type;
        int size;
    };
    struct UniformBlock {
        unsigned int index;
        int size;
        std::vector<int> offsets;
    };
    using Attribute = Uniform;
private:
    static constexpr int BUFFER_SIZE = 32;
    std::vector<Uniform> uniforms;
    std::vector<Attribute> attributes;
    std::unordered_map<std::string, UniformBlock> uniformBlocks;
    std::unordered_map<std::string, int> uniformLocations;
    unsigned int program;
public:
    Shader(const std::string& vertexShaderPath, const std::string fragmentShaderPath, const std::string& geometryShaderPath = "");
    Shader(const std::string& computeShaderPath);
    ~Shader();
    unsigned int getProgram() const;
    const UniformBlock& getUniformBlock(const std::string& name);
    void use() const;
    void unUse() const;
    template<typename T>
    void modifyUniform(const std::string& name, const T& uniformVariable);
private:
    std::string parseShader(const std::string& path);
    unsigned int compileShader(const std::string& path, int shaderType);
    void createProgram(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryShaderPath);
    void createProgram(const std::string& computePath);
    bool linkValidateError();
};