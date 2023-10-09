#pragma once

#include <vertex_buffer.h>

class VertexArray {
	unsigned int id;
public:
	VertexArray();
	~VertexArray();
	void addLayout(VertexBufferLayout& layout);
	void use() const;
	void unUse() const;
};