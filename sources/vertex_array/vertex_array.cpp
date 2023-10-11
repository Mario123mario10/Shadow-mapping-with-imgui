#include <glad/glad.h>

#include "vertex_array.h"


VertexArray::VertexArray() {
	glGenVertexArrays(1, &id);
}

VertexArray::~VertexArray() {
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &id);
}

template<typename T>
void VertexArray::setLayout() {
	static_assert(false, "VertexArray::addLayout invalid type");
}

void constituteVertexArray(const std::vector<VertexBufferElement>& elements, int stride) {
	int offset = {};
	for (int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, GL_FLOAT, GL_FALSE, stride, (void*)offset);
		offset += element.count * element.typeSize;
	}
}

template<>
void VertexArray::setLayout<Vertex3D>() {
	VertexBufferLayout layout;
	layout.addLayoutElement<float>(3);
	layout.addLayoutElement<float>(2);
	layout.addLayoutElement<float>(3);
	constituteVertexArray(layout.getLayoutElements(), layout.getStride());
}

template<>
void VertexArray::setLayout<Vertex2D>() {
	VertexBufferLayout layout;
	layout.addLayoutElement<float>(2);
	layout.addLayoutElement<float>(2);
	constituteVertexArray(layout.getLayoutElements(), layout.getStride());
}

void VertexArray::use() const {
	glBindVertexArray(id);
}

void VertexArray::unUse() const {
	glBindVertexArray(0);
}