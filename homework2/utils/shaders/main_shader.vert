#version 330 core

uniform mat4 model;
uniform mat4 sunView;
uniform mat4 projection;
uniform mat4 view;
uniform float far;
uniform float near;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texcoord;

out vec3 inputNormal;
out vec2 texCoord;
out vec4 shadowPosition;
out float depth;

void main()
{
    shadowPosition =  projection *  sunView * model  * vec4(in_position, 1.0);
    gl_Position = projection * view * model * vec4(in_position, 1.0);
    inputNormal = normalize(mat3(model) * in_normal);
    texCoord = in_texcoord;
    depth = (shadowPosition.z - near)/(far-near);
}