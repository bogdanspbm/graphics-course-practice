#version 330 core

layout (location = 0) out vec4 out_color;
in vec2 texcoord;

void main()
{
    out_color = vec4(texcoord, 0.0, 1.0);
}