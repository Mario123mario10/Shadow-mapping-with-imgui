#pragma once
// glm
#include <glm/glm.hpp>
// stl
#include <unordered_map>
#include <string>


class Light {
protected:
	glm::vec3 position;
public:
	Light();
	void setPosition(float x, float y, float z);
	void setPosition(const glm::vec3& pos);
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