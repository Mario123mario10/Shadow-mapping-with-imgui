#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "light.h"

#define UP_VECTOR glm::vec3(0.0f, 1.0f, 0.0f)

Light::Light() : position(0.0f, 0.0f, 0.0f) {

}

void Light::setPosition(float x, float y, float z) {
	position = { x, y, z };
}

void Light::setPosition(const glm::vec3& pos) {
	position = pos;
}

PerspectiveLight::PerspectiveLight(float fovy, float aspectRatio, float zNear, float zFar) 
	: Light(), projection(glm::perspective(fovy, aspectRatio, zNear, zFar)) {
}

const glm::mat4& PerspectiveLight::getProjectionMatrix() const {
	return projection;
}

glm::mat4 PerspectiveLight::getViewMatrix() const {
	return glm::lookAt(position, position + viewDirection, UP_VECTOR);
}

const glm::vec3& PerspectiveLight::getPosition() const {
	return position;
}

void PerspectiveLight::setViewDirection(float x, float y, float z) {
	viewDirection = { x, y, z };
}

OrthographicLight::OrthographicLight(float width, float height, float zNear, float zFar)
	: Light(), projection(glm::ortho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, zNear, zFar)) {

}

const glm::mat4& OrthographicLight::getProjectionMatrix() const {
	return projection;
}

glm::mat4 OrthographicLight::getViewMatrix() const {
	return glm::lookAt(position, position + viewDirection, UP_VECTOR);
}

const glm::vec3& OrthographicLight::getPosition() const {
	return position;
}

void OrthographicLight::setViewDirection(float x, float y, float z) {
	 viewDirection = { x, y, z };
}