#version 330 core

uniform vec3 inputAlbedo;
uniform vec3 inputAmbientLight;
uniform vec3 inputSunColor;
uniform vec3 inputSunDirection;
uniform vec3 inputViewDirection;
uniform vec3 inputViewPosition;

uniform vec3 absorption;
uniform vec3 scattering;

uniform int enabledTextures[32];
uniform int textureChannels[32];

uniform sampler3D texture20;// DEFAULT 3D

uniform vec3 bBoxMin;
uniform vec3 bBoxMax;

layout (location = 0) out vec4 out_color;

void sort(inout float x, inout float y)
{
    if (x > y)
    {
        float t = x;
        x = y;
        y = t;
    }
}

float vmin(vec3 v)
{
    return min(v.x, min(v.y, v.z));
}

float vmax(vec3 v)
{
    return max(v.x, max(v.y, v.z));
}

vec2 intersect_bbox(vec3 origin, vec3 direction)
{
    vec3 tmin = (bBoxMin - origin) / direction;
    vec3 tmax = (bBoxMax - origin) / direction;

    sort(tmin.x, tmax.x);
    sort(tmin.y, tmax.y);
    sort(tmin.z, tmax.z);

    return vec2(vmax(tmin), vmin(tmax));
}

vec3 toTexcoords(vec3 p) {
    return (p - bBoxMin) / (bBoxMax - bBoxMin);
}

const float PI = 3.1415926535;

in vec3 position;

void main()
{
    vec3 direction = -normalize(inputViewPosition - position);
    vec2 tmintmax = intersect_bbox(inputViewPosition, direction);
    float tmin = tmintmax.x;
    tmin = max(0, tmin);
    float tmax = tmintmax.y;
    
    vec3 extinction = absorption + scattering;
    vec3 lightColor = vec3(16.0);

    vec3 color = vec3(0);

    vec3 opticalDepth = vec3(0);

    float N = textureChannels[20];
    float dt = (tmax - tmin) / N;
    for (int i = 0; i < N; i++) {
        float t = tmin + (i + 0.5) * dt;
        vec3 p = inputViewPosition + t * direction;
        vec3 texcoords = toTexcoords(p);
        float density = texture(texture20, texcoords).r;
        opticalDepth += extinction * density * dt;

        vec2 lightTmintmax = intersect_bbox(p, inputSunDirection);
        float lightTmin = lightTmintmax.x;
        lightTmin = max(0, lightTmin);
        float lightTmax = lightTmintmax.y;
        vec3 lightOpticalDepth = vec3(0);
        float lightN = 16;
        float light_dt = (lightTmax - lightTmin) / lightN;
        for (int j = 0; j < lightN; j++) {
            float t = lightTmin + (j + 0.5) * light_dt;
            vec3 lightP = p + t * inputSunDirection;
            vec3 texcoords = toTexcoords(lightP);
            float lightDensity = texture(texture20, texcoords).r;

            lightOpticalDepth += extinction * lightDensity * light_dt;
        }

        color += lightColor * exp(-lightOpticalDepth) * exp(-opticalDepth) * dt * density * scattering / 4.0 / PI;
    }

    //    float opacity = 1.0 - exp(-opticalDepth);

    //    vec3 color = vec3(48.0/255.0, 213.0/255.0, 200.0/255.0)/2;
    float alpha = 1;
    out_color = vec4(color, (color.x + color.y + color.z) / 3);
}