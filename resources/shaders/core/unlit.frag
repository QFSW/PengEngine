#version 330 core

in vec2 tex_coord;

out vec4 frag_color;

uniform vec4 base_color = vec4(1);
uniform sampler2D color_tex;

void main()
{
	frag_color = texture(color_tex, tex_coord) * base_color;
}