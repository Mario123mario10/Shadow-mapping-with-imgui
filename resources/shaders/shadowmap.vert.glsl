#version 450
layout (location = 0) in vec3 aPos;
layout (location = 3) in mat4 aModel;
 
uniform mat4 model;
uniform mat4 PV;
uniform bool instanced;

void main() {
    mat4 modelMat = (instanced) ? aModel : model;
    gl_Position = PV * modelMat * vec4(aPos, 1.0);
} 