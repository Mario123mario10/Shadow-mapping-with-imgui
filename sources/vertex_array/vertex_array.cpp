#include <glad/glad.h>

#include "vertex_array.h"


VertexArray::VertexArray() : attribIndex(0) {
	glGenVertexArrays(1, &id);
}

VertexArray::~VertexArray() {
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &id);
}

void VertexArray::addVertexBufferLayout(const VertexBufferLayout& layout) {
	const std::vector<VertexBufferElement>& elements = layout.getLayoutElements();
	int stride = layout.getStride();
	int offset = {};
	unsigned int i, j = {};
	for (i = attribIndex; i < attribIndex + elements.size(); i++) {
		const auto& element = elements[j++];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, stride, (void*)offset);
		glVertexAttribDivisor(i, static_cast<unsigned int>(element.instanced));
		offset += element.count * element.typeSize;
	}
	attribIndex = i;
}

void VertexArray::use() const {
	glBindVertexArray(id);
}

void VertexArray::unUse() const {
	glBindVertexArray(0);
}