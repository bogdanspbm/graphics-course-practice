#version 330 core
in vec3 inputNormal;
in vec4 shadowPosition;
in vec2 texCoord;
in float depth;
in vec3 vertexPosition;

uniform vec3 lightPosition;

uniform float roughness;
uniform float glossiness;
uniform float opacity;

uniform float lightRange;

uniform float useDisplacementMap;

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

uniform vec3 inputAlbedo;
uniform vec3 inputAmbientLight;
uniform vec3 inputSunColor;
uniform vec3 inputSunDirection;
uniform vec3 inputViewDirection;
uniform vec3 inputViewPosition;


layout (location = 0) out vec4 outColor;

float gaussianBlur(sampler2D inputTexture, vec2 texCoord, float depth, float blurRadius) {
    vec4 color = vec4(0.0);
    float shadow = 0.0;
    float totalWeight = 0.0;
    vec2 resolution = vec2(textureSize(inputTexture, 0));

    // Iterate over a kernel based on the blur radius
    for (float i = -blurRadius; i <= blurRadius; i += 1.0) {
        for (float j = -blurRadius; j <= blurRadius; j += 1.0) {
            vec2 offset = vec2(i, j);
            float weight = exp(-(offset.x * offset.x + offset.y * offset.y) / (2.0 * blurRadius * blurRadius));

            color = texture(inputTexture, texCoord + offset / resolution);

            if (depth > color.r || depth < 0){
                shadow += weight;
            }

            totalWeight += weight;
        }
    }

    // Normalize the result
    return 1- (shadow / totalWeight);
}


void main()
{
    vec4 textureColor = texture(texture0, texCoord);

    vec3 shadowTextCoord = shadowPosition.xyz / shadowPosition.w;
    shadowTextCoord.xy = shadowTextCoord.xy * 0.5 + 0.5;
    float shadow = gaussianBlur(texture31, shadowTextCoord.xy, depth, 5);

    if(shadow < 0.01){
        discard;
    }

    vec2 resolution = vec2(textureSize(texture31, 0));

    if (shadowTextCoord.x < 0 || shadowTextCoord.y < 0){
        discard;
    }

    if (shadowTextCoord.x > resolution.x || shadowTextCoord.y > resolution.y){
        discard;
    }

    vec3 lightDirection = normalize(lightPosition-vertexPosition);

    vec3 reflectDir = reflect(-lightDirection, inputNormal);
    float spec = pow(max(dot(inputViewDirection, reflectDir), 0.0), glossiness);
    vec3 specular = spec * inputSunColor;

    float diff = max(dot(inputNormal, lightDirection), 0.0);
    vec3 diffuse =  diff * inputSunColor;

    float distanceToLight = sqrt(pow(lightPosition.x-vertexPosition.x, 2) + pow(lightPosition.y-vertexPosition.y, 2) + pow(lightPosition.z-vertexPosition.z, 2));

    float lightOpacity = (lightRange-distanceToLight) / lightRange;

    if (lightRange == 0){
        lightOpacity = 1;
    }


    if (lightOpacity < 0){
        discard;
    }


    vec3 result = (4 * (specular + diffuse) * lightOpacity * shadow + inputAmbientLight) / 3 * textureColor.xyz;

    float outOpacity = opacity;

    if (useDisplacementMap > 0.5f){
        vec4 displacementMap = texture(texture1, texCoord);
        outOpacity = displacementMap.x;
    }

    if (outOpacity == 0){
        discard;
    }


    outColor = vec4(result, 1);
}
