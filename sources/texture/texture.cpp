#include "texture.h"

#include <glad/glad.h>

#include <iostream>

Texture::Texture(unsigned int type) : type(type) {
	glGenTextures(1, &id);
}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

unsigned int Texture::getType() const {
	return type;
}

unsigned int Texture::getId() const {
	return id;
}

void Texture::activate(unsigned int slot)  const {
	glBindTextureUnit(slot, id);
}

Texture2DMultisample::Texture2DMultisample(int width, int height, int internalFormat, unsigned int samples)
    : Texture(GL_TEXTURE_2D_MULTISAMPLE), width(width), height(height), internalFormat(internalFormat), samples(samples) {
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
    if (samples < 2)
        std::cout << "Number of samples must be at least 2!!!" << std::endl;
    glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, true);
}

int Texture2DMultisample::getSamplesNumber() const {
    return samples;
}

Texture2D::Texture2D() : Texture(GL_TEXTURE_2D) {

}

TextureCubeMap::TextureCubeMap() : Texture(GL_TEXTURE_CUBE_MAP) {

}