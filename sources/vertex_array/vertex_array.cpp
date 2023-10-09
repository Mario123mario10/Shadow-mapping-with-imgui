#include <glad/glad.h>

#include "vertex_array.h"


VertexArray::VertexArray() {
	glGenVertexArrays(1, &id);
}

VertexArray::~VertexArray() {
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &id);
}

void VertexArray::addLayout(VertexBufferLayout& layout) {
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