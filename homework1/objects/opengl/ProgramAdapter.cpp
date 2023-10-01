//
// Created by Bogdan Madzhuga on 16.09.2023.
//

#include "ProgramAdapter.h"

void ProgramAdapter::useProgram() {
    glUseProgram(this->id);
}

void ProgramAdapter::setScaleX(float scale) {
    this->scaleX = scale;
    this->setUniformMatrix4FV("view", this->calcTransformMatrix());
}

void ProgramAdapter::setScaleY(float scale) {
    this->scaleY = scale;
    this->setUniformMatrix4FV("view", this->calcTransformMatrix());
}

void ProgramAdapter::setOffsetX(float offset) {
    this->offsetX = offset;
    this->setUniformMatrix4FV("view", this->calcTransformMatrix());
}

void ProgramAdapter::setOffsetY(float offset) {
    this->offsetY = offset;
    this->setUniformMatrix4FV("view", this->calcTransformMatrix());
}

void ProgramAdapter::setRotation(float rotation) {
    this->rotationAngle = rotation;
    this->setUniformMatrix4FV("view", this->calcTransformMatrix());
}

GLfloat *ProgramAdapter::calcTransformMatrix() {
    float c = cos(this->rotationAngle);
    float s = sin(this->rotationAngle);
    GLfloat *transform = new GLfloat[16];

    transform[0] = this->scaleX * c;
    transform[1] = -this->scaleX * s;
    transform[2] = 0.0;
    transform[3] = 0.0;

    transform[4] = this->scaleY * s;
    transform[5] = this->scaleY * c;
    transform[6] = 0.0;
    transform[7] = 0.0;

    transform[8] = 0.0;
    transform[9] = 0.0;
    transform[10] = 1.0;
    transform[11] = 0.0;

    transform[12] = this->offsetX;
    transform[13] = this->offsetY;
    transform[14] = 0.0;
    transform[15] = 1.0;

    return transform;
}


void ProgramAdapter::setUniformMatrix4FV(const GLchar *name, GLfloat *value) {
    GLint uniformLocationID = glGetUniformLocation(this->id, name);

    if (uniformLocationID == -1) {
        throw std::invalid_argument(TAG + ": Can't find uniform location.");
    } else {
        glUniformMatrix4fv(uniformLocationID, 1, GL_FALSE, value);
    }
}

void ProgramAdapter::setUniform1F(const GLchar *name, GLfloat value) {
    GLint uniformLocationID = glGetUniformLocation(this->id, name);

    if (uniformLocationID == -1) {
        throw std::invalid_argument(TAG + ": Can't find uniform location.");
    } else {
        glUniform1f(uniformLocationID, value);
    }

}

void ProgramAdapter::setRatio(float ratio) {
    this->ratio = ratio;
    this->setUniformMatrix4FV("projection", this->calcViewMatrixRatio());
}

GLfloat *ProgramAdapter::calcViewMatrixRatio() {
    GLfloat *transform = new GLfloat[16];

    transform[0] = 1;
    transform[1] = 0.0;
    transform[2] = 0.0;
    transform[3] = 0.0;

    transform[4] = 0;
    transform[5] = this->ratio;
    transform[6] = 0.0;
    transform[7] = 0.0;

    transform[8] = 0.0;
    transform[9] = 0.0;
    transform[10] = 1.0;
    transform[11] = 0.0;

    transform[12] = 0;
    transform[13] = 0;
    transform[14] = 0.0;
    transform[15] = 1.0;

    return transform;
}

void ProgramAdapter::setResolution(int width, int height) {
    this->width = width;
    this->height = height;
    this->setUniformMatrix4FV("projection", this->calcViewMatrixResolution());
}

GLfloat *ProgramAdapter::calcViewMatrixResolution() {
    GLfloat *transform = new GLfloat[16];

    transform[0] = 2.0f / width;
    transform[1] = 0.0f;
    transform[2] = 0.0f;
    transform[3] = 0.0f;

    transform[4] = 0.0f;
    transform[5] = -2.0f / height;
    transform[6] = 0.0f;
    transform[7] = 0.0f;

    transform[8] = 0.0f;
    transform[9] = 0.0f;
    transform[10] = 1.0f;
    transform[11] = 0.0f;

    transform[12] = -1.0f;
    transform[13] = 1.0f;
    transform[14] = 0.0f;
    transform[15] = 1.0f;

    return transform;
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
