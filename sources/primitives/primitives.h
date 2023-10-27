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

struct AABB {
	glm::vec3 firstPoint;
	glm::vec3 secondPoint;
};

namespace DefaultGeometry {
	const std::vector<Vertex2D> screenVertices = {
		{ {-1.0f, -1.0f},   {0.0f, 0.0f} },
		{ { 1.0f, -1.0f},   {1.0f, 0.0f} },
		{ { 1.0f,  1.0f},   {1.0f, 1.0f} },
		{ {-1.0f,  1.0f},   {0.0f, 1.0f} },
	};

	const std::vector<uint8_t> screenIndices = {
		0, 1, 2,
		2, 3, 0
	};
}