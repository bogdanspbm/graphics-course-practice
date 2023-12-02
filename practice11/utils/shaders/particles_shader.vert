#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in float in_rotation;
layout (location = 2) in float in_size;
layout (location = 3) in vec3 in_color;

out float size;
out float rotation;
out vec3 colors;

void main()
{
    gl_Position = vec4(in_position, 1.0);
    size = in_size;
    rotation = in_rotation;
    colors = in_color;
}