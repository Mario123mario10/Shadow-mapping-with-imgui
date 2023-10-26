#include "object.h"

void Object::render() {
	for (unsigned int i = 0; i < textures.size(); i++) {
		textures[i]->activate(i);
	}
	vao.use();
	glDrawElements(GL_TRIANGLES, ibo->getSize(), ibo->getType(), 0);
}

void Object::renderGeometry() {
	vao.use();
	glDrawElements(GL_TRIANGLES, ibo->getSize(), ibo->getType(), 0);
}

void ObjectInterface::addVertexBuffer(std::shared_ptr<VertexBuffer> vbo) {
	vbos.push_back(vbo);
	vao.use();
	vbo->use();
	vao.addVertexBufferLayout(vbo->getLayout());
	vao.unUse();
}

void ObjectInterface::attachIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) {
	ibo = indexBuffer;
	vao.use();
	ibo->use();
	vao.unUse();
}

void ObjectInstanced::render() {
	for (unsigned int i = 0; i < textures.size(); i++) {
		textures[i]->activate(i);
	}
	vao.use();
	glDrawElementsInstanced(GL_TRIANGLES, ibo->getSize(), ibo->getType(), 0, amount);
}

void ObjectInstanced::renderGeometry() {
	vao.use();
	glDrawElementsInstanced(GL_TRIANGLES, ibo->getSize(), ibo->getType(), 0, amount);
}

void ObjectInterface::addTexture(std::shared_ptr<Texture> texture) {
	textures.push_back(texture);
}
