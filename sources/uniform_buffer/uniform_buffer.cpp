#include "uniform_buffer.h"

unsigned int UniformBuffer::blockBindings = 0;

UniformBuffer::UniformBuffer(const std::string& blockName, const std::vector<Shader>& shaders) : blockBinding(blockBindings++) {
	int size = 0;
	glGenBuffers(1, &id);
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

}
