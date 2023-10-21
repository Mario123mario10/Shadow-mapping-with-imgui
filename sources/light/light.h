#pragma once
// glm
#include <glm/glm.hpp>
// our libraries
#include <shader.h>
// stl
#include <unordered_map>
#include <string>


class Light {
private:
	Shader shader;
public:
	Light(Shader s) : shader(s) {};
	~Light() default;
	template<typename T>
	void setValues(const std::unordered_map<std::string& uniformName, T& uniformVariable> &values) const;
};