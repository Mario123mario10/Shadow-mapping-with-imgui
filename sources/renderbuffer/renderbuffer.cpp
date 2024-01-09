#include <glad/glad.h>

#include "renderbuffer.h"

RenderbufferInterface::RenderbufferInterface() {
	glGenRenderbuffers(1, &id);
}

RenderbufferInterface::~RenderbufferInterface() {
	glDeleteRenderbuffers(1, &id);
}

unsigned int RenderbufferInterface::getId() const {
	return id;
}

Renderbuffer::Renderbuffer(int width, int height, unsigned int internalFormat)
	: RenderbufferInterface(), width(width), height(height) {
	glBindRenderbuffer(GL_RENDERBUFFER, id);
	glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
}

RenderbufferMultisample::RenderbufferMultisample(int width, int height, unsigned int internalFormat, unsigned int samples)
	: RenderbufferInterface(), width(width), height(height), samples(samples) {
	glBindRenderbuffer(GL_RENDERBUFFER, id);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalFormat, width, height);
}

