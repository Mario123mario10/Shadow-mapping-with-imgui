#version 330 core

in vec3 outNormal;
in vec3 fragPos;
in vec2 fragmentTextureCoordinates;

out vec4 outColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D cubeTexture;

void main() {
    vec3 fragColor = texture(cubeTexture, fragmentTextureCoordinates).xyz;
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