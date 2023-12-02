#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 camera_position;

layout (points) in;
layout (points, max_vertices = 1) out;

void main()
{
    vec3 center = gl_in[0].gl_Position.xyz;
    gl_Position = projection * view * model * vec4(center, 1.0);
    EmitVertex();
    EndPrimitive();
}