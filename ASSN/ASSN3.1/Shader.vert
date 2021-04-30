//
// Created by 최은수 on 2021/04/29.
//

/**  vertex shader */

in vec3 vPosition;


// to Fragment Shader
out vec4 vertexColor;

// Matrix from appl
uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 color;

void main(){

gl_Position = Projection * ModelView * vec4(vPosition, 1);
vertexColor = color;

}