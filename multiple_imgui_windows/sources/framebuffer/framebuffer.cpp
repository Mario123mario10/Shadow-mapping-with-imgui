#include "framebuffer.h"
#include <texture.h>
#include <renderbuffer.h>

#include <iostream>

Framebuffer::Framebuffer(std::vector<unsigned int>&& drawBuffers, unsigned int readBuffer)
	: readBuffer(readBuffer), drawBuffers(drawBuffers) {
	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glDrawBuffers(drawBuffers.size(), drawBuffers.data());
	glReadBuffer(readBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &id);
}

template<>
void Framebuffer::attach<RenderbufferInterface>(const RenderbufferInterface& rbo, unsigned int attachment) {
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rbo.getId());
}

template<>
void Framebuffer::attach<Texture>(const Texture& texture, unsigned int attachment) {
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, texture.getType(), texture.getId(), 0);
}

void Framebuffer::use() const {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
}

unsigned int Framebuffer::getId() const {
	return id;
}

void Framebuffer::isComplete() {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer incomplete!!!" << std::endl;
	else
		std::cout << "Framebuffer complete!!!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
