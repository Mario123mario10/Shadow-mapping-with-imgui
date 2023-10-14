#include <glad/glad.h>

#include "vertex_buffer.h"
#include <iostream>

template<typename T>
void VertexBufferLayout::addLayoutElement(int count, bool instanced) {
    static_assert(false, "Wrong type");
}

template<>
void VertexBufferLayout::addLayoutElement<char>(int count, bool instanced) {
    elements.push_back({ GL_BYTE, sizeof(char), count, GL_TRUE, instanced });
    stride += count * sizeof(char);
}

template<>
void VertexBufferLayout::addLayoutElement<float>(int count, bool instanced) {
    elements.push_back({ GL_FLOAT, sizeof(float), count, GL_FALSE, instanced });
    stride += count * sizeof(float);
}

template<>
void VertexBufferLayout::addLayoutElement<Vertex3D>(int count, bool instanced) {
    addLayoutElement<float>(3, instanced);
    addLayoutElement<float>(2, instanced);
    addLayoutElement<float>(3, instanced);
}                            

template<>
void VertexBufferLayout::addLayoutElement<Vertex2D>(int count, bool instanced) {
    addLayoutElement<float>(2, instanced);
    addLayoutElement<float>(2, instanced);
}

template<>
void VertexBufferLayout::addLayoutElement<glm::mat4>(int count, bool instanced) {
    addLayoutElement<float>(4, instanced);
    addLayoutElement<float>(4, instanced);
    addLayoutElement<float>(4, instanced);
    addLayoutElement<float>(4, instanced);
}

const std::vector<VertexBufferElement>& VertexBufferLayout::getLayoutElements() const {
    return elements;
}

VertexBufferLayout& VertexBuffer::getLayout() {
    return layout;
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
    std::cout << "DESTROYED" << std::endl;
    glDeleteBuffers(1, &id);
}

void VertexBuffer::use() const {
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::unUse() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}