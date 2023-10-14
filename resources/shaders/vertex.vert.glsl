#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;
layout (location = 2) in vec3 normal;
layout (location = 3) in mat4 model;

out vec4 fragColor;

uniform mat4 PVM;
uniform mat4 PV;

void main() {
    gl_Position = PV * model * vec4(position, 1.0);
    fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}