//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(int width, int height) {
    this->width = width;
    this->height = height;
    this->generateFrameBuffer();
}

void FrameBuffer::generateFrameBuffer() {
    texture = new Texture(width, height);
    glGenRenderbuffers(1, &framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
    glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture->getTextureID(), 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::invalid_argument(TAG + ": Can't create frame buffer.");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bindFrameBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, width / 2, height / 2);
}

Texture *FrameBuffer::getTexture() {
    return this->texture;
}

void FrameBuffer::setClearColor(glm::vec4 clearColor) {
    this->clearColor = clearColor;
}