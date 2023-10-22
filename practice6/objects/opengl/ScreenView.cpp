//
// Created by Bogdan Madzhuga on 22.10.2023.
//

#include "ScreenView.h"
#include "utils/ShaderUtils.h"

ScreenView::ScreenView(Vector2F position, Vector2F size) {
    this->position = position;
    this->size = size;

    GLuint fragmentShader = createFragmentShader(ScreenViewFragmentShader);
    GLuint vertexShader = createVertexShader(ScreenViewVertexShader);
    this->id = createProgram(vertexShader, fragmentShader);

    glUseProgram(id);
    glGenVertexArrays(1, &vao);

    centerLocationID = glGetUniformLocation(id, "center");
    sizeLocationID = glGetUniformLocation(id, "size");
}

void ScreenView::draw(SDL_Window *window) {
    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(0.8f, 0.8f, 1.f, 0.f);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(id);
    glUniform2f(centerLocationID, position.x, position.y);
    glUniform2f(sizeLocationID, size.x, size.y);
    glBindVertexArray(vao);

    bindTexture();

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void ScreenView::bindTexture() {

    if (texture == nullptr) {
        return;
    }

    glBindTexture(GL_TEXTURE_2D, texture->getTextureID());

    int textureLocation = glGetUniformLocation(this->id, texture->getLayerName().c_str());
    if (textureLocation != -1) {
        glUniform1i(textureLocation, 0); // 0 corresponds to GL_TEXTURE0
    }
}

void ScreenView::setTexture(Texture *texture) {
    this->texture = texture;
}

void ScreenView::setPosition(Vector2F position) {
    this->position = position;
}
