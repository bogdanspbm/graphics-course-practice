//
// Created by Bogdan Madzhuga on 21.10.2023.
//

#ifndef PRACTICE6_FRAMEBUFFER_H
#define PRACTICE6_FRAMEBUFFER_H


#include <GL/glew.h>
#include <string>
#include "structures/Vector2I.h"
#include "objects/graphics/Texture.h"
#include "structures/Vector4F.h"

const char BufferVertexShader[] =
        R"(#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec2 center;
uniform vec2 size;

out vec2 texcoord;

vec2 vertices[6] = vec2[6](
    vec2(-1.0, -1.0),
    vec2( 1.0, -1.0),
    vec2( 1.0,  1.0),
    vec2(-1.0, -1.0),
    vec2( 1.0,  1.0),
    vec2(-1.0,  1.0)
);

void main()
{
    vec2 vertex = vertices[gl_VertexID];
    gl_Position = vec4(vertex * size + center, 0.0, 1.0);
    texcoord = vertex * 0.5 + vec2(0.5);
}
)";

const char BufferFragmentShader[] =
        R"(#version 330 core

in vec2 texcoord;


layout (location = 0) out vec4 out_color;

void main()
{
    out_color = vec4(texcoord, 0.0, 1.0);
}
)";

class FrameBuffer {
private:
    const std::string TAG = "FrameBuffer";

    Texture *texture;
    GLuint depthRenderbuffer;
    GLuint framebuffer;

    Vector4F clearColor = Vector4F{0.2f, 0.4f, 0.2f, 0.f};

    int width = 800, height = 400;
public:
    FrameBuffer(int width, int height);

    void bindFrameBuffer();

    Texture *getTexture();

    void setClearColor(Vector4F clearColor);

private:
    void generateFrameBuffer();
};


#endif //PRACTICE6_FRAMEBUFFER_H
