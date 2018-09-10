#version 460 

uniform mat4 perspective_mat;
uniform mat4 modelview_mat;
uniform mat4 object_mat;
in vec4 position;
in vec2 texcoord;

out vec2 texture_coordinate;

void main(){

	gl_Position=perspective_mat*modelview_mat*object_mat*position;
	texture_coordinate=texcoord;
}
