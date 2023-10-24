#pragma once
// glm
#include <glm/glm.hpp>
// stl
#include <unordered_map>
#include <string>


class Light {
protected:
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 attenuation;
public:
	Light();
	void setPosition(float x, float y, float z);
	void setPosition(const glm::vec3& pos);
	void setColor(float r, float g, float b);
	void setColor(const glm::vec3& col);
	void setAttenuation(float constantFactor, float linearFactor, float quadraticFactor);
	void setAttenuation(const glm::vec3& col);
	const glm::vec3& getPosition() const;
	const glm::vec3& getColor() const;
	const glm::vec3& getAttenuation() const;
	float getAttenuationConstantFactor() const;
	float getAttenuationLinearFactor() const;
	float getAttenuationQuadraticFactor() const;
};

class PerspectiveLight : public Light {
	glm::mat4 projection;
	glm::vec3 viewDirection;
public:
	PerspectiveLight(float fovy, float aspectRatio, float zNear, float zFar);
	const glm::mat4& getProjectionMatrix() const;
	glm::mat4 getViewMatrix() const;
	const glm::vec3& getPosition() const;
	void setViewDirection(float x, float y, float z);
};

class OrthographicLight : public Light {
	glm::mat4 projection;
	glm::vec3 viewDirection;
public:
	OrthographicLight(float width, float height, float zNear, float zFar);
	const glm::mat4& getProjectionMatrix() const;
	glm::mat4 getViewMatrix() const;
	const glm::vec3& getPosition() const;
	void setViewDirection(float x, float y, float z);
};