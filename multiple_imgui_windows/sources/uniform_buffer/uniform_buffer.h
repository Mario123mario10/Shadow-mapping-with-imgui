#include <glad/glad.h>

#include <shader.h>
#include <memory>

#include <vector>
#include <string>
#include <array>

class UniformBuffer {
	unsigned int id;
	static unsigned int blockBindings;
	unsigned int blockBinding;
	std::array<uint8_t, 1024> buffer;
public:
	UniformBuffer(const std::string& blockName, const std::vector<Shader>& shaders);

};