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

void Light::setAmbient(const glm::vec3& amb) {
	ambient = amb;
}

void Light::setAmbient(float x, float y, float z) {
	ambient = { x, y, z };
}

void Light::setDiffuse(const glm::vec3& diff) {
	diffuse = diff;
}

void Light::setDiffuse(float x, float y, float z) {
	diffuse = { x, y, z };
}

void Light::setSpecular(const glm::vec3& spec) {
	specular = spec;
}

void Light::setSpecular(float x, float y, float z) {
	specular = { x, y, z };
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

const glm::vec3& Light::getAmbient() const {
	return ambient;
}

const glm::vec3& Light::getDiffuse() const {
	return diffuse;
}

const glm::vec3& Light::getSpecular() const {
	return specular;
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

void PerspectiveLight::setViewDirection(const glm::vec3& viewDir) {
	viewDirection = viewDir;
}

const glm::vec3& PerspectiveLight::getViewDirection() const {
	return viewDirection;
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

void OrthographicLight::setViewDirection(const glm::vec3& viewDir) {
	viewDirection = viewDir;
}

const glm::vec3& OrthographicLight::getViewDirection() const {
	return viewDirection;
}

SpotLight::SpotLight(float fovy, float aspectRatio, float zNear, float zFar)
	: PerspectiveLight(fovy, aspectRatio, zNear, zFar) {

}

void SpotLight::setInnerCutOff(float cutoff) {
	innerCutOff = cutoff;
}

void SpotLight::setOuterCutOff(float cutoff) {
	outerCutOff = cutoff;
}

float SpotLight::getInnerCutOff() const {
	return innerCutOff;
}

float SpotLight::getOuterCutOff() const {
	return outerCutOff;
}
