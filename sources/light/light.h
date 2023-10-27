#pragma once
// glm
#include <glm/glm.hpp>
// stl
#include <unordered_map>
#include <string>


class Light {
protected:
	glm::mat4 projection;
	glm::vec3 viewDirection;
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

	glm::mat4 getViewMatrix() const;
	const glm::mat4& getProjectionMatrix() const;
	void setViewDirection(float x, float y, float z);
	void setViewDirection(const glm::vec3& direction);
};

class PerspectiveLight : public Light {
public:
	PerspectiveLight(float fovy, float aspectRatio, float zNear, float zFar);

};

class OrthographicLight : public Light {
public:
	OrthographicLight(float width, float height, float zNear, float zFar);
	
};