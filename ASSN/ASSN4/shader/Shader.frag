#version 330 core

#define NUM_DIR_LIGHT 1
#define NUM_PNT_LIGHT 1

struct Material {
    vec3 ambient;
    // sampler2D ambient;
    vec3 diffuse;
    // sampler2D diffuse;
    vec3 specular;
    float shininess;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PntLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec4 vertexColor;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;
uniform DirLight directionalLight[NUM_DIR_LIGHT];
uniform PntLight pointLight[NUM_PNT_LIGHT];
uniform Material material;

vec3 renderDirectionalLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 renderPointLight(PntLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0, 0, 0);

    for(int i = 0; i < NUM_DIR_LIGHT; i++) {
        result += renderDirectionalLight(directionalLight[i], norm, viewDir);
    }

    for(int i = 0; i < NUM_PNT_LIGHT; i++) {
        result += renderPointLight(pointLight[i], norm, FragPos, viewDir);
    }

    FragColor = vec4(result, 1.0f);
}

vec3 renderDirectionalLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 dirLight = normalize(-light.direction);
    vec3 dirReflect = reflect(-dirLight, normal);

    float wDiffuse = max(dot(normal, dirLight), 0.0);
    float wSpecular = pow(max(dot(viewDir, dirReflect), 0.0), material.shininess);

    // vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 lAmbient = light.ambient * material.ambient;
    // vec3 diffuse = light.diffuse * vec3(texture(material.diffuse, TexCoords));
    vec3 lDiffuse = light.diffuse * material.diffuse;
    // vec3 specular = light.specular * vec3(texture(material.specular, TexCoords));
    vec3 lSpecular = light.specular * material.specular;

    mat3 l = mat3(lAmbient, lDiffuse, lSpecular);
    vec3 weights = vec3(1, wDiffuse, wSpecular);

    return l * weights;
}

vec3 renderPointLight(PntLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    float wDiffuse = max(dot(normal, lightDir), 0.0);

    vec3 dirReflect = reflect(-lightDir, normal);
    float wSpecular = pow(max(dot(viewDir, dirReflect), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / dot(vec3(light.constant, light.linear, light.quadratic), vec3(1, distance, distance * distance));

    vec3 lAmbient = light.ambient * material.diffuse;
    vec3 lDiffuse = light.diffuse * material.diffuse;
    vec3 lSpecular = light.specular * material.specular;

    mat3 l = mat3(lAmbient, lDiffuse, lSpecular);
    vec3 weights = vec3(1, wDiffuse, wSpecular);

    return attenuation * (l * weights);
}