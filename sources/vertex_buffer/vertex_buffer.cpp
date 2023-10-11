#include <glad/glad.h>

#include "vertex_buffer.h"


template<typename T>
void VertexBufferLayout::addLayoutElement(int count) {
    static_assert(false, "Wrong type");
}

template<>
void VertexBufferLayout::addLayoutElement<float>(int count) {
    elements.push_back({ GL_FLOAT, sizeof(float), count, GL_FALSE });
    stride += count * sizeof(float);
}

template<>
void VertexBufferLayout::addLayoutElement<char>(int count) {
    elements.push_back({ GL_BYTE, sizeof(char), count, GL_TRUE });
    stride += count * sizeof(char);
}

const std::vector<VertexBufferElement>& VertexBufferLayout::getLayoutElements() const {
    return elements;
}

// do not use it
void VertexBuffer::parseRawData(int size, const void* data) {
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

int VertexBufferLayout::getStride() const {
    return stride;
}

VertexBuffer::VertexBuffer() {
    glGenBuffers(1, &id);
}

VertexBuffer::~VertexBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &id);
}

void VertexBuffer::use() const {
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::unUse() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}