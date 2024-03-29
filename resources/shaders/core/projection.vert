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
uniform vec2 tex_offset = vec2(0);

void main()
{
    pos = vec3(model_matrix * vec4(a_pos, 1.0));
    gl_Position = view_matrix * vec4(pos, 1.0);
    
    // TODO: the normal_matrix should be constructed such that normalization isn't needed
    normal = normalize(normal_matrix * a_normal);
    tex_coord = tex_offset + a_tex_coord * tex_scale;
    vertex_color = vec4(a_col, 1);
}