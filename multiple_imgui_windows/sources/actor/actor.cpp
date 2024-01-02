#include "actor.h"

template<>
Geometry Actor::getComponent() {
	return geometry;
}

template<>
Lights Actor::getComponent() {
	return lights;
}

template<>
Textures Actor::getComponent() {
	return textures;
}

template<>
void Actor::attach(Geometry component) {
	geometry = component;
}

template<>
void Actor::add(std::shared_ptr<Light> component) {
	lights.push_back(component);
}

template<>
void Actor::add(std::shared_ptr<Texture> component) {
	textures.push_back(component);
}

void Actor::setPosition(float x, float y, float z) {
	position = { x, y, z };
}

void Actor::setRotation(float alpha, float beta) {
	rotation = { alpha, beta };
}

void Actor::setScale(float x, float y, float z) {
	scale = { x, y, z };
}

void GameObject::render() {
	for (unsigned int i = 0; i < textures.size(); i++) {
		textures[i]->activate(i);
	}
	renderGeometry();
}

void GameObject::renderGeometry() {
	geometry->renderGeometry();
}