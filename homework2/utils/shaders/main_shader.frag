#version 330 core
in vec3 inputNormal;
in vec4 shadowPosition;
in vec2 texCoord;
in float depth;

uniform float roughness;
uniform float glossiness;
uniform float opacity;

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

vec4 gaussianBlur(sampler2D inputTexture, vec2 texCoord, float blurRadius) {
    vec4 color = vec4(0.0);
    float totalWeight = 0.0;
    vec2 resolution = vec2(textureSize(inputTexture, 0));

    // Iterate over a kernel based on the blur radius
    for (float i = -blurRadius; i <= blurRadius; i += 1.0) {
        for (float j = -blurRadius; j <= blurRadius; j += 1.0) {
            vec2 offset = vec2(i, j);
            float weight = exp(-(offset.x * offset.x + offset.y * offset.y) / (2.0 * blurRadius * blurRadius));

            color += texture(inputTexture, texCoord + offset / resolution) * weight;
            totalWeight += weight;
        }
    }

    // Normalize the result
    return color / totalWeight;
}


void main()
{
    vec4 textureColor = texture(texture0, texCoord);

    vec3 shadowTextCoord = shadowPosition.xyz / shadowPosition.w;
    shadowTextCoord = shadowTextCoord * 0.5 + 0.5;
    vec4 depthValue = gaussianBlur(texture31, shadowTextCoord.xy, 5.f);

    vec3 reflectDir = reflect(-inputSunDirection, inputNormal);
    float spec = pow(max(dot(inputViewDirection, reflectDir), 0.0), glossiness);
    vec3 specular = spec * inputSunColor;

    float diff = max(dot(inputNormal, inputSunDirection), 0.0);
    vec3 diffuse =  diff * inputSunColor;

    float shadowCoef = 1;

    if (depth > depthValue.r + 0.01){
        shadowCoef = 0.25;
    }

    vec3 result = ((specular + diffuse) * shadowCoef + inputAmbientLight) / 3 * textureColor.xyz;

    outColor = vec4(result , opacity);
}
