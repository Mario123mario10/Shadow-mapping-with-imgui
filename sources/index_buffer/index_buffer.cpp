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

int IndexBuffer::getSize() const {
	return size;
}

unsigned IndexBuffer::getType() const {
	return type;
}

template<typename T>
void IndexBuffer::setType() {
	static_assert(false, "Type of index can be: uint8_t, uint16_t, uint32_t");
}

template<>
void IndexBuffer::setType<uint8_t>() {
	type = GL_UNSIGNED_BYTE;
}

template<>
void IndexBuffer::setType<uint16_t>() {
	type = GL_UNSIGNED_SHORT;
}

template<>
void IndexBuffer::setType<uint32_t>() {
	type = GL_UNSIGNED_INT;
}