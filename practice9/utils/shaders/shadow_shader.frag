#version 330 core
in float depth;
layout (location = 0) out vec4 out_color;

void main()
{
    out_color = vec4(depth, depth*depth, 0, 0);
}