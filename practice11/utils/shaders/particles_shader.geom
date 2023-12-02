#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 inputViewPosition;

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in float size[];
in float rotation[];

out vec2 texcoord;


void main()
{
    vec3 center = gl_in[0].gl_Position.xyz;

    vec3 normalZ = -normalize(inputViewPosition - center);

    vec3 normalX = normalize(vec3(normalZ.y, -normalZ.x, 0));
    vec3 normalY = normalize(cross(normalX, normalZ));

    vec3 normalXR = normalX * cos(rotation[0]) - normalY * sin(rotation[0]);
    vec3 normalYR = normalX * sin(rotation[0]) + normalY * cos(rotation[0]);

    for (int i = 0; i < 4; i++) {
        vec3 posX = size[0] * normalXR * (i / 2 == 0 ? -1 : 1);
        vec3 posY = size[0] * normalYR * (i % 2 == 0 ? -1 : 1);
        vec3 pos = center + posX + posY;
        gl_Position = projection * view * model * vec4(pos, 1.0);
        texcoord = vec2((i / 2 == 0 ? 0 : 1), (i % 2 == 0 ? 0 : 1));
        EmitVertex();
    }

    EmitVertex();
    EndPrimitive();
}