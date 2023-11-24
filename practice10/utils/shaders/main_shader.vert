#version 330 core

uniform mat4 model;
uniform mat4 sunView;
uniform mat4 projection;
uniform mat4 view;
uniform float far;
uniform float near;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_tangent;
layout (location = 3) in vec2 in_texcoord;

out vec3 inputNormal;
out vec3 inputTangent;
out vec2 texCoord;
out vec4 shadowPosition;

void main()
{
    gl_Position = projection * view * model * vec4(in_position, 1.0);
    inputNormal = mat3(model) * in_normal;
    inputTangent = mat3(model) * in_tangent;
    shadowPosition =  projection *  sunView * model  * vec4(in_position, 1.0);
    texCoord = in_texcoord;
}