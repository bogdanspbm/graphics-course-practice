//
// Created by Bogdan Madzhuga on 22.10.2023.
//

#ifndef PRACTICE6_SCREENVIEW_H
#define PRACTICE6_SCREENVIEW_H


#include "structures/Vector2F.h"
#include "objects/graphics/Texture.h"
#include <string>
#include <GL/glew.h>
#include <SDL2/SDL_system.h>

const char ScreenViewVertexShader[] =
        R"(#version 330 core

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

const char ScreenViewFragmentShader[] =
        R"(#version 330 core

in vec2 texcoord;
uniform sampler2D textureLayer;

layout (location = 0) out vec4 out_color;

void main(){
     vec4 screenColor = texture(textureLayer, texcoord);
     out_color = screenColor;
}
)";

class ScreenView {

private:

    const std::string TAG = "ScreenView";

    Vector2F position;
    Vector2F size;

    GLuint id;
    GLuint vao;
    GLuint centerLocationID;
    GLuint sizeLocationID;
    Texture *texture;

public:
    ScreenView(Vector2F position, Vector2F size);
    void setTexture(Texture* texture);

    void draw(SDL_Window *window);

    void setPosition(Vector2F position);

protected:
    void bindTexture();
};


#endif //PRACTICE6_SCREENVIEW_H
