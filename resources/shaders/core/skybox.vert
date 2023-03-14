#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 2) in vec2 a_tex_coord;

out vec2 tex_coord;

uniform mat4 view_matrix = mat4(1);

void main()
{
    vec4 pos = view_matrix * vec4(a_pos, 1.0);
    gl_Position = pos.xyww;
    
    tex_coord = a_tex_coord;
}