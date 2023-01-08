#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_col;
layout(location = 2) in vec2 a_tex_coord;

out vec4 vertex_color;
out vec2 tex_coord;

uniform mat4 transform = mat4(1);
uniform vec2 tex_scale = vec2(1);
uniform float wobble_strength = 0.05;
uniform float wobble_freq = 20;
uniform float wobble_speed = 2;
uniform float time = 0;

void main()
{
    vec3 pos = a_pos;
    pos.xz *= 1 + wobble_strength * sin(time * wobble_speed + pos.y * wobble_freq);

    // TODO: we could remove so much of this boilerplate with an include system
    gl_Position = transform * vec4(pos, 1.0);
    vertex_color = vec4(a_col, 1);
    tex_coord = a_tex_coord * tex_scale;
}