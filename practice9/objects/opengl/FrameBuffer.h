//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#ifndef HOMEWORK2_FRAMEBUFFER_H
#define HOMEWORK2_FRAMEBUFFER_H

#include <string>
#include "objects/graphics/textures/Texture.h"
#include "glm/vec4.hpp"

class FrameBuffer {
private:
    const std::string TAG = "FrameBuffer";

    Texture *texture;
    GLuint framebuffer;

    glm::vec4 clearColor = {1.0f, 1.0f, 1.0f, 1.f};

    int width = 800, height = 400;
public:
    FrameBuffer(int width, int height);

    void bindFrameBuffer();

    Texture *getTexture();

    void setClearColor(glm::vec4 clearColor);

private:
    void generateFrameBuffer();
};


#endif //HOMEWORK2_FRAMEBUFFER_H
