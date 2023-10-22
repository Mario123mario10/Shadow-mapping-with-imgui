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

template<typename T>
void Framebuffer::attach(const T& object, unsigned int attachment) {
	static_assert(false, "Unrecognized type for shadow!");
}

template<>
void Framebuffer::attach<RenderbufferInterface>(const RenderbufferInterface& rbo, unsigned int attachment) {
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rbo.getId());
}

template<>
void Framebuffer::attach<Renderbuffer>(const Renderbuffer& rbo, unsigned int attachment) {
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rbo.getId());
}

template<>
void Framebuffer::attach<RenderbufferMultisample>(const RenderbufferMultisample& rbo, unsigned int attachment) {
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rbo.getId());
}

template<>
void Framebuffer::attach<Texture2D>(const Texture2D& texture, unsigned int attachment) {
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, texture.getType(), texture.getId(), 0);
}

template<>
void Framebuffer::attach<Texture2DMultisample>(const Texture2DMultisample& texture, unsigned int attachment) {
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
