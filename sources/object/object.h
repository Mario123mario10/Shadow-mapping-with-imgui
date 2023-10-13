#include <vertex_array.h>
#include <index_buffer.h>
#pragma once

#include <shader.h>
#include <vertex_array.h>
#include <index_buffer.h>
#include <primitives.h>
#include <texture.h>

#include <memory>

class Object {
    VertexArray vao;
    VertexBuffer vbo;
    IndexBuffer ibo;
    std::vector<std::shared_ptr<const Texture>> textures;
    int meshSize;
    unsigned int type;
public:
    template<typename VertexType, typename IndicesType>
    Object(const Mesh<VertexType, IndicesType>& mesh);
    void addTexture(const Texture& texture);
    void renderObject();
private:
    template<typename IndicesType>
    unsigned int setType() const;
};

template<typename VerticesType, typename IndicesType>
Object::Object(const Mesh<VerticesType, IndicesType>& mesh)
    : meshSize(mesh.indices.size()), type(setType<IndicesType>()) {
    vao.use();
    vbo.use();
    ibo.use();
    vbo.parseVertexData(mesh.vertices);
    ibo.parseElementData(mesh.indices);
    vao.setLayout<VerticesType>();
}
