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

void Light::setColor(float r, float g, float b) {
	color = { r, g, b };
}

void Light::setColor(const glm::vec3& col) {
	color = col;
}

void Light::setAttenuation(const glm::vec3& atten) {
	attenuation = atten;
}

void Light::setAttenuation(float constantFactor, float linearFactor, float quadraticFactor) {
	attenuation = { constantFactor, linearFactor, quadraticFactor };
}

const glm::vec3& Light::getPosition() const {
	return position;
}

const glm::vec3& Light::getColor() const {
	return color;
}

const glm::vec3& Light::getAttenuation() const {
	return attenuation;
}

float Light::getAttenuationConstantFactor() const {
	return attenuation.x;
}

float Light::getAttenuationLinearFactor() const {
	return attenuation.y;
}

float Light::getAttenuationQuadraticFactor() const {
	return attenuation.z;
}

PerspectiveLight::PerspectiveLight(float fovy, float aspectRatio, float zNear, float zFar) 
	: Light() {
	projection = glm::perspective(fovy, aspectRatio, zNear, zFar);
}

const glm::mat4& Light::getProjectionMatrix() const {
	return projection;
}

glm::mat4 Light::getViewMatrix() const {
	return glm::lookAt(position, position + viewDirection, UP_VECTOR);
}

void Light::setViewDirection(float x, float y, float z) {
	viewDirection = { x, y, z };
}

OrthographicLight::OrthographicLight(float width, float height, float zNear, float zFar)
	: Light() {
	projection = glm::ortho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, zNear, zFar);
}

void Light::setViewDirection(const glm::vec3& direction) {
	viewDirection = direction;
}