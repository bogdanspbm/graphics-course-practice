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
    float tmin = tmintmax.x / 200;
    float tmax = tmintmax.y / 200;
    float opticalDepth = (tmax - tmin) * absorption.x;
    float opacity = 1.0 - exp(-opticalDepth);

    out_color =  vec4(vec3(opacity), opacity);
}