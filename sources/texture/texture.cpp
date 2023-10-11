#include "texture.h"

#include <glad/glad.h>

Texture::Texture(unsigned int type) : type(type) {
	glGenTextures(1, &id);
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

Texture2D::Texture2D() : Texture(GL_TEXTURE_2D) {

}

TextureCubeMap::TextureCubeMap() : Texture(GL_TEXTURE_CUBE_MAP) {

}