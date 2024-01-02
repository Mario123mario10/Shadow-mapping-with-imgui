#include <glm/glm.hpp>

#include <object.h>
#include <light.h>

#include <memory>
#include <vector>

using Geometry = std::shared_ptr<ObjectInterface>;
using Textures = std::vector<std::shared_ptr<Texture>>;
using Lights = std::vector<std::shared_ptr<Light>>;
using ShaderActive = std::shared_ptr<Shader>;

class Actor {
protected:
	std::vector<std::shared_ptr<Texture>> textures;
	std::vector<std::shared_ptr<Light>> lights;
	std::shared_ptr<ObjectInterface> geometry;
	glm::vec3 position;
	glm::vec2 rotation;
	glm::vec3 scale;
public:
	virtual void render() = 0;
	virtual void renderGeometry() = 0;
	template<typename Type>
	Type getComponent();
	template<typename Type>
	void attach(Type component);
	template<typename Type>
	void add(Type component);
	void setPosition(float x, float y, float z);
	void setRotation(float alpha, float beta);
	void setScale(float x, float y, float z);
};

class GameObject : public Actor {
public:
	virtual void render() override;
	virtual void renderGeometry() override;
};