#version 330 core
in vec3 inputNormal;
in vec4 shadowPosition;
in vec2 texCoord;
in float depth;

uniform float roughness;
uniform float glossiness;

uniform sampler2D texture0;// ALBEDO
uniform sampler2D texture1;// NORMAL
uniform sampler2D texture2;// GLOSS
uniform sampler2D texture3;// ROUGH
uniform sampler2D texture4;// ALPHA
uniform sampler2D texture5;// SPECULAR
uniform sampler2D texture6;// BUMP_MAP
uniform sampler2D texture7;// DISPLACEMENT_MAP
uniform sampler2D texture8;
uniform sampler2D texture9;

uniform sampler2D texture31;// SHADOW_MAP

uniform vec3 inputAlbedo;
uniform vec3 inputAmbientLight;
uniform vec3 inputSunColor;
uniform vec3 inputSunDirection;
uniform vec3 inputViewDirection;
uniform vec3 inputViewPosition;


layout (location = 0) out vec4 outColor;



void main()
{
    vec4 textureColor = texture(texture0, texCoord);

    vec3 shadowTextCoord = shadowPosition.xyz / shadowPosition.w;
    shadowTextCoord = shadowTextCoord * 0.5 + 0.5;
    vec4 depthValue = texture(texture31, shadowTextCoord.xy);

    if (depth > depthValue.r + 0.01){
        textureColor.xyz = textureColor.xyz * 0.5;
    }

    outColor = textureColor;
}
