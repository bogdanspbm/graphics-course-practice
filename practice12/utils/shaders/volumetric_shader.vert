#version 330 core

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform vec3 bBoxMin;
uniform vec3 bBoxMax;

layout (location = 0) in vec3 in_position;

out vec3 position;

void main()
{
    vec3 lPosition = bBoxMin + in_position * (bBoxMax - bBoxMin);

    position = (model * vec4(lPosition, 1.0)).xyz;

    gl_Position = projection * view * vec4(position, 1.0);
}