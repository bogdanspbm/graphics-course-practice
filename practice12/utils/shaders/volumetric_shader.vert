#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 bBoxMin;
uniform vec3 bBoxMax;

layout (location = 0) in vec3 in_position;

out vec3 position;

void main()
{
    position = bBoxMin + in_position * (bBoxMax - bBoxMin);
    gl_Position = projection * view * model * vec4(position, 1.0);
}