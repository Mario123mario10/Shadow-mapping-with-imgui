#pragma once
#include <vector>

#include <primitives.h>

struct VertexBufferElement {
    unsigned int type;
    unsigned int typeSize;
    int count;
    bool normalized;
    bool instanced;
};

class VertexBufferLayout {
    std::vector<VertexBufferElement> elements;
    int stride;
public:
    VertexBufferLayout() : stride(0) {};
    const std::vector<VertexBufferElement>& getLayoutElements() const;
    template<typename T>
    void addLayoutElement(int count = 1, bool isntanced = false);
    int getStride() const;
};

class VertexBuffer {
    unsigned int id;
    VertexBufferLayout layout;
public:
    VertexBuffer();
    ~VertexBuffer();
    void parseRawData(int size, const void* data);
    template<typename T>
    void parseData(const std::vector<T>& vertexData);
    VertexBufferLayout& getLayout();
    void use() const;
    void unUse() const;
};

template<typename T>
void VertexBuffer::parseData(const std::vector<T>& vertexData) {
    const unsigned int size = vertexData.size() * sizeof(T);
    glBufferData(GL_ARRAY_BUFFER, size, vertexData.data(), GL_STATIC_DRAW);
}