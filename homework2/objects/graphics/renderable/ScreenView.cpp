//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#include "ScreenView.h"

void ScreenView::draw() {
    GLProgram::getGLProgram(VIEW)->useProgram();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLProgram::getGLProgram()->setUniformVector2F("center", position);
    GLProgram::getGLProgram()->setUniformVector2F("center", size);

    glBindVertexArray(vao);

    texture->bindTexture();
}
