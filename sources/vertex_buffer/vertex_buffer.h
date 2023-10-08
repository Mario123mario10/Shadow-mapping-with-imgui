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
    VertexBufferLayout() = default;
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
    VertexBufferLayout parseVertexData(const std::vector<Vertex3D>& vertexData);
    VertexBufferLayout parseVertexData(const std::vector<Vertex2D>& vertexData);
    VertexBufferLayout parseVertexData(const std::vector<glm::vec3>& vertexData);
    void use() const;
    void unUse() const;
};