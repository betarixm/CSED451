#version 330 core

#define NUM_DIR_LIGHT 1
#define NUM_PNT_LIGHT 2

struct Material {
    sampler2D diffuse;
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

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vTex;
layout (location = 3) in vec3 vTangent;

//out vec4 vertexColor;
//out vec4 FragPos;
//out vec3 Normal;
//out vec2 TexCoords;
out vec4 Color; // linked to Fragment Shader

uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 color;
uniform mat4 View;

uniform vec3 viewPos;
uniform DirLight directionalLight[NUM_DIR_LIGHT];
uniform PntLight pointLight[NUM_PNT_LIGHT];
uniform Material material;

// for normal mapping
uniform sampler2D normalTexture;
uniform bool isNormap;


vec3 renderDirectionalLight(DirLight light, vec3 normal, vec3 viewDir, vec2 TexCoords);
vec3 renderPointLight(PntLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 TexCoords);

void main(){

    vec3 norm = normalize(mat3(transpose(inverse(ModelView))) * vNormal);
    vec4 FragPos = ModelView * vec4(vPosition, 1.0);
    vec3 FragPos3v = vec3(FragPos) / FragPos[3];
    vec2 TexCoords = vec2(vTex[0], vTex[1]);

    gl_Position = Projection * View * FragPos;


    vec3 viewDir = normalize(viewPos - FragPos3v);
    vec3 result = vec3(0, 0, 0);


    for(int i = 0; i < NUM_DIR_LIGHT; i++) {
        result += renderDirectionalLight(directionalLight[i], norm, viewDir, TexCoords);
    }

    for(int i = 0; i < NUM_PNT_LIGHT; i++) {
        result += renderPointLight(pointLight[i], norm, FragPos3v, viewDir, TexCoords);
    }

    Color = vec4(result, 1.0f);
}



vec3 renderDirectionalLight(DirLight light, vec3 normal, vec3 viewDir, vec2 TexCoords)
{
    vec3 dirLight = normalize(-light.direction);
    vec3 dirReflect = reflect(-dirLight, normal);

    float wDiffuse = max(dot(normal, dirLight), 0.0);
    float wSpecular = pow(max(dot(viewDir, dirReflect), 0.0), material.shininess);

    vec3 lAmbient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    //vec3 lAmbient = light.ambient * material.ambient;
    vec3 lDiffuse = light.diffuse * vec3(texture(material.diffuse, TexCoords));
    //vec3 lDiffuse = light.diffuse * material.diffuse;
    //vec3 lSpecular = light.specular * vec3(texture(material.specular, TexCoords));
    vec3 lSpecular = light.specular * material.specular;

    mat3 l = mat3(lAmbient, lDiffuse, lSpecular);
    vec3 weights = vec3(1, wDiffuse, wSpecular);

    return l * weights;
}

vec3 renderPointLight(PntLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 TexCoords) {
    vec3 dirLight = normalize(light.position - fragPos);

    float wDiffuse = max(dot(normal, dirLight), 0.0);

    vec3 dirReflect = reflect(-dirLight, normal);
    float wSpecular = pow(max(dot(viewDir, dirReflect), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / dot(vec3(light.constant, light.linear, light.quadratic), vec3(1, distance, distance * distance));

    vec3 lAmbient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 lDiffuse = light.diffuse * vec3(texture(material.diffuse, TexCoords));
    vec3 lSpecular = light.specular * material.specular;

    mat3 l = mat3(lAmbient, lDiffuse, lSpecular);
    vec3 weights = vec3(1, wDiffuse, wSpecular);

    return attenuation * (l * weights);
}