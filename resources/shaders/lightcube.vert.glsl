#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;
layout (location = 2) in vec3 normal;

uniform mat4 PVM;

void main() {
    gl_Position = PVM * vec4(position, 1.0);
}