#version 460

uniform sampler2D texmap;

in vec2 texture_coordinate;
out vec4 frag_color;

void main(){
	vec4 texel=texture(texmap, texture_coordinate);
	texel.a=1.0f;
	frag_color=texel;
}
