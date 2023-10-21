#version 330 core

in vec3 outNormal;
in vec3 fragPos;
out vec4 outColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

struct Light {
    vec3 position;  
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

uniform Light light;


void main() {
    vec3 fragColor = 0.5 * vec3(3.0, 0.5, 0.31);
    // ambient
    float ambientStrength = 0.25;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(outNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 3.0;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // final light
    outColor = vec4(fragColor * (ambient + diffuse + specular), 1.0);
}