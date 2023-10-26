#version 330 core

in vec3 fragPos;
in vec2 texCoords;
in vec3 outNormal;

in vec4 lightFragPos;

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

const int KELNER_SIZE = 9;  // size of offsets
const ivec2 offsets[] = ivec2[](
	ivec2(-1, 1), ivec2(0, 1), ivec2(1, 1),
	ivec2(-1, 0), ivec2(0, 0), ivec2(1, 0),
	ivec2(-1, -1), ivec2(0, -1), ivec2(1, -1)
);

uniform vec3 lightColor;
uniform vec3 viewPos;
uniform Light light;

uniform sampler2D diffuseTexture;
uniform sampler2DShadow shadowMap;

float calculateShadow() {
    vec3 lightFrag = lightFragPos.xyz / lightFragPos.w;
    if(lightFrag.z >= 1.0)
        return 1.0;
    float sum = 0.0;
    for(int i = 0; i < KELNER_SIZE; i++) {
        sum += textureOffset(shadowMap, lightFrag.xyz, offsets[i]);
    }
    return sum / KELNER_SIZE;
}

void main() {
    vec3 fragColor = texture(diffuseTexture, texCoords).xyz;
    // ambient
    float ambientStrength = 0.25;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(outNormal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 3.0;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(norm, halfwayDir), 0.0), 128);
    vec3 specular = specularStrength * spec * lightColor;

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + (light.linear + light.quadratic * distance) * distance); 

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // final light
    outColor =  vec4(fragColor * (ambient + calculateShadow() * (diffuse + specular)), 1.0);
}