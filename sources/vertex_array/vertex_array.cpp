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

template<>
void VertexArray::setLayout<Vertex3D>() {
	VertexBufferLayout layout;
	layout.addLayoutElement<float>(3);
	layout.addLayoutElement<float>(2);
	layout.addLayoutElement<float>(3);

	const std::vector<VertexBufferElement> elements = layout.getLayoutElements();

	int offset = {};
	int stride = layout.getStride();
	for (int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, GL_FLOAT, GL_FALSE, stride, (void*)offset);
		offset += element.count * element.typeSize;
	}
}

void VertexArray::use() const {
	glBindVertexArray(id);
}

void VertexArray::unUse() const {
	glBindVertexArray(0);
}