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
    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->getTextureID(), 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::invalid_argument(TAG + ": Can't create frame buffer.");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bindFrameBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glViewport(0, 0, width, height);

    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Texture *FrameBuffer::getTexture() {
    return this->texture;
}

void FrameBuffer::setClearColor(glm::vec4 clearColor) {
    this->clearColor = clearColor;
}