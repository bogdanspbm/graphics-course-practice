//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#ifndef HOMEWORK2_SCREENVIEW_H
#define HOMEWORK2_SCREENVIEW_H

#include "string"
#include "glm/vec2.hpp"
#include "objects/graphics/textures/Texture.h"
#include "objects/opengl/GLProgram.h"


class ScreenView {
private:
    const std::string TAG = "ScreenView";
    glm::vec2 position = {0.75, 0.75};
    glm::vec2 size = {0.25, 0.25};
    Texture *texture;
    GLuint vao;

public:
    ScreenView(Texture *texture) {
        GLProgram::getGLProgram(VIEW)->useProgram();
        glGenVertexArrays(1, &vao);

        this->texture = texture;
    };

    void setPosition(glm::vec2 position) {
        this->position = position;
    }

    void setSize(glm::vec2 size) {
        this->size = size;
    }

    void draw();



};


#endif //HOMEWORK2_SCREENVIEW_H
