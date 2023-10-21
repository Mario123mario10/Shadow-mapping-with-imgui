#include "texture.h"

#include <glad/glad.h>
#include "../../thirdparty/stb_image/include/stb_image/stb_image.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image/stb_image.h>

#include <iostream>
#include <memory>
#include <string>

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

// Assumed one texture level
Texture2D::Texture2D(int width, int height, int internalFormat, int textureLevel) : 
    Texture(GL_TEXTURE_2D), width(width), height(height), internalFormat(internalFormat) {
    glBindTexture(GL_TEXTURE_2D, id);
    // Add subvariants if necessary
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureStorage2D(GL_TEXTURE_2D, textureLevel, internalFormat, width, height);
}


//Texture2D::~Texture2D() {
//    stbi_image_free(textureImage.get());
//}




std::shared_ptr<Texture2D> Texture2D::fromImage(const std::string imagePath) {
    int textureWidth, textureHeight, nrChannels;
    unsigned char* loadedData = stbi_load(imagePath.c_str(), &textureWidth, &textureHeight, &nrChannels, 0);
    std::shared_ptr<unsigned char> loadedImageData(loadedData);
    std::shared_ptr<Texture2D> createdTexture = std::make_shared<Texture2D>(textureWidth, textureHeight, GL_R11F_G11F_B10F);
    createdTexture->setTextureImage(std::move(loadedImageData));
    return createdTexture;
}


void Texture2D::setTextureImage(std::shared_ptr<unsigned char> textureImage) {
    this->textureImage = textureImage;
    initialiseTexture();
}

void Texture2D::initialiseTexture() {
    if (this->textureImage)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->textureImage.get());
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    { std::cout << "Failed to load texture" << std::endl; }

}


TextureCubeMap::TextureCubeMap() : Texture(GL_TEXTURE_CUBE_MAP) {

}