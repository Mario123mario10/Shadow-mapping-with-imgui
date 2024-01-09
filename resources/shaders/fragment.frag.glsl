#version 330 core

in vec3 fragPos;
in vec2 texCoords;
in vec3 outNormal;

const int maxNumLights = 9;
const int maxNumDirPointLights = 3;

in vec4 lightFragPos[maxNumLights];

out vec4 outColor;

struct PointLight {
    int shadowIndex;

    vec3 position;  
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
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
    float innerCutOff;
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

uniform bool flashlightOn;

uniform vec3 viewPos;

uniform int numPointLights;
uniform PointLight pointLight[maxNumDirPointLights];

uniform int numDirLights;
uniform DirLight dirLight[maxNumDirPointLights];

uniform Spotlight spotlight;

uniform sampler2D diffuseTexture;
uniform sampler2DShadow shadowMap[2];

float calculateShadow(int index) {
    vec3 lightFrag = lightFragPos[index].xyz / lightFragPos[index].w;
    if(lightFrag.z >= 1.0)
        return 1.0;
    float sum = 0.0;
    for(int i = 0; i < KELNER_SIZE; i++) {
        sum += textureOffset(shadowMap[index], lightFrag.xyz, offsets[i]);
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
    vec3 ambient = light.ambient * light.color;
    vec3 diffuse = light.diffuse * diff * light.color;
    vec3 specular = light.specular * spec * light.color;

    return (ambient + calculateShadow(light.shadowIndex) * (diffuse + specular));
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
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    vec3 diffuse = light.diffuse * diff * light.color;
    vec3 specular = light.specular * spec * light.color;
    
    return attenuation * intensity * (diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // ambient
    float ambientStrength = 0.25;
    vec3 ambient = ambientStrength * light.color;
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * light.color;

     // specular
    float specularStrength = 3.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), 128);
    vec3 specular = light.specular * spec * light.color;

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + (light.linear + light.quadratic * distance) * distance); 

    return attenuation * (ambient + calculateShadow(light.shadowIndex) * (diffuse + specular));
}

void main() {
    vec3 fragColor = texture(diffuseTexture, texCoords).xyz;

    vec3 norm = normalize(outNormal);

    vec3 result;
    vec3 viewDir = normalize(viewPos - fragPos);

    for(int i = 0; i < min(numDirLights, maxNumDirPointLights); i++) {
        vec3 lightDir = normalize(pointLight[i].position - fragPos);
        result += calcDirLight(dirLight[i], norm, viewDir);
    }

    for(int i = 0; i < min(numPointLights, maxNumDirPointLights); i++) {
        result += calcPointLight(pointLight[i], norm, viewDir);
    }

    if(flashlightOn) {
        result += calcSpotlight(spotlight, norm, viewDir);
    }

    // final light
    outColor =  vec4(fragColor * result, 1.0);
}