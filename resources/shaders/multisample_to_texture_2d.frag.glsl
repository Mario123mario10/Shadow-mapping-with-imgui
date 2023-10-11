#version 450

in vec2 texCoords;
uniform sampler2DMS planeTexture;
uniform int numSamples;

out vec4 color;

void main()
{  
    vec3 col;
    for(int i = 0; i < numSamples; i++)
        col += texelFetch(planeTexture, ivec2(gl_FragCoord.xy), i).rgb;
    col /= numSamples;
    color = vec4(col, 1.0f);
}