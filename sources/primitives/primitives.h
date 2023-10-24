#pragma once

#include <glm/glm.hpp>

#include <vector>

struct Vertex2D {
	glm::vec2 position;
	glm::vec2 texCoords;
};

struct Vertex3D {
	glm::vec3 position;
	glm::vec2 texCoords;
	glm::vec3 normal;
};

struct Triangle3D {
	Vertex3D a;
	Vertex3D b;
	Vertex3D c;
};

struct Segment3D {
	Vertex3D a;
	Vertex3D b;
};

template<typename VerticesType, typename IndicesType>
struct Mesh {
	std::vector<VerticesType> vertices;
	std::vector<IndicesType> indices;
};

