#pragma once
#include <shader.h>
#include <vertex_array.h>
#include <vertex_buffer.h>
#include <index_buffer.h>
#include <primitives.h>
#include <texture.h>

#include <memory>

class ObjectInterface {
protected:
    VertexArray vao;
    std::shared_ptr<IndexBuffer> ibo;
    std::vector<std::shared_ptr<VertexBuffer>> vbos;
    std::vector<std::shared_ptr<Texture>> textures;
public:
    void addVertexBuffer(std::shared_ptr<VertexBuffer> vbo);
    void attachIndexBuffer(std::shared_ptr<IndexBuffer> ibo);
    void addTexture(std::shared_ptr<Texture> texture);
    virtual void render() =0;
    virtual ~ObjectInterface() = default;
private:
};

class Object : public ObjectInterface {
public:
    virtual void render() override;
};

class ObjectInstanced : public ObjectInterface {
    int amount;
public:
    ObjectInstanced(int amount) : amount(amount) {};
    virtual void render() override;
};
