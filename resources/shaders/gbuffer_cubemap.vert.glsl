#version 330 core
layout (location = 0) in vec3 position;

// out vec3 afragPos;
out vec3 atexCoords;
// out vec3 aNormal;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    atexCoords = position;
    // afragPos = position;
    gl_Position = (projection * view * vec4(position, 1.0)).xyww;
}