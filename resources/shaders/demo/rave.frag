#version 330 core

#pragma symbol SHADER_LIT

in vec3 pos;
in vec3 normal;
in vec2 tex_coord;

out vec4 frag_color;

uniform vec4 base_color = vec4(1);
uniform sampler2D color_tex;
uniform float time;

uniform vec3 light_pos = vec3(0);
uniform vec3 light_color = vec3(1);

float triangle(float x)
{
	return 1 - abs(mod(x, 2) - 1) / 2;
}

void main()
{
	float time_scaled = time * 3;
	vec4 rave_col = vec4(triangle(tex_coord.x + time_scaled), triangle(tex_coord.y + time_scaled), 1, 1);
	vec4 obj_color = texture(color_tex, tex_coord) * base_color * rave_col;

	vec3 light_dir = normalize(light_pos - pos);
	float diffuse_amount = max(0, dot(light_dir, normal));
	vec3 diffuse_color = diffuse_amount * light_color;

	frag_color = obj_color * vec4(diffuse_color, 1);
}