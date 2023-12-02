#version 330 core

layout (location = 0) out vec4 out_color;
in vec2 texcoord;

uniform int enabledTextures[32];

uniform sampler2D texture0;// ALBEDO
uniform sampler2D texture1;// DISPLACEMENT_MAP
uniform sampler2D texture2;// GLOSS
uniform sampler2D texture3;// ROUGH
uniform sampler2D texture4;// ALPHA
uniform sampler2D texture5;// SPECULAR
uniform sampler2D texture6;// BUMP_MAP
uniform sampler2D texture7;// NORMAL
uniform sampler2D texture8;
uniform sampler2D texture9;

uniform sampler2D texture31;// SHADOW_MAP

void main()
{
    out_color = vec4(texcoord, 0.0, 1.0);
}