#version 330 core

in vec4 vertex_color;
in vec2 tex_coord;

out vec4 frag_color;

uniform sampler2D color_tex;

void main()
{
	frag_color = texture(color_tex, tex_coord) * vertex_color;
}