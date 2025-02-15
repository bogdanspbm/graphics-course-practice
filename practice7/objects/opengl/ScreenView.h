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
uniform int vertexFilter;

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
    gl_Position = vec4((vertex * size) + center, 0.0, 1.0);
    texcoord = vertex * 0.5 + vec2(0.5);
}
)";

const char ScreenViewFragmentShader[] =
        R"(#version 330 core

in vec2 texcoord;
uniform int fragmentFilter;
uniform sampler2D textureLayer;

layout (location = 0) out vec4 out_color;

const int kernelSize = 5;
const float sigma = 2.0;

float gaussian(float x, float sigma) {
    return exp(-(x * x) / (2.0 * sigma * sigma)) / (sqrt(2.0 * 3.14159265358979323846) * sigma);
}

void main(){
     vec2 dist_texcoord = texcoord;

     if(fragmentFilter == 2){
     dist_texcoord = texcoord + vec2(sin(texcoord.y * 50.0) * 0.01, 0.0);
     }

    if (fragmentFilter == 3) { // Gaussian blur filter
        vec2 texelSize = 1.0 / textureSize(textureLayer, 0); // Calculate the size of one texel
        vec4 blurColor = vec4(0.0);

        for (int i = -kernelSize; i <= kernelSize; ++i) {
            for (int j = -kernelSize; j <= kernelSize; ++j) {
                float weight = gaussian(float(i), sigma) * gaussian(float(j), sigma);
                blurColor += texture(textureLayer, dist_texcoord + vec2(float(i), float(j)) * texelSize) * weight;
            }
        }

        out_color = blurColor;
    } else { // Other filters (1 and 2)
        vec4 screenColor = texture(textureLayer, dist_texcoord);

        if (fragmentFilter == 1) {
            screenColor = floor(screenColor * 4.0) / 3.0;
        }

        out_color = screenColor;
    }
}
)";

class ScreenView {

private:

    const std::string TAG = "ScreenView";

    Vector2F position;
    Vector2F size;

    int filterID = 0;

    GLuint id;
    GLuint vao;
    GLuint fragmentFilterLocationID;
    GLuint vertexFilterLocationID;
    GLuint centerLocationID;
    GLuint sizeLocationID;
    Texture *texture;

public:
    ScreenView(Vector2F position, Vector2F size);

    void setTexture(Texture *texture);

    void draw(SDL_Window *window);

    void setPosition(Vector2F position);

    void setFilterID(int id);

protected:
    void bindTexture();
};


#endif //PRACTICE6_SCREENVIEW_H
