#version 330 core

// in vec3 afragPos;
in vec3 atexCoords;
// in vec3 aNormal;

layout (location = 0) out vec3 outPosition;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec4 outAlbedo;

uniform samplerCube cubemap;

void main()
{    
    //outPosition = afragPos;
    //outNormal = vec3(0.0, 0.0, 0.0);
    outAlbedo = texture(cubemap, atexCoords);
}