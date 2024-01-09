#pragma once

#include <glad/glad.h>

#include <initializer_list>
#include <vector>

class Framebuffer {
	unsigned int id;
	unsigned int readBuffer;
	std::vector<unsigned int> drawBuffers;
public:
	Framebuffer(std::vector<unsigned int>&& drawBuffers, unsigned int readBuffers = GL_NONE);
	~Framebuffer();
	template<typename T>
	void attach(const T& object, unsigned int attachment);
	void use() const;
	unsigned int getId() const;
	void isComplete();
};