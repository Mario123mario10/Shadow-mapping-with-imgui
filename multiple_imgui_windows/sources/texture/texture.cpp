#include "texture.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>
#include <array>
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
    glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, true);
}

int Texture2DMultisample::getSamplesNumber() const {
    return samples;
}

static void helperSetTextureFormat(unsigned int* format, unsigned int* internalFormat, int nrChannels) {
    enum {
        R = 1,
        RG = 2,
        RGB = 3,
        RGBA = 4
    };

    switch (nrChannels)
    {
    case R:
        *internalFormat = GL_RED;
        *format = GL_RED;
        break;
    case RG:
        *internalFormat = GL_RG8;
        *format = GL_RG;
        break;
    case RGB:
        *internalFormat = GL_RGB8;
        *format = GL_RGB;
        break;
    case RGBA:
        *internalFormat = GL_RGBA8;
        *format = GL_RGBA;
        break;
    }
}

Texture2D::Texture2D(int width, int height) : Texture(GL_TEXTURE_2D) {
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture2D::Texture2D(std::string_view filePath) : Texture(GL_TEXTURE_2D) {
    int nrChannels = {}, width = {}, height = {};
    unsigned int internalFormat = { GL_NONE }, format = { GL_NONE };

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filePath.data(), &width, &height, &nrChannels, 0);
    if (data)
    {
        helperSetTextureFormat(&format, &internalFormat, nrChannels);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        // alternative code
        //numMipMaps = static_cast<unsigned int>(glm::log2(static_cast<float>(width)));
        //glTexStorage2D(GL_TEXTURE_2D, numMipMaps, internalFormat, width, height);
        //glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to laod data from file " << filePath << std::endl;
    }
    stbi_set_flip_vertically_on_load(false);
    stbi_image_free(data);
}

TextureCubeMap::TextureCubeMap(const std::vector<std::string_view>& textureFilenames) 
    : Texture(GL_TEXTURE_CUBE_MAP) {
    glBindTexture(type, id);

    for (unsigned int i = 0; i < 6; i++) {
        int nrChannels = {}, width = {}, height = {};
        unsigned int internalFormat, format;

        unsigned char* data = stbi_load(textureFilenames[i].data(), &width, &height, &nrChannels, 0);

        if (data) {
            helperSetTextureFormat(&format, &internalFormat, nrChannels);
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

ShadowMap::ShadowMap(int width, int height, int internalFormat) : Texture(GL_TEXTURE_2D) {
    glBindTexture(type, id);
    const std::array<float, 4> border = { 1.0f, 0.0f, 0.0f, 0.0f };
    glBindTexture(type, id);
    glTexStorage2D(type, 1, internalFormat, width, height);
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(type, GL_TEXTURE_BORDER_COLOR, border.data());
    glTexParameteri(type, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(type, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
}

ShadowCubeMap::ShadowCubeMap(int width, int height, int internalFormat) : Texture(GL_TEXTURE_CUBE_MAP) {
    glBindTexture(type, id);
    glTexStorage2D(type, 1, internalFormat, width, height);
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(type, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(type, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
}