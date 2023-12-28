#version 330 core

in vec3 fragPos;
in vec2 texCoords;
in vec3 outNormal;

const int maxNumLights = 9;
const int maxNumDirPointLights = 3;

in vec4 lightFragPos;

out vec4 outColor;

struct PointLight {
    int shadowIndex;

    vec3 position;  
  
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;

    vec3 color;
}; 

struct DirLight {
    int shadowIndex;

    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 color;
};

struct Spotlight {
    int shadowIndex;

    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 color;
};

const int KELNER_SIZE = 9;  // size of offsets
const ivec2 offsets[] = ivec2[](
	ivec2(-1, 1), ivec2(0, 1), ivec2(1, 1),
	ivec2(-1, 0), ivec2(0, 0), ivec2(1, 0),
	ivec2(-1, -1), ivec2(0, -1), ivec2(1, -1)
);

uniform vec3 lightColor;
uniform vec3 viewPos;

uniform int numPointLights;
uniform PointLight light;

uniform int numDirLights;
uniform DirLight dirLight;

uniform Spotlight spotlight;

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

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 128);
    vec3 ambient = light.ambient * lightColor;
    vec3 diffuse = light.diffuse * diff * lightColor;
    vec3 specular = light.specular * spec * lightColor;

    return (ambient + calculateShadow() * (diffuse + specular));
}

vec3 calcSpotlight(Spotlight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 128);
    // attenuation
    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist)); 
    // spotlight
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    vec3 diffuse = light.diffuse * diff * lightColor;
    vec3 specular = light.specular * spec * lightColor;
    
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // ambient
    float ambientStrength = 0.25;
    vec3 ambient = ambientStrength * lightColor;
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

     // specular
    float specularStrength = 3.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), 128);
    vec3 specular = specularStrength * spec * lightColor;

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + (light.linear + light.quadratic * distance) * distance); 

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + calculateShadow() * (diffuse + specular));
}

void main() {
    vec3 fragColor = texture(diffuseTexture, texCoords).xyz;

    vec3 norm = normalize(outNormal);
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 dirResult = calcDirLight(dirLight, norm, viewDir);
    vec3 spotResult = calcSpotlight(spotlight, norm, viewDir); 
    vec3 pointResult = calcPointLight(light, norm, viewDir);

    // final light
    outColor =  vec4(fragColor * (pointResult + spotResult), 1.0);
}