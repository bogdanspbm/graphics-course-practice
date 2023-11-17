//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#include "ScreenView.h"

void ScreenView::draw() {
    GLProgram::getGLProgram(VIEW)->useProgram();
    GLProgram::getGLProgram()->setUniformVector2F("center", position);
    GLProgram::getGLProgram()->setUniformVector2F("size", size);
    texture->bindTexture();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
