#version 330 core

layout (location = 0) out vec3 outPosition;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec4 outAlbedo;

void main() {
    outNormal = vec3(0.0, 0.0, 0.0);
    outAlbedo = vec4(4.0, 4.0, 4.0, 1.0);
}