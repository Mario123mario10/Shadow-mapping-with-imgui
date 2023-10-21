#include <glad/glad.h>
#include <glm/glm.hpp>

#include "light.h"


template<typename T>
void Light::setValues(const std::unordered_map<std::string& uniformName, T& uniformVariable> &values) const {
	for (auto& v : values) {
		shader.modifyUniform<T&>(v.first, v.second);
	}
};
