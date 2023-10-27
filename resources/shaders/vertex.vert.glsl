#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;
layout (location = 2) in vec3 normal;
layout (location = 3) in mat4 ModelMat;

out vec3 fragPos;
out vec2 texCoords;
out vec3 outNormal;

out vec4 lightFragPos;

uniform mat4 PVM;
// uniform mat4 umodel;
uniform mat4 ProjViewMat;
uniform mat4 LightProjViewMat;

const mat4 BiasMat = mat4(
	0.5, 0, 0, 0,
	0, 0.5, 0, 0,
	0, 0, 0.5, 0,
	0.5, 0.5, 0.5, 1.0
);

void main() {
    fragPos = vec3(ModelMat * vec4(position, 1.0));
	lightFragPos = BiasMat * LightProjViewMat * vec4(fragPos, 1.0);
    outNormal = mat3(transpose(inverse(ModelMat))) * normal;
    texCoords = textureCoords;
    gl_Position = ProjViewMat * vec4(fragPos, 1.0);
}