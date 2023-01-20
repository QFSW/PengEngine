#version 330 core

in vec2 tex_coord;

out vec4 frag_color;

uniform vec3 base_color = vec3(1);
uniform sampler2D color_tex;

void main()
{
	frag_color = texture(color_tex, tex_coord) * vec4(base_color, 1);
}