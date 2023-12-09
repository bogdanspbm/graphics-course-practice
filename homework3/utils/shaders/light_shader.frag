#version 330 core
in vec3 inputNormal;
in vec3 inputTangent;
in vec4 shadowPosition;
in vec2 texCoord;

uniform vec3 lightDirection;

uniform float roughness;
uniform float glossiness;
uniform float opacity;

uniform mat4 model;

uniform float lightRange;

uniform float useDisplacementMap;

uniform int enabledTextures[32];
uniform int useNormal;

uniform sampler2D texture0;// ALBEDO
uniform sampler2D texture1;// DISPLACEMENT_MAP
uniform sampler2D texture2;// GLOSS
uniform sampler2D texture3;// ROUGH
uniform sampler2D texture4;// ALPHA
uniform sampler2D texture5;// SPECULAR
uniform sampler2D texture6;// BUMP_MAP
uniform sampler2D texture7;// NORMAL
uniform sampler2D texture8;// REFLECTION
uniform sampler2D texture9;//

uniform sampler2D texture31;// SHADOW_MAP

const float PI = 3.141592653589793;

uniform vec3 inputAlbedo;
uniform vec3 inputAmbientLight;
uniform vec3 inputSunColor;
uniform vec3 inputSunDirection;
uniform vec3 inputViewDirection;
uniform vec3 inputViewPosition;


layout (location = 0) out vec4 outColor;

vec3 getBumpOffsets(vec2 texCoord) {

    vec2 resolution = vec2(textureSize(texture6, 0));

    float pixelDeltaX = 1.0 / resolution.x;
    float pixelDeltaY = 1.0 / resolution.y;

    vec2 up    = vec2(0.0, pixelDeltaY);
    vec2 down  = vec2(0.0, -pixelDeltaY);
    vec2 left  = vec2(-pixelDeltaX, 0.0);
    vec2 right = vec2(pixelDeltaX, 0.0);

    vec4 rightColor = texture(texture6, texCoord + right);
    vec4 leftColor  = texture(texture6, texCoord + left);
    vec4 upColor    = texture(texture6, texCoord + up);
    vec4 downColor  = texture(texture6, texCoord + down);

    float center = texture(texture6, texCoord).x;

    float deltaX = rightColor.r - leftColor.r;
    float delyaY = upColor.r - downColor.r;

    return vec3(deltaX,delyaY, 0);
}

void main()
{
    vec4 textureColor = texture(texture0, texCoord);

    vec3 shadowTextCoord = shadowPosition.xyz / shadowPosition.w;
    shadowTextCoord.xyz = shadowTextCoord.xyz * 0.5 + 0.5;

    vec3 bitangent = cross(inputTangent, inputNormal);
    mat3 tbn = mat3(inputTangent, bitangent, inputNormal);
    vec3 normal = inputNormal;

    if (enabledTextures[7] == 1 && useNormal == 1){
        vec3 normilizedMapNormal = texture(texture7, texCoord).xyz * 2.0 - 1.0;
        normal = tbn * normilizedMapNormal;
    }

    if (enabledTextures[6] == 1 && useNormal == 1) {
        vec3 normalOffset = getBumpOffsets(texCoord);
        normal = normal + 2 * inputTangent * normalOffset.x + 2 * bitangent * normalOffset.y;
    }

    vec3 ambientLight = inputAmbientLight;
    vec3 reflectDir = reflect(-lightDirection, normal);

    vec3 envAlbedo = vec3(0);

    if (enabledTextures[8] == 1){
        float x = atan(reflectDir.z, reflectDir.x) / PI * 0.5 + 0.5;
        float y = -atan(reflectDir.y, length(reflectDir.xz)) / PI + 0.5;
        envAlbedo = texture(texture8, vec2(x, y)).xyz;
    }

    float finalGlossines = glossiness;

    if (enabledTextures[2] == 1){
        finalGlossines = texture(texture2, texCoord).x;
    }

    vec2 s = vec2(0.0, 0.0);
    vec2 w = vec2(0.0, 0.0);
    const int N = 7;
    float radius = 5.0;
    for (int x = -N; x <= N; ++x) {
        for (int y = -N; y <= N; ++y) {
            float c = exp(-float(x * x + y * y) / (radius*radius));
            s += c * texture(texture31, shadowTextCoord.xy + vec2(x, y) / vec2(textureSize(texture31, 0))).rg;
            w += c;
        }
    }

    vec2 data = s / w;
    float mu = data.r;
    float sigma = data.g - mu * mu;
    float z = shadowTextCoord.z - 0.001;
    float factor = (z < mu) ? 1.0 : sigma / (sigma + (z - mu) * (z - mu));
    float delta = 0.125;
    float sFactor = (factor < delta) ? 0.0 : (factor - delta) / (1 - delta);


    float spec = pow(max(dot(inputViewDirection, reflectDir), 0.0), finalGlossines);
    vec3 specular = spec * inputSunColor;

    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse =  diff * inputSunColor  * roughness;

    float lightness = ambientLight.x + max(0.0, dot(normalize(normal), inputSunDirection)) * sFactor;

    vec4 materialColor = texture(texture7, texCoord);

    if (enabledTextures[0] == 0){
        materialColor.xyz = inputAlbedo;
    }

    vec3 color = materialColor.xyz * lightness + envAlbedo / 2;

    outColor = vec4(vec3(color), 1);
}
