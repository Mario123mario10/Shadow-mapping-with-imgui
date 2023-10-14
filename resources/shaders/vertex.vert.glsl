#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;
layout (location = 2) in vec3 normal;
out vec3 fragColor;
out vec3 outNormal;
out vec3 fragPos;

uniform mat4 PVM;
uniform mat4 model;
uniform mat4 invModel;


void main() {
    gl_Position = PVM * vec4(position, 1.0);
    fragColor = vec3(1.0, 0.5, 0.31);
    fragPos = vec3(model * vec4(position, 1.0));
    outNormal = mat3(transpose(invModel)) * normal;
}