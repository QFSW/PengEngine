#version 330 core

#pragma symbol SHADER_LIT

in vec3 pos;
in vec3 normal;
in vec2 tex_coord;

out vec4 frag_color;

uniform vec4 base_color = vec4(1);
uniform sampler2D color_tex;

uniform vec3 view_pos = vec3(0);
uniform vec3 light_pos = vec3(0);
uniform vec3 light_color = vec3(1);
uniform float specular_strength = 0.5;
uniform float shinyness = 32;

void main()
{
	vec4 obj_color = texture(color_tex, tex_coord) * base_color;

	vec3 light_dir = normalize(light_pos - pos);
	float diffuse_amount = max(0, dot(light_dir, normal));
	vec3 diffuse_color = diffuse_amount * light_color;

	vec3 view_dir = normalize(view_pos - pos);
	vec3 reflect_dir = reflect(-light_dir, normal);

	float specular_amount = pow(max(dot(view_dir, reflect_dir), 0.0), shinyness);
	vec3 specular_color = specular_strength * specular_amount * light_color;  

	// TODO: add ambient lighting

	frag_color = obj_color * vec4(diffuse_color + specular_color, 1);
}