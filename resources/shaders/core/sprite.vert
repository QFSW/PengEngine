#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 2) in vec2 a_tex_coord;

out vec2 tex_coord;
out vec4 vertex_color;

uniform mat4 mvp_matrix = mat4(1);
uniform vec2 tex_scale = vec2(1);
uniform vec2 tex_offset = vec2(0);

void main()
{
    gl_Position = mvp_matrix * vec4(a_pos, 1.0);
    
    tex_coord = tex_offset + a_tex_coord * tex_scale;
    vertex_color = vec4(1);
}