//
// Created by 최은수 on 2021/04/29.
//

/**  vertex shader */

in vec4 vPosition;
in vec4 color;


// to Fragment Shader
out vec4 vertexColor;

// Matrix from appl
uniform mat4 ModelView;
uniform mat4 Projection;

void main(){

gl_Position = Projection * ModelView * vPosition;
vertexColor = color;

}