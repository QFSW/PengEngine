#version 330 core

in vec2 tex_coord;
in vec4 vertex_color;

out vec4 frag_color;

uniform vec4 base_color = vec4(1);
uniform sampler2D color_tex;

void main()
{
	frag_color = texture(color_tex, tex_coord) * base_color * vertex_color;

	if (frag_color.w < 0.01)
	{
		discard;
	}
}