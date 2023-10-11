#pragma once
#include <vector>

#include <primitives.h>

struct VertexBufferElement {
    unsigned int type;
    unsigned int typeSize;
    int count;
    bool normalized;
};

class VertexBufferLayout {
    std::vector<VertexBufferElement> elements;
    int stride;
public:
    VertexBufferLayout() : stride(0) {};
    const std::vector<VertexBufferElement>& getLayoutElements() const;
    template<typename T>
    void addLayoutElement(int count);
    int getStride() const;
};

class VertexBuffer {
    unsigned int id;
    std::vector<VertexBufferLayout> layouts;
public:
    VertexBuffer();
    ~VertexBuffer();
    void parseRawData(int size, const void* data);
    template<typename T>
    void parseVertexData(const std::vector<T>& vertexData);
    void use() const;
    void unUse() const;
};

template<typename T>
void VertexBuffer::parseVertexData(const std::vector<T>& vertexData) {
    const unsigned int size = vertexData.size() * sizeof(T);
    glBufferData(GL_ARRAY_BUFFER, size, &vertexData[0], GL_STATIC_DRAW);
}