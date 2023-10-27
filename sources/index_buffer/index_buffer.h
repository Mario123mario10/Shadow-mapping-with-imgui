#pragma once

#include <glad/glad.h>

#include <vector>

class IndexBuffer {
	unsigned int id;
	int size;
	unsigned int type;
	template<typename IndicesType>
	void setType();
public:
	IndexBuffer();
	template<typename T>
	void parseElementData(const std::vector<T>& indices);
	int getSize() const;
	unsigned getType() const;
	void use() const;
	void unUse() const;
};

template<typename T>
void IndexBuffer::parseElementData(const std::vector<T>& indices) {
	setType<T>();
	size = indices.size() * sizeof(T);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices.data(), GL_STATIC_DRAW);
}