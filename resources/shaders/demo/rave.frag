#version 330 core

in vec2 tex_coord;

out vec4 frag_color;

uniform vec4 base_color = vec4(1);
uniform sampler2D color_tex;
uniform float time;

float triangle(float x)
{
	return 1 - abs(mod(x, 2) - 1) / 2;
}

void main()
{
	float time_scaled = time * 3;
	vec4 rave_col = vec4(triangle(tex_coord.x + time_scaled), triangle(tex_coord.y + time_scaled), 1, 1);
	frag_color = texture(color_tex, tex_coord) * base_color * rave_col;
}