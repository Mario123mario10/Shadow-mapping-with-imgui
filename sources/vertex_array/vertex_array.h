#pragma once

#include <vertex_buffer.h>

class VertexArray {
	unsigned int id;
	unsigned int attribIndex;
public:
	VertexArray();
	~VertexArray();
	void addVertexBufferLayout(const VertexBufferLayout& layout);
	void use() const;
	void unUse() const;
};