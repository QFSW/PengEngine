#version 330 core

in vec4 vertex_color;
in vec2 tex_coord;

out vec4 frag_color;

uniform float feather = 0.05;

void main()
{
	vec2 pos = tex_coord * 2 - vec2(1, 1);
	float mag_sqr = pos.x * pos.x + pos.y * pos.y;

	if (mag_sqr < 1 - feather / 2)
	{
		frag_color = vertex_color;
	}
	else if (mag_sqr < 1 + feather / 2)
	{
		frag_color = mix(vertex_color, vec4(0, 0, 0, 0), (mag_sqr - (1 - feather / 2)) / feather);
	}
	else
	{
		frag_color = vec4(0, 0, 0, 0);
	}
}