#version 430 core

layout(location = 0) in vec3 a_pos;
layout(location = 2) in vec2 a_tex_coord;

struct SpriteInstanceData
{
    vec4 color;
    mat4 mvp_matrix;
    vec2 tex_scale;
    vec2 tex_offset;
};

layout (std140, binding = 0) readonly buffer sprite_instance_data
{
    SpriteInstanceData instance_data[];
};

out vec2 tex_coord;
out vec4 vertex_color;

void main()
{
    gl_Position = instance_data[gl_InstanceID].mvp_matrix * vec4(a_pos, 1.0);
    
    tex_coord = instance_data[gl_InstanceID].tex_offset + a_tex_coord * instance_data[gl_InstanceID].tex_scale;
    vertex_color = instance_data[gl_InstanceID].color;
}