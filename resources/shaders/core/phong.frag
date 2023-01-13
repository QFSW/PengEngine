#version 330 core

in vec3 pos;
in vec3 normal;
in vec2 tex_coord;

out vec4 frag_color;

uniform vec4 base_color = vec4(1);
uniform sampler2D color_tex;

uniform vec3 light_pos = vec3(0);
uniform vec3 light_color = vec3(1);

void main()
{
	vec4 obj_color = texture(color_tex, tex_coord) * base_color;

	vec3 light_dir = normalize(light_pos - pos);
	float diffuse_amount = max(0, dot(light_dir, normal));
	vec3 diffuse_color = diffuse_amount * light_color;

	// TODO: add ambient lighting
	// TODO: add specular lighting

	frag_color = obj_color * vec4(diffuse_color, 1);
}