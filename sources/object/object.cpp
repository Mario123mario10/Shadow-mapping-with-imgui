#include "object.h"

void Object::renderObject() {
	for (unsigned int i = 0; i < textures.size(); i++) {
		textures[i]->activate(i);
	}
	vao.use();
	glDrawElements(GL_TRIANGLES, meshSize, type, 0);
}

void Object::addTexture(const Texture& texture) {
	textures.emplace_back(&texture);
}

template<typename T>
unsigned int Object::setType() const {
	static_assert(false, "Type of index can be: uint8_t, uint16_t, uint32_t");
}

template<>
unsigned int Object::setType<uint8_t>() const {
	return GL_UNSIGNED_BYTE;
}

template<>
unsigned int Object::setType<uint16_t>() const {
	return GL_UNSIGNED_SHORT;
}

template<>
unsigned int Object::setType<uint32_t>() const {
	return GL_UNSIGNED_INT;
}