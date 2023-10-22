#version 330 core

in vec3 fragPos;
in vec2 texCoords;
in vec3 outNormal;
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

uniform sampler2D diffuseTexture;

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
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // final light
    outColor = vec4(fragColor * (ambient + diffuse + specular), 1.0);
}