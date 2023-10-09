#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;
layout (location = 2) in vec3 normal;
out vec4 fragColor;

uniform mat4 PVM;

void main() {
    gl_Position = PVM * vec4(position, 1.0);
    fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}