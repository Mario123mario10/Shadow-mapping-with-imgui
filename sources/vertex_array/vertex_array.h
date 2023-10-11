#pragma once

#include <vertex_buffer.h>

class VertexArray {
	unsigned int id;
public:
	VertexArray();
	~VertexArray();
	template<typename T>
	void setLayout();
	void use() const;
	void unUse() const;
};