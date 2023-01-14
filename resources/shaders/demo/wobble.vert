#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_tex_coord;
layout(location = 3) in vec3 a_col;

out vec3 pos;
out vec3 normal;
out vec2 tex_coord;
out vec4 vertex_color;

uniform mat4 model_matrix = mat4(1);
uniform mat3 normal_matrix = mat3(1);
uniform mat4 view_matrix = mat4(1);
uniform vec2 tex_scale = vec2(1);
uniform float wobble_strength = 0.05;
uniform float wobble_freq = 20;
uniform float wobble_speed = 2;
uniform float time = 0;

void main()
{
    vec3 pos_local = a_pos;
    pos_local.xz *= 1 + wobble_strength * sin(time * wobble_speed + pos_local.y * wobble_freq);

    // TODO: we could remove so much of this boilerplate with an include system
    pos = vec3(model_matrix * vec4(pos_local, 1.0));
    gl_Position = view_matrix * vec4(pos, 1.0);
    
    normal = normal_matrix * a_normal;
    tex_coord = a_tex_coord * tex_scale;
    vertex_color = vec4(a_col, 1);
}