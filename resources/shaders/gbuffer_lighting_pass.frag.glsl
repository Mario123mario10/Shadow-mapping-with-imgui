#version 330 core

in vec2 texCoords;

out vec4 outColor;

struct Light {
    vec3 position;  
  
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
}; 

uniform vec3 lightColor;
uniform vec3 viewPos;
uniform Light light;

uniform int numSamples;
uniform sampler2DMS positionTexture;
uniform sampler2DMS normalTexture;
uniform sampler2DMS albedoTexture;

float gamma = 2.2;
float exposure = 0.75;

vec3 applyGammaCorrection(vec3 color) {
    return pow(color, vec3(1.0/gamma));
}

vec3 applyHDR(vec3 color) {
    return vec3(1.0) - exp(-color * exposure);
}


void main() {
    vec3 fragColor, fragPos, norm;
    int numNormals = 0;
    for(int i = 0; i < numSamples; i++){
        fragColor += texelFetch(albedoTexture, ivec2(gl_FragCoord.xy), i).rgb;
        norm += texelFetch(normalTexture, ivec2(gl_FragCoord.xy), i).rgb;
        fragPos += texelFetch(positionTexture, ivec2(gl_FragCoord.xy), i).rgb;
    }
    fragColor /= numSamples;
    fragPos /= numSamples;
    norm /= numSamples;

    if(norm.x == 0.0 && norm.y == 0.0 && norm.z == 0.0) {
        outColor = vec4(fragColor, 1.0);
    } else {
        norm = normalize(norm);

        // ambient
        float ambientStrength = 0.25;
        vec3 ambient = ambientStrength * lightColor;

        // diffuse
        vec3 lightDir = normalize(light.position - fragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        // specular
        float specularStrength = 3.0;
        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);

        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColor;

        // attentuation
        float distance = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 

        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;

        // final light
        outColor = vec4(fragColor * (ambient + diffuse + specular), 1.0);
    }
    outColor = vec4(applyGammaCorrection(applyHDR(outColor.xyz)), 1.0f);
}