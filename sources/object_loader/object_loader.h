#pragma once
#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <string_view>

#include "primitives.h"

template<typename T>
class ObjectLoader {
	std::vector<T> indices;
	std::vector<Vertex3D> vertices;
	const unsigned int type;
public:
	ObjectLoader(std::string_view filePath);
	unsigned int getType() const { return type; }
	const std::vector<T>& getIndices() const { return indices; }
	const std::vector<Vertex3D>& getVertices() const { return vertices; }
private:
	void init(std::string_view filePath);
};