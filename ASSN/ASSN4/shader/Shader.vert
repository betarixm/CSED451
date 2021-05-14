#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vTex;

out vec4 vertexColor;
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 color;

void main(){
    gl_Position = Projection * ModelView * vec4(vPosition, 1.0);

    FragPos = vec3(ModelView * vec4(vPosition, 1.0));
    Normal = mat3(transpose(inverse(ModelView))) * vNormal;
    TexCoords = vec2(vTex[0], vTex[1]);
    vertexColor = color;
}