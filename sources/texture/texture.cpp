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

Texture2D::Texture2D(int width, int height, int internalFormat) : 
    Texture(GL_TEXTURE_2D), width(width), height(height), internalFormat(internalFormat) {
    glBindTexture(GL_TEXTURE_2D, id);
    // Add subvariants if necessary
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);

}

TextureCubeMap::TextureCubeMap() : Texture(GL_TEXTURE_CUBE_MAP) {

}