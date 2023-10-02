//
// Created by Bogdan Madzhuga on 16.09.2023.
//

#include "ProgramAdapter.h"

void ProgramAdapter::useProgram() {
    glUseProgram(this->id);
}

void ProgramAdapter::setUniformMatrix4FV(const GLchar *name, GLfloat *value) {
    GLint uniformLocationID = glGetUniformLocation(this->id, name);

    if (uniformLocationID == -1) {
        throw std::invalid_argument(TAG + ": Can't find uniform location.");
    } else {
        glUniformMatrix4fv(uniformLocationID, 1, GL_TRUE, value);
    }
}

void ProgramAdapter::cleanUniformMatrix4FV(const GLchar *name) {
    float matrix[16] =
            {
                    1.f, 0.f, 0.f, 0.f,
                    0.f, 1.f, 0.f, 0.f,
                    0.f, 0.f, 1.f, 0.f,
                    0.f, 0.f, 0.f, 1.f,
            };
    this->setUniformMatrix4FV(name, matrix);
}
