#pragma once
// glm
#include <glm/glm.hpp>
// stl
#include <unordered_map>
#include <string>
#include <array>

#define NUM_FACES 6

class Light {
protected:
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 attenuation;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
public:
	Light();
	void setPosition(float x, float y, float z);
	void setPosition(const glm::vec3& pos);
	void setColor(float r, float g, float b);
	void setColor(const glm::vec3& col);
	void setAttenuation(float constantFactor, float linearFactor, float quadraticFactor);
	void setAttenuation(const glm::vec3& col);
	void setAmbient(const glm::vec3& ambient);
	void setAmbient(float x, float y, float z);
	void setDiffuse(const glm::vec3& diffuse);
	void setDiffuse(float x, float y, float z);
	void setSpecular(const glm::vec3& specular);
	void setSpecular(float x, float y, float z);
	const glm::vec3& getPosition() const;
	const glm::vec3& getColor() const;
	const glm::vec3& getAttenuation() const;
	const glm::vec3& getAmbient() const;
	const glm::vec3& getDiffuse() const;
	const glm::vec3& getSpecular() const;
	float getAttenuationConstantFactor() const;
	float getAttenuationLinearFactor() const;
	float getAttenuationQuadraticFactor() const;
};

class OmniDirectionalLight : public Light {
	std::array<glm::mat4, NUM_FACES> projectionMatrices;
public:
	// TODO for future work
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
	void setViewDirection(const glm::vec3& viewDir);
	const glm::vec3& getViewDirection() const;
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
	void setViewDirection(const glm::vec3& viewDir);
	const glm::vec3& getViewDirection() const;
};

class SpotLight : public PerspectiveLight {
	float innerCutOff;
	float outerCutOff;
public:
	SpotLight(float fovy, float aspectRatio, float zNear, float zFar);
	void setInnerCutOff(float cutoff);
	void setOuterCutOff(float cutoff);
	float getInnerCutOff() const;
	float getOuterCutOff() const;
};