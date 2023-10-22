#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;
layout (location = 2) in vec3 normal;
layout (location = 3) in mat4 model;

out vec3 afragPos;
out vec2 atexCoords;
out vec3 aNormal;

uniform mat4 PVM;
uniform mat4 PV;

void main() {
    afragPos = vec3(model * vec4(position, 1.0));
    aNormal = mat3(transpose(inverse(model))) * normal;
    atexCoords = textureCoords;
    gl_Position = PV * vec4(afragPos, 1.0);
}