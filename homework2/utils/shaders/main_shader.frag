#version 330 core
in vec3 inputNormal;
in vec4 shadowPosition;
in vec2 texCoord;
in float depth;

uniform float roughness;
uniform float glossiness;

uniform sampler2D texture0; // ALBEDO
uniform sampler2D texture1; // NORMAL
uniform sampler2D texture2; // GLOSS
uniform sampler2D texture3; // ROUGH
uniform sampler2D texture4; // ALPHA
uniform sampler2D texture5; // SPECULAR
uniform sampler2D texture6; // BUMP_MAP
uniform sampler2D texture7; // DISPLACEMENT_MAP
uniform sampler2D texture8;
uniform sampler2D texture9;

uniform sampler2D texture31;    // SHADOW_MAP

uniform vec3 inputAlbedo;
uniform vec3 inputAmbientLight;
uniform vec3 inputSunColor;
uniform vec3 inputSunDirection;
uniform vec3 inputViewDirection;
uniform vec3 inputViewPosition;

float approxErf(float x) {
    float a1 = 0.254829592;
    float a2 = -0.284496736;
    float a3 = 1.421413741;
    float a4 = -1.453152027;
    float a5 = 1.061405429;
    float p = 0.3275911;

    float sign = 1.0;
    if (x < 0.0)
    sign = -1.0;

    x = abs(x);

    float t = 1.0 / (1.0 + p * x);
    float y = (((((a5 * t + a4) * t) + a3) * t + a2) * t) + a1;

    return sign * (1.0 - y * exp(-x * x));
}

float VSM(in vec4 shadowCoord, in sampler2D depthMap, in float depthBias) {
    vec3 shadowCoordNormalized = shadowCoord.xyz / shadowCoord.w;
    shadowCoordNormalized = shadowCoordNormalized * 0.5 + 0.5;

    float depth = shadowCoordNormalized.z;
    float moments[2];
    moments[0] = depth;
    moments[1] = depth * depth;

    vec2 texelSize = 1.0 / textureSize(depthMap, 0);

    // Сбор моментов освещенности в окрестности texel
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            vec2 offset = vec2(x, y) * texelSize;
            float sampleDepth = texture(depthMap, shadowCoordNormalized.xy + offset).r;
            moments[0] += sampleDepth;
            moments[1] += sampleDepth * sampleDepth;
        }
    }

    // Вычисление среднего и дисперсии
    moments[0] /= 9.0;
    moments[1] /= 9.0;
    float variance = moments[1] - moments[0] * moments[0];

    // Применение depth bias
    float d = depth - depthBias;
    // Вычисление тени с использованием VSM
    float visibility = clamp(0.5 * (1.0 + approxErf((d - moments[0]) / (sqrt(2.0 * (variance))))), 0.0, 1.0);

    return visibility;
}

// Функция для separable blur
float separableBlur(in sampler2D textureSampler, in vec2 texCoords, in vec2 direction, in float resolution) {
    float value = 0.0;

    // Первый проход: blur по горизонтали
    for (float i = -1.0; i <= 1.0; i += 1.0) {
        value += texture(textureSampler, texCoords + direction * i / resolution).r;
    }

    // Нормализация
    value /= 3.0;

    return value;
}


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
