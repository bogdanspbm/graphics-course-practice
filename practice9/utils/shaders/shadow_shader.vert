#version 330 core
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float far;
uniform float near;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texcoord;
layout (location = 3) in vec3 in_color;

out vec3 fragColor;

void main()
{
    gl_Position = projection * view * model * vec4(in_position, 1.0);
    float depth = gl_Position.z; // Retrieve the depth value
    depth = (depth - near) / (far - near);

    fragColor = vec3(depth);
}