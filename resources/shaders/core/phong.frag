#version 330 core

#pragma symbol SHADER_LIT

#define MAX_POINT_LIGHTS 4
#define MAX_DIRECTIONAL_LIGHTS 1

struct PointLight
{
	vec3 pos;
	vec3 color;
	vec3 ambient;
	float range;
	float max_strength;
};

struct DirectionalLight
{
	vec3 dir;
	vec3 color;
	vec3 ambient;
	float intensity;
};

in vec3 pos;
in vec3 normal;
in vec2 tex_coord;

out vec4 frag_color;

uniform vec4 base_color = vec4(1);
uniform sampler2D color_tex;

uniform vec3 view_pos = vec3(0);
uniform PointLight point_lights[MAX_POINT_LIGHTS];
uniform DirectionalLight directional_lights[MAX_DIRECTIONAL_LIGHTS];
uniform float specular_strength = 0.5;
uniform float shinyness = 32;

vec3 calc_diffuse(vec3 light_dir, vec3 light_col)
{
	float diffuse_amount = max(0, dot(light_dir, normal));
	vec3 diffuse_color = diffuse_amount * light_col;

	return diffuse_color;
}

vec3 calc_specular(vec3 light_dir, vec3 light_col)
{
	vec3 view_dir = normalize(view_pos - pos);
	vec3 reflect_dir = reflect(-light_dir, normal);

	float specular_amount = pow(max(dot(view_dir, reflect_dir), 0.0), shinyness);
	vec3 specular_color = specular_strength * specular_amount * light_col;

	return specular_color;
}

void main()
{
	vec4 obj_color = texture(color_tex, tex_coord) * base_color;
	vec3 lighting = vec3(0);

	for (int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		PointLight light = point_lights[i];

		vec3 light_dir = normalize(light.pos - pos);
		vec3 diffuse_color = calc_diffuse(light_dir, light.color);
		vec3 specular_color = calc_specular(light_dir, light.color);

		float light_dist = length(light.pos - pos);
		float attenuation = min(light.max_strength, light.range / (1 + light_dist * light_dist));

		lighting += attenuation * (light.ambient + diffuse_color + specular_color);
	}

	for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++)
	{
		DirectionalLight light = directional_lights[i];

		vec3 diffuse_color = calc_diffuse(light.dir, light.color);
		vec3 specular_color = calc_specular(light.dir, light.color);

		lighting += light.intensity * (light.ambient + diffuse_color + specular_color);
	}

	frag_color = obj_color * vec4(lighting, 1);
}