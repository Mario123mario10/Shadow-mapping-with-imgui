#include "texture.h"

#include <glad/glad.h>

#include <iostream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

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

TextureCubeMap::TextureCubeMap(const std::vector<std::string>& textureFilenames) 
    : Texture(GL_TEXTURE_CUBE_MAP) {
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    enum {
        R = 1,
        RGB = 3,
        RGBA = 4
    };

    for (unsigned int i = 0; i < 6; i++) {
        int width, height, nrChannels;
        std::string texturePath = CUBEMAP_PATH + textureFilenames[i];
        unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);

        int internalFormat;
        int format;

        if (data) {
            switch (nrChannels)
            {
            case R:
                internalFormat = GL_RED;
                format = GL_RED;
                break;
            case RGB:
                internalFormat = GL_RGB8;
                format = GL_RGB;
                break;
            case RGBA:
                internalFormat = GL_RGBA8;
                format = GL_RGBA;
                break;
            }

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            std::cout << "Cubemap tex failed to load at path: " << textureFilenames[i] << std::endl;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
