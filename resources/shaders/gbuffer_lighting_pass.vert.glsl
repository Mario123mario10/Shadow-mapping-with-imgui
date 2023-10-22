#version 450

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 textureCoords;

out vec2 texCoords;

void main(){
	gl_Position = vec4(position, -1.0f, 1.0f);
	texCoords = textureCoords;
}