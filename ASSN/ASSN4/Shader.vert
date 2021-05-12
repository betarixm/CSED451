#version 330 core

layout (location = 0) in vec3 vPosition;
out vec4 vertexColor;

uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 color;

void main(){

gl_Position =   Projection * ModelView *vec4(vPosition, 1.0);
vertexColor = color;

}