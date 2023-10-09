#include <glad/glad.h>

#include "index_buffer.h"

IndexBuffer::IndexBuffer() {
	glGenBuffers(1, &id);
}

void IndexBuffer::use() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBuffer::unUse() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}