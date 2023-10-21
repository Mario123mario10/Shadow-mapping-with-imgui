#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;
layout (location = 2) in vec3 normal;
layout (location = 3) in mat4 model;

out vec3 fragPos;
out vec2 texCoords;
out vec3 outNormal;

uniform mat4 PVM;
// uniform mat4 umodel;
uniform mat4 PV;

void main() {
    fragPos = vec3(model * vec4(position, 1.0));
    outNormal = mat3(transpose(inverse(model))) * normal;
    texCoords = textureCoords;
    gl_Position = PV * vec4(fragPos, 1.0);
}