#version 330 core

layout (location = 0) out vec4 out_color;
in vec2 texcoord;
in vec3 particleColor;


uniform int enabledTextures[32];

uniform sampler2D texture0;// ALBEDO
uniform sampler2D texture1;// DISPLACEMENT_MAP
uniform sampler2D texture2;// GLOSS
uniform sampler2D texture3;// ROUGH
uniform sampler2D texture4;// ALPHA
uniform sampler2D texture5;// SPECULAR
uniform sampler2D texture6;// BUMP_MAP
uniform sampler2D texture7;// NORMAL
uniform sampler2D texture8;// REFLECTION
uniform sampler2D texture9;

uniform sampler2D texture31;// SHADOW_MAP

void main()
{
    float opacity = 1;
    if (enabledTextures[4] > 0.5){
        vec4 textColor = texture(texture4, texcoord);
        opacity = textColor.r;
        if (opacity <= 0.01){
            discard;
        }
    }

    out_color = vec4(particleColor, opacity);
}