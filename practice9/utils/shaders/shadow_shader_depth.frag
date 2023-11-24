#version 330 core
layout (location = 0) out vec4 out_color;

void main()
{
    float z = gl_FragCoord.z;
    out_color = vec4(z, z*z, 0, 0);
}