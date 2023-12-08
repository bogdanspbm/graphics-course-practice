#version 330 core

uniform vec3 camera_position;
uniform vec3 light_direction;
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

const float PI = 3.1415926535;

in vec3 position;

void main()
{
    vec3 direction = normalize(position - camera_position);
    vec2 intersect = intersect_bbox(position, direction);
    out_color = vec4(vec3(intersect.y - intersect.x) / 4, 1.0);
}