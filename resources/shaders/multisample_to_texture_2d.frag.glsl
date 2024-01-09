#version 450

in vec2 texCoords;

uniform sampler2DMS planeTexture;
uniform int numSamples;

uniform float gamma;
uniform float exposure;

out vec4 color;

vec3 applyGammaCorrection(vec3 color) {
    return pow(color, vec3(1.0/gamma));
}

vec3 applyHDR(vec3 color) {
    return vec3(1.0) - exp(-color * exposure);
}

void main()
{  
    vec3 col;
    for(int i = 0; i < numSamples; i++)
        col += texelFetch(planeTexture, ivec2(gl_FragCoord.xy), i).rgb;
    col /= numSamples;
    color = vec4(applyGammaCorrection(applyHDR(col)), 1.0f);
}